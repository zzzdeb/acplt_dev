/******************************************************************************
 *
 *   FILE
 *   ----
 *   player.c
 *
 *   History
 *   -------
 *   Do 29 Nov 2018 15:37:07 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_sync
#define OV_COMPILE_LIBRARY_sync
#endif

#include "sync.h"
#include "sync_helper.h"

#include "PostSys.h"
#include "PostSys_helpers.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"

OV_DLLFNCEXPORT OV_RESULT sync_player_switch_set(OV_INSTPTR_sync_player pinst,
                                                 const OV_BOOL          value) {
  pinst->v_switch = value;
  if(value) {
    pinst->p_bufNode.v_status = BUFFERER_NOBUFFERING;
    pinst->p_dstNode.v_actimode = 1;
  }
  return 0;
}

OV_DLLFNCEXPORT void sync_player_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_player pinst = Ov_StaticPtrCast(sync_player, this);
  /* code */

  OV_RESULT                      result = OV_ERR_OK;
  OV_INSTPTR_PostSys_MsgDelivery pMsgDelivery = NULL;

  OV_INSTPTR_sync_bufferer pBufNode = &pinst->p_bufNode;

  OV_INSTPTR_sync_Buffer   pBuffer = &pBufNode->p_buffer;
  OV_INSTPTR_sync_dstNode  pNode = &pinst->p_dstNode;
  OV_INSTPTR_PostSys_inbox pNodeInb =
      Ov_SearchChildEx(ov_containment, pNode, "inbox", PostSys_inbox);

  OV_UINT srcPreLen = ov_string_getlength(pinst->v_srcPath);
  OV_UINT dstPreLen = ov_string_getlength(pinst->v_dstPath);

  // TODO: zzz: dummy implementation. Has to be worked on :2018 Dez 19 23:24
  OV_INSTPTR_PostSys_Message pBufMsg = NULL;
  Ov_ForEachChildEx(ov_containment, pBuffer, pBufMsg, PostSys_Message) {
    switch(pBufMsg->v_msgStatus) {
      case MSGNEW:
        pBufMsg->v_msgStatus = MSGREADYFORSENDING;
      case MSGREADYFORSENDING:
        break;
      case MSGWAITING:
        continue;
        break;
      default:
        ov_logfile_error("sync_player: unknown state %d", pBufMsg->v_msgStatus);
        return;
    }
    OV_UINT pathLen = pBufMsg->v_pathLen;
    if(ov_string_match(pBufMsg->v_msgBody, "<bdy Op=\"RES:*")) {
      /* changing settings */
      /* edit comp */
      for(OV_UINT i = 0; i < pathLen; ++i) {
        if(!strncmp(pBufMsg->v_pathComponent.value[i], pinst->v_srcPath,
                    srcPreLen) &&
           ov_string_compare(pinst->v_srcHost,
                             pBufMsg->v_pathAddress.value[i]) ==
               OV_STRCMP_EQUAL &&
           ov_string_compare(pinst->v_srcServer,
                             pBufMsg->v_pathName.value[i]) == OV_STRCMP_EQUAL) {
          ov_string_setvalue(&pBufMsg->v_pathAddress.value[i],
                             pinst->v_dstHost);
          ov_string_setvalue(&pBufMsg->v_pathName.value[i], pinst->v_dstServer);
          OV_STRING tmpStr = NULL;
          ov_string_setvalue(&tmpStr, pinst->v_dstPath);
          ov_string_append(&tmpStr,
                           pBufMsg->v_pathComponent.value[i] + srcPreLen);
          ov_string_setvalue(&pBufMsg->v_pathComponent.value[i], NULL);
          pBufMsg->v_pathComponent.value[i] = tmpStr;
        }
      }
      pMsgDelivery = Ov_StaticPtrCast(
          PostSys_MsgDelivery,
          Ov_GetFirstChild(ov_instantiation, pclass_PostSys_MsgDelivery));
      if(!pMsgDelivery) {
        ov_logfile_error("sync_player: no msgdelivery found");
        return;
      }
      PostSys_MsgDelivery_sendMessage(pMsgDelivery, pBufMsg);
      continue;
    }
    break;
  }
  if(!pBufMsg) {
    switch(pinst->v_status) {
      case PLAYER_INIT:
      case PLAYER_PLAYING:
        ov_logfile_debug("sync_player: buffer empty. switching");
        /* send all msgs in nodeInbox */
        /* pNode->v_actimode = 1; */
        /* pBufNode->v_status = 1; */
        /* ready to go*/
        pinst->v_status = PLAYER_WAITING_RESULTS;
        OV_INSTPTR_sync_dsyncDst psyncDst =
            Ov_StaticPtrCast(sync_dsyncDst, pinst->v_pouterobject);
        result = sync_dsyncDst_switch_set(psyncDst, 1);
        if(Ov_Fail(result)) {
          pinst->v_status = PLAYER_ERROR;
          return;
        }

        break;
      case PLAYER_WAITING_RESULTS:
        if(!pinst->p_bufNode.v_msgsWaitingRes.veclen) {
          pinst->v_status = PLAYER_DONE;
          pinst->v_actimode = 0;
        }
        break;
      case PLAYER_DONE:
      case PLAYER_ERROR:
        pinst->v_actimode = 0;
        break;
      default:
        ov_logfile_error("sync_player: wrong status %d", pinst->v_status);
        return;
    }
    return;
  }

  OV_INSTPTR_PostSys_Message pMsg = NULL;
  Ov_ForEachChildEx(ov_containment, pNodeInb, pMsg, PostSys_Message) { break; }
  if(!pMsg) {
    return;
  }

  OV_UINT pathLen = pMsg->v_pathLen;
  Ov_ForEachChildEx(ov_containment, pNodeInb, pMsg, PostSys_Message) {
    /*comparing 2 msgs*/
    if(pMsg->v_pathLen != pBufMsg->v_pathLen) {
      break;
    }
    for(OV_UINT i = 0; i < pathLen; ++i) {
      if(ov_string_compare(pMsg->v_pathComponent.value[i] + srcPreLen,
                           pBufMsg->v_pathComponent.value[i] + dstPreLen) !=
         OV_STRCMP_EQUAL) {
        break;
      }
    }
    ov_logfile_info("sync_player: deleting 2 msgs");
    Ov_DeleteObject(pMsg);
    Ov_DeleteObject(pBufMsg);
    break;
  }
  /*checking if buffer is now empty*/
  Ov_ForEachChildEx(ov_containment, pBuffer, pBufMsg, PostSys_Message) {
    if(pBufMsg->v_msgStatus == MSGWAITING) {
      continue;
    }
    break;
  }
  if(!pBufMsg) {
    /* ready to go*/
    switch(pinst->v_status) {
      case PLAYER_INIT:
      case PLAYER_PLAYING:
        ov_logfile_debug("sync_player: buffer empty. switching");
        /* send all msgs in nodeInbox */
        /* pNode->v_actimode = 1; */
        /* pBufNode->v_status = 1; */
        /* ready to go*/
        pinst->v_status = PLAYER_WAITING_RESULTS;
        OV_INSTPTR_sync_dsyncDst psyncDst =
            Ov_StaticPtrCast(sync_dsyncDst, pinst->v_pouterobject);
        sync_dsyncDst_switch_set(psyncDst, 1);

        break;

      case PLAYER_WAITING_RESULTS:
        if(!pinst->p_bufNode.v_msgsWaitingRes.veclen) {
          pinst->v_status = PLAYER_DONE;
          pinst->v_actimode = 0;
        }
        break;
      case PLAYER_DONE:
      case PLAYER_ERROR:
        pinst->v_actimode = 0;
        break;
      default:
        ov_logfile_error("sync_player: wrong status %d", pinst->v_status);
    }
  }
  return;
}

