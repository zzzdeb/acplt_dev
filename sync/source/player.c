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

#include "PostSys.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"

const OV_STRING SRCNODENAME = "srcNode";
const OV_STRING DSTNODENAME = "logNode";
const OV_STRING PLAYERBUFFERNAME = "buffer";

OV_DLLFNCEXPORT OV_RESULT sync_player_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT              result = OV_ERR_OK;
  OV_INSTPTR_sync_player pinst = Ov_StaticPtrCast(sync_player, pobj);
  OV_INSTPTR_ov_domain   pNode = NULL;
  OV_INSTPTR_ov_object   pInbox = NULL;

  result = ov_object_constructor(pobj);
  result |= Ov_CreateObject(ov_domain, pNode, pinst, SRCNODENAME);
  result |= Ov_CreateObject(sync_sscBuffer, pInbox, pNode, "inbox");
  result |= Ov_CreateObject(ov_domain, pNode, pinst, DSTNODENAME);
  result |= Ov_CreateObject(sync_dstNode, pInbox, pNode, "inbox");
  // TODO: zzz: not ready  Do 29 Nov 2018 16:01:39 CET
  return result;
}

OV_DLLFNCEXPORT void sync_player_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_player pinst = Ov_StaticPtrCast(sync_player, this);
  /* code */

  OV_RESULT result = OV_ERR_OK;
  OV_STRING bufferPath = NULL;
  ov_string_print(
      &bufferPath, "%s/%s/inbox/%s",
      ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pinst), 2),
      SRCNODENAME, PLAYERBUFFERNAME);

  OV_INSTPTR_ov_domain pBuffer =
      Ov_StaticPtrCast(ov_domain, ov_path_getobjectpointer(bufferPath, 2));

  OV_STRING pNodeInboxPath = NULL;
  ov_string_print(
      &pNodeInboxPath, "%s/%s/inbox",
      ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pinst), 2),
      DSTNODENAME);

  OV_INSTPTR_ov_domain pNodeInbox =
      Ov_StaticPtrCast(ov_domain, ov_path_getobjectpointer(pNodeInboxPath, 2));

  // TODO: zzz: dummy implementation. Has to be worked on.  Do 06 Dez 2018
  // 17:45:05 CET

  OV_INSTPTR_sync_bufferMessage pMsg = NULL;
  Ov_ForEachChildEx(ov_containment, pNodeInbox, pMsg, PostSys_Message) {
    break;
  }
  OV_INSTPTR_PostSys_MsgDelivery pMsgDelivery = NULL;

  if(pMsg) {
    pMsgDelivery = Ov_GetParent(PostSys_MsgDelivery2Message, pMsg);
    if(pMsgDelivery) { /*	this message was already tried to be send
                        */
      // TODO: zzz: num of tries: Sa 01 Dez 2018 18:34:50 CET
      //			pinst->v_tries++;
      //			if(pinst->v_tries > 3) {
      //				Ov_DeleteObject(pMsg);
      //				Ov_ForEachChildEx(ov_containment, pinst,
      // pMsg, PostSys_Message)
      //				{
      //					break;
      //				}
      //				if(pMsg) {
      //					Ov_Link(PostSys_MsgDelivery2Message,
      // pMsgDelivery, pMsg); 					pinst->v_tries
      // = 0;
      //				}
      //			}
    } else {
      pMsgDelivery = Ov_StaticPtrCast(
          PostSys_MsgDelivery,
          Ov_GetFirstChild(ov_instantiation, pclass_PostSys_MsgDelivery));
      if(pMsgDelivery) {
        Ov_Link(PostSys_MsgDelivery2Message, pMsgDelivery, pMsg);
        //				pinst->v_tries = 0;
      }
    }
  }

  return;
}

