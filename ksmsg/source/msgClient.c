

/******************************************************************************
 *
 *   FILE
 *   ----
 *   msgClient_main.c
 *
 *   History
 *   -------
 *   2013-02-22   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

/************************************************************************************************************************************************************************
 * The msgClient Handler is based on a state machine
 * the states are
 * INITIAL:
 * 		nothing is done. one can start from here. typemethod is
 *deactivated. AWAITINGCONNECTION: a ks-service was requested. The associated
 *channel is opening a connection. the typemethod polls the connection status of
 *the channel. if a connection is opened the request is sent and the state is
 *changed to AWAITINGANSWER if the connection does not open withing timeout
 *seconds the typemethod sets an errorstate and deactivates itself
 * AWAITINGANSWER
 * 		the typemethod just calculates the timeout.
 * 		upon receipt of a reply the HandleData function is called by the
 *Channel HandleData tries to process the generic part of the message. if handle
 *data fails because the message is incomplete the state is changed to BUSY. if
 *it fails for some other reason the state is changed to ERROR and the
 *typemethod is deactivated if HandleData succeeds the state is changed to
 *COMPLETED BUSY the typemethod calls HandleData again to try processing the
 *answer. state changes are the same as in AWAITINGANSWER the timeout is
 *calculated ERROR the typemethod is deactivated
 *
 ************************************************************************************************************************************************************************/

#ifndef OV_COMPILE_LIBRARY_ksmsg
#define OV_COMPILE_LIBRARY_ksmsg
#endif

#include "CTree_helper.h"
#include "PostSys_helper.h"
#include "acplt_simpleMsgHandling.h"
#include "cJSON.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"
#include "ksmsg.h"
#include "ksmsg_helper.h"
#include "ksxdr.h"
#include "ksxdr_config.h"
#include "ksxdr_services.h"
#include "libov/ov_macros.h"
#include "libov/ov_malloc.h"
#include "libov/ov_memstack.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include "ov_ksserver_backend.h"

OV_DLLFNCEXPORT OV_RESULT
                ksmsg_msgClient_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT result = OV_ERR_OK;
  OV_INSTPTR_ksmsg_msgClient this = Ov_StaticPtrCast(ksmsg_msgClient, pobj);

  result = ksbase_ComTask_constructor(pobj);
  if(result)
    return result;
  OV_INSTPTR_ov_domain pInbox = NULL;
  result = Ov_CreateObject(ov_domain, pInbox, this, "Inbox");
  ksmsg_msgClient_pathLen_set(this, 2);
  OV_ANY servername = {0};
  servername.value.vartype = OV_VT_VOID;
  servername.value.valueunion.val_string = NULL;
  result |= ov_vendortree_getservername(&servername, NULL);
  result |= ov_string_setvalue(
      &this->v_pathInstance.value[0],
      ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, this), 2));
  result |= ov_string_setvalue(&this->v_pathHost.value[0], "localhost");
  result |= ov_string_setvalue(&this->v_pathName.value[0],
                               servername.value.valueunion.val_string);
  result |= ov_string_setvalue(&this->v_pathInstance.value[1], EXECUTERPATH);

  this->v_answerItems = NULL;

  return result;
}

OV_DLLFNCEXPORT void ksmsg_msgClient_destructor(OV_INSTPTR_ov_object pobj) {
  OV_INSTPTR_ksmsg_msgClient this = Ov_StaticPtrCast(ksmsg_msgClient, pobj);
  ksbase_ComTask_destructor(pobj);

  Ov_HeapFree(this->v_answerItems);
  this->v_answerItemsLen = 0;
  return;
}

OV_DLLFNCEXPORT
OV_RESULT
ksmsg_msgClient_path_deleteElement(OV_INSTPTR_ksmsg_msgClient pMsgClnt,
                                   OV_UINT                    ind) {
  OV_RESULT result = OV_ERR_OK;

  OV_UINT newLen = pMsgClnt->v_pathLen - 1;
  if(ind >= pMsgClnt->v_pathLen) {
    ov_logfile_debug("ksmsg_msgClient: index too big");
    return OV_ERR_BADVALUE;
  }
  OV_STRING tmpStr = NULL;
  for(OV_UINT i = ind; i < newLen; ++i) {
    tmpStr = pMsgClnt->v_pathHost.value[i];
    pMsgClnt->v_pathHost.value[i] = pMsgClnt->v_pathHost.value[i + 1];
    pMsgClnt->v_pathHost.value[i + 1] = tmpStr;
    tmpStr = pMsgClnt->v_pathName.value[i];
    pMsgClnt->v_pathName.value[i] = pMsgClnt->v_pathName.value[i + 1];
    pMsgClnt->v_pathName.value[i + 1] = tmpStr;
    tmpStr = pMsgClnt->v_pathInstance.value[i];
    pMsgClnt->v_pathInstance.value[i] = pMsgClnt->v_pathInstance.value[i + 1];
    pMsgClnt->v_pathInstance.value[i + 1] = tmpStr;
  }
  pMsgClnt->v_pathLen = newLen;
  result |= Ov_SetDynamicVectorLength(&pMsgClnt->v_pathHost, newLen, STRING);
  result |= Ov_SetDynamicVectorLength(&pMsgClnt->v_pathName, newLen, STRING);
  result |=
      Ov_SetDynamicVectorLength(&pMsgClnt->v_pathInstance, newLen, STRING);
  return result;
}

OV_DLLFNCEXPORT OV_UINT
                ksmsg_msgClient_msgsInQueue_get(OV_INSTPTR_ksmsg_msgClient pobj) {
  OV_UINT                    len = 0;
  OV_INSTPTR_PostSys_Message pmsg = NULL;
  Ov_ForEachChildEx(ov_containment, pobj, pmsg, PostSys_Message) { len++; }
  return len;
}

OV_DLLFNCEXPORT OV_RESULT ksmsg_msgClient_pathLen_set(
    OV_INSTPTR_ksmsg_msgClient pobj, const OV_UINT value) {
  pobj->v_pathLen = value;
  OV_STRING_VEC* tmp[] = {&pobj->v_pathHost, &pobj->v_pathName,
                          &pobj->v_pathInstance};
  OV_UINT        len = tmp[1]->veclen;
  for(OV_UINT i = 0; i < 3; i++) {
    if(len != value) {
      Ov_SetDynamicVectorLength(tmp[i], value, STRING);
      if(len < value) {
        for(OV_UINT j = len; j < value; j++) {
          tmp[i]->value[j] = NULL;
        }
      }
    }
  }
  return 0;
}

OV_RESULT msgClient_handleMessage(OV_INSTPTR_ksmsg_msgClient pinst,
                                  OV_INSTPTR_PostSys_Message pMsg) {
  OV_RESULT     result = OV_ERR_OK;
  OV_STRING     svc = NULL;
  OV_STRING     op = NULL;
  OV_STRING     container = NULL;
  OV_STRING_VEC ids = {0};
  OV_STRING_VEC vals = {0};
  OV_STRING_VEC types = {0};
  OV_STRING_VEC units = {0};
  // TODO: zzz: check param Do 13 Dez 2018 23:16:24 CET
  ov_memstack_lock();
  acplt_simpleMsg_parseFlatBody(pMsg->v_msgBody, &svc, &op, 0, &container, &ids,
                                &vals, &units, &types);
  cJSON*  jsvalue = NULL;
  OV_UINT len = vals.veclen;

  if(pinst->v_answerItems) {
    Ov_HeapRealloc(pinst->v_answerItems, len * sizeof(OV_GETVAR_PAR));
  } else {
    pinst->v_answerItems = Ov_HeapMalloc(len * sizeof(OV_GETVAR_PAR));
  }
  for(OV_UINT i = 0; i < len; ++i) {
    if(ov_string_compare(op, "RES:GET") == OV_STRCMP_EQUAL) {
      // TODO: zzz: check if value is only result Di 11 Dez 2018 15:25:20 CET
      jsvalue = cJSON_Parse(vals.value[i]);
      if(!jsvalue) {
        ov_logfile_error("ksmsg_msgClient: bad json format %s", vals.value[i]);
        pinst->v_state = KSBASE_CLST_ERROR;
        ov_memstack_unlock();
        return OV_ERR_BADVALUE;
      }
      if(cJSON_IsArray(jsvalue) || cJSON_GetArraySize(jsvalue) == 2) {
        result = jsonToVarvalue(
            &pinst->v_answerItems[i].var_current_props.value, jsvalue);
        if(Ov_Fail(result)) {
          ov_logfile_error("%u: %s: couldnt convert json to Varvalu", result,
                           ov_result_getresulttext(result));
          pinst->v_state = KSBASE_CLST_ERROR;
          ov_memstack_unlock();
          return OV_ERR_GENERIC;
        }
        pinst->v_answerItems[i].result = 0;
      } else if(cJSON_IsNumber(jsvalue)) {
        pinst->v_answerItems[i].result = jsvalue->valueint;
      } else {
        ov_logfile_warning("ksmsg_msgClient: json NULL");
        pinst->v_answerItems[i].result = jsvalue->valueint =
            atoi(vals.value[i]);
      }
    } else if(ov_string_compare(op, "RES:SET") == OV_STRCMP_EQUAL) {
      pinst->v_answerItems[i].result = atoi(vals.value[i]);
    }
  }
  ov_memstack_unlock();
  return result;
}

/*******************************************************************************************************************************************************************************
 * 				typemethod
 *******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT
void ksmsg_msgClient_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  OV_INSTPTR_ksmsg_msgClient thisCl = Ov_StaticPtrCast(ksmsg_msgClient, this);
  OV_TIME                    now;
  OV_TIME_SPAN               tstemp;
  OV_RESULT                  result;

  OV_INSTPTR_PostSys_Message     pMsg = NULL;
  OV_INSTPTR_PostSys_Message     pMsgTmp = NULL;
  OV_INSTPTR_PostSys_MsgDelivery pMsgDelivery = NULL;
  OV_INSTPTR_ov_domain           pInbox =
      Ov_SearchChildEx(ov_containment, thisCl, "Inbox", ov_domain);

  /*
   * state machine
   */
  switch(thisCl->v_state) {
    case KSBASE_CLST_INITIAL:
      Ov_ForEachChildEx(ov_containment, thisCl, pMsg, PostSys_Message) {
        break;
      }
      if(pMsg) {
        switch(pMsg->v_msgStatus) {
          case MSGREADYFORSENDING:
            break;
          case MSGNEW:
          case MSGWAITING:
          case MSGDONE:
          case MSGFATALERROR:
          default:
            ov_logfile_debug("ksmsg_msgClient: deleting msg with status %d",
                             pMsg->v_msgStatus);
            Ov_DeleteObject(pMsg);
            return;
        }
        pMsgDelivery = Ov_GetParent(PostSys_MsgDelivery2Message, pMsg);
        if(pMsgDelivery) { /*	this message was already tried to be send
                            */
        } else {
          pMsgDelivery = Ov_StaticPtrCast(
              PostSys_MsgDelivery,
              Ov_GetFirstChild(ov_instantiation, pclass_PostSys_MsgDelivery));
          if(pMsgDelivery) {
            ov_logfile_debug("ksmsg_msgClient: sending msg");
            PostSys_MsgDelivery_sendMessage(pMsgDelivery, pMsg);
            ov_time_gettime(&thisCl->v_timeLastEvent);
          }
        }
        thisCl->v_state = KSBASE_CLST_AWAITINGCONNECTION;
      }
      break;
    case KSBASE_CLST_AWAITINGCONNECTION: /*	check if connection is open now.
                                            if so, send. otherwise calculate
                                            timeout and return	*/
      Ov_ForEachChildEx(ov_containment, thisCl, pMsg, PostSys_Message) {
        break;
      }
      if(pMsg) {
        pMsgDelivery = Ov_GetParent(PostSys_MsgDelivery2Message, pMsg);
        if(pMsgDelivery) { /*	this message was already tried to be send
                            */
          ov_time_gettime(&now);
          ov_time_diff(&tstemp, &now, &(thisCl->v_timeLastEvent));
          if((tstemp.secs > (OV_INT)thisCl->v_timeout) ||
             ((tstemp.secs == (OV_INT)thisCl->v_timeout) &&
              (tstemp.usecs > 0))) {
            thisCl->v_state = KSBASE_CLST_ERROR | XDRCL_TIMEOUT;
            thisCl->v_actimode = 0;
          }
        }
      } else {
        thisCl->v_state = KSBASE_CLST_AWAITINGANSWER;
        // TODO: zzz: if msg is sent go to awaitinganswer Mo 10 Dez 2018
        // 18:21:49 CET
        // TODO: zzz: delete better the message So 09 Dez 2018 10:44:18 CET
        return;

        case KSBASE_CLST_AWAITINGANSWER: /*	waiting for answer --> just
                                            calculate timeouts	*/
        case KSBASE_CLST_BUSY: /*	call HandleData again to process data
                                */
                               /*	calculate timeout	*/
          Ov_ForEachChildEx(ov_containment, pInbox, pMsgTmp, PostSys_Message) {
            pMsg = pMsgTmp;
          }
          if(pMsg) {
            result = msgClient_handleMessage(thisCl, pMsg);
            if(Ov_Fail(result)) {
              thisCl->v_state = KSBASE_CLST_ERROR;
              ov_logfile_error("%u: %s: could not handle the message", result,
                               ov_result_getresulttext(result));
              return;
            }
            Ov_DeleteObject(pMsg);
            thisCl->v_state = KSBASE_CLST_COMPLETED;
            break;
          }
          ov_time_gettime(&now);
          ov_time_diff(&tstemp, &now, &(thisCl->v_timeLastEvent));
          if((tstemp.secs > (OV_INT)thisCl->v_timeout) ||
             ((tstemp.secs == (OV_INT)thisCl->v_timeout) &&
              (tstemp.usecs > 0))) {
            thisCl->v_state = KSBASE_CLST_ERROR | XDRCL_TIMEOUT;
            thisCl->v_actimode = 0;
          }
          return;
        case KSBASE_CLST_COMPLETED: /*	issue the callback	*/
          thisCl->v_state = KSBASE_CLST_INITIAL;
          thisCl->v_actimode = 0;
          KS_logfile_debug(
              ("%s: completed. issuing callback", thisCl->v_identifier));
          if(thisCl->v_callback.callbackFunction)
            thisCl->v_callback.callbackFunction(
                thisCl->v_callback.instanceCalled,
                Ov_StaticPtrCast(ov_domain, this));
          // TODO: zzz: close connection Mo 10 Dez 2018 18:25:03 CET
          return;
        case KSBASE_CLST_ERROR:
          thisCl->v_actimode = 0;
          return;
        default:
          KS_logfile_debug(("entering default in msgClient typemethod - this "
                            "should not happen"));
          thisCl->v_actimode = 0;
          break;
      }

      return;
  }
}

/*******************************************************************************************************************************************************************************
 * 				Reset
 *******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT
                ksmsg_msgClient_reset(OV_INSTPTR_ksbase_ClientBase this) {
  OV_RESULT result = OV_ERR_OK;
  result = ksxdr_xdrClient_reset(this);
  OV_INSTPTR_PostSys_Message pmsg = NULL;
  OV_INSTPTR_ov_domain       pinbox =
      Ov_SearchChildEx(ov_containment, this, "Inbox", ov_domain);
  if(!pinbox) {
    return 0;
  }
  Ov_ForEachChildEx(ov_containment, pinbox, pmsg, PostSys_Message) {
    result |= Ov_DeleteObject(pmsg);
  }
  return result;
}
