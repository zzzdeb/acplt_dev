/*
 * msgClient_VariableFuncs.c
 *
 *  Created on: 24.04.2013
 *      Author: lars
 */

#ifndef OV_COMPILE_LIBRARY_ksmsg
#define OV_COMPILE_LIBRARY_ksmsg
#endif

/* #include "KSDATAPACKET_xdrhandling.h" */
#include "PostSys.h"
#include "PostSys_helpers.h"
#include "acplt_simpleMsgHandling.h"
#include "ks_logfile.h"
#include "ksbase.h"
#include "ksbase_helper.h"
#include "ksmsg.h"
#include "ksxdr.h"
#include "ksxdr_config.h"
#include "ksxdr_services.h"
#include "libov/ov_macros.h"
#include "libov/ov_malloc.h"
#include "libov/ov_result.h"
#include "ov_ksserver_backend.h"

#include "ksbase.ovt"

/*******************************************************************************************************************************************************************************
 * 				getVar
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksmsg_msgClient_requestGetVar(
    const OV_INSTPTR_ksbase_ClientBase this,
    const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
    const OV_UINT items_length, const OV_STRING* paths,
    const OV_INSTPTR_ov_domain callbackThat,
    void (*callback)(const OV_INSTPTR_ov_domain this,
                     const OV_INSTPTR_ov_domain that)) {
  ov_logfile_debug("ksmsg_ksmsg_getVar: requistGetVar");
  /*
   *   local variables
   */
  OV_RESULT                  result = OV_ERR_OK;
  OV_STRING                  msgBody = NULL;
  OV_UINT                    pathLen = 0;
  OV_INSTPTR_PostSys_Message pMsg = NULL;
  // TODO: zzz: paramcheck Sa 08 Dez 2018 23:57:24 CET

  /* OV_UINT                    msgindex = 0; */
  OV_INSTPTR_ksmsg_msgClient pobj = Ov_StaticPtrCast(ksmsg_msgClient, this);

  if((pobj->v_state != KSBASE_CLST_COMPLETED) &&
     (pobj->v_state != KSBASE_CLST_INITIAL))
    return OV_ERR_NOACCESS;
  /*	set callback function	*/
  pobj->v_callback.instanceCalled = callbackThat;
  pobj->v_callback.callbackFunction = callback;

  /*	set ProcID	*/
  pobj->v_sentProcID = KS_GETVAR;

  // todo check lenght r same, and check if it is greater than 2
  pathLen = pobj->v_pathHost.veclen;
  ov_string_setvalue(&pobj->v_pathHost.value[pathLen - 1], pobj->v_serverHost);
  ov_string_setvalue(&pobj->v_pathName.value[pathLen - 1], pobj->v_serverName);

  OV_STRING_VEC* a[3] = {&pobj->v_pathHost, &pobj->v_pathName,
                         &pobj->v_pathInstance};
  for(OV_UINT i = 0; i < 3; i++) {
    for(OV_UINT j = 0; j < pathLen; j++) {
      if(!a[i]->value[j]) {
        ov_logfile_debug("ksmsg_ksmsg_getVar: value is NULL");
        pobj->v_state = KSBASE_CLST_ERROR;
        return OV_ERR_BADVALUE;
      }
    }
  }
  ov_memstack_lock();

  if(ksmsg_msgClient_msgsInQueue_get(pobj) >= pobj->v_queueLength)
    return OV_ERR_NOACCESS;

  result = PostSys_createAnonymousMessage(pobj, "Message",
                                          (OV_INSTPTR_ov_object*)(&pMsg));
  if(Ov_Fail(result)) {
    ov_logfile_error("Couldn't create Object 'Message' Reason: %s",
                     ov_result_getresulttext(result));
    ov_memstack_unlock();
    return OV_ERR_OK;
  }

  OV_STRING_VEC tmpStrVec = {0};
  Ov_SetDynamicVectorLength(&tmpStrVec, pathLen, STRING);

  /*setting Host path */
  result =
      PostSys_Message_pathAddress_set(pMsg, pobj->v_pathHost.value, pathLen);
  if(Ov_Fail(result)) {
    Ov_DeleteObject(pMsg);
    ov_memstack_unlock();
    return result;
  }

  /* setting Servername path */
  result = PostSys_Message_pathName_set(pMsg, pobj->v_pathName.value, pathLen);
  if(Ov_Fail(result)) {
    Ov_DeleteObject(pMsg);
    ov_memstack_unlock();
    return result;
  }
  /*setting Inst path */
  result = PostSys_Message_pathComponent_set(pMsg, pobj->v_pathInstance.value,
                                             pathLen);
  if(Ov_Fail(result)) {
    Ov_DeleteObject(pMsg);
    ov_memstack_unlock();
    return result;
  }

  pMsg->v_sendBy = MSG_SEND_KSSETVAR; /*	send via ks-setvar	*/

  /*	generic part set, now build the body	*/
  OV_STRING*    empty_vec = Ov_HeapMalloc(items_length * sizeof(OV_STRING));
  OV_STRING_VEC ids = {items_length, paths};

  OV_STRING_VEC values = {items_length, empty_vec};
  OV_STRING_VEC types = {items_length, empty_vec};
  OV_STRING_VEC units = {items_length, empty_vec};
  msgBody = acplt_simpleMsg_GenerateFlatBody(
      "ProcessControl", "GET", FALSE, NULL, &ids, &values, &units, &types);
  Ov_HeapFree(empty_vec);
  if(!msgBody) {
    Ov_DeleteObject(pMsg);
    ov_memstack_unlock();
    return OV_ERR_HEAPOUTOFMEMORY;
  }

  //    ov_logfile_debug("length: %lu\nmessageBody:\n%s\n", tempctr, msgBody);

  result = PostSys_Message_msgBody_set(pMsg, msgBody);
  if(Ov_Fail(result)) {
    Ov_DeleteObject(pMsg);
    ov_memstack_unlock();
    return result;
  }
  pobj->v_msgsInQueue++;
  ov_memstack_unlock();

  pobj->v_actimode = 1;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksmsg_msgClient_processGetVar(
    const OV_INSTPTR_ksbase_ClientBase this,
    const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
    OV_RESULT* result, OV_UINT* items_len, OV_GETVAR_ITEM** items_val) {
  /*
   *   local variables
   */
  OV_INSTPTR_ksmsg_msgClient thisCl = Ov_StaticPtrCast(ksmsg_msgClient, this);
  OV_RESULT                  fncresult = OV_ERR_OK;

  *items_val = thisCl->v_answerItems;
  *result = 0;
  *items_len = thisCl->v_answerItemsLen;
  return fncresult;
}
/*******************************************************************************************************************************************************************************
 * 				setVar
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksmsg_msgClient_requestSetVar(
    const OV_INSTPTR_ksbase_ClientBase this,
    const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
    const OV_UINT items_length, const OV_SETVAR_ITEM* items_val,
    const OV_INSTPTR_ov_domain callbackThat,
    void (*callback)(const OV_INSTPTR_ov_domain this,
                     const OV_INSTPTR_ov_domain that)) {
  /*
   *   local variables
   */
  OV_RESULT                  result = OV_ERR_OK;
  OV_INSTPTR_ksmsg_msgClient thisCl = Ov_StaticPtrCast(ksmsg_msgClient, this);

  if((thisCl->v_state != KSBASE_CLST_COMPLETED) &&
     (thisCl->v_state != KSBASE_CLST_INITIAL))
    return OV_ERR_NOACCESS;

  /*	set callback function	*/
  thisCl->v_callback.instanceCalled = callbackThat;
  thisCl->v_callback.callbackFunction = callback;

  ov_logfile_debug("ksmsg_ksmsg_getVar: requestSetVar");

  // TODO: zzz: give ID of messge to Client.  Mo 10 Dez 2018 17:50:30 CET

  thisCl->v_actimode = 1;

  /*	set ProcID	*/
  thisCl->v_sentProcID = KS_SETVAR;

  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksmsg_msgClient_processSetVar(
    const OV_INSTPTR_ksbase_ClientBase this,
    const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
    OV_RESULT* result, OV_UINT* items_length, OV_RESULT** items_results) {
  /*
   *   local variables
   */
  OV_INSTPTR_ksmsg_msgClient thisCl = Ov_StaticPtrCast(ksmsg_msgClient, this);
  OV_RESULT                  fncresult = OV_ERR_OK;
  // TODO: zzz: in typemethod directly change the values Di 11 Dez 2018 12:53:36
  // CET
  OV_UINT len = thisCl->v_answerItemsLen;
  *result = 0;
  *items_length = len;
  *items_results = ov_memstack_alloc(len * thisCl->v_answerItemsLen);
  for(OV_UINT i = 0; i < len; ++i) {
    *items_results[i] = thisCl->v_answerItems->result;
  }
  return fncresult;
}

