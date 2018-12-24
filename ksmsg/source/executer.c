
/******************************************************************************
 *
 *   FILE
 *   ----
 *   executer.c
 *
 *   History
 *   -------
 *   Sa 01 Dez 2018 11:29:06 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_ksmsg
#define OV_COMPILE_LIBRARY_ksmsg
#endif

#include "ksmsg.h"

#include "libov/ov_macros.h"
#include "libov/ov_memstack.h"

#include "ksbase.h"
#include "ov_ksserver_backend.h"

#include "CTree_helper.h"
#include "NoneTicketAuthenticator.h"
#include "acplt_simpleMsgHandling.h"
#include "cJSON.h"

OV_DLLFNCEXPORT OV_RESULT
                ksmsg_executer_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT result = OV_ERR_OK;
  OV_INSTPTR_ksmsg_executer this = Ov_StaticPtrCast(ksmsg_executer, pobj);
  OV_INSTPTR_ov_domain pinbox = NULL;

  result = ksbase_ComTask_constructor(pobj);

  result |= Ov_CreateObject(PostSys_inbox, pinbox, this, "inbox");

  return result;
}

OV_DLLFNCEXPORT void ksmsg_executer_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  OV_RESULT                 result = OV_ERR_OK;
  OV_INSTPTR_ksmsg_executer pinst = Ov_StaticPtrCast(ksmsg_executer, this);

  OV_INSTPTR_PostSys_Message pMsg = NULL;
  OV_INSTPTR_ov_domain       pinbox =
      Ov_SearchChildEx(ov_containment, pinst, "inbox", ov_domain);
  if(!pinbox) {
    ov_logfile_error("no inbox for executer");
    return;
  }

  Ov_ForEachChildEx(ov_containment, pinbox, pMsg, PostSys_Message) { break; }
  if(!pMsg)
    return;
  ov_logfile_info("postExec: %s", pMsg->v_msgBody);
  OV_STRING     srv = NULL;
  OV_STRING     op = NULL;
  OV_STRING     contName = NULL;
  OV_STRING_VEC ids = {0};
  OV_STRING_VEC values = {0};
  OV_STRING_VEC units = {0};
  OV_STRING_VEC types = {0};

  // TODO: zzz: work on it Sa 01 Dez 2018 18:30:29 CET
  acplt_simpleMsg_parseFlatBody(pMsg->v_msgBody, &srv, &op, 0, &contName, &ids,
                                &values, &units, &types);
  /* creating res msg*/
  OV_INSTPTR_PostSys_Message     pMsgRes = NULL;
  OV_INSTPTR_PostSys_MsgDelivery pMsgDelivery = NULL;
  OV_STRING                      msgBody = NULL;
  OV_UINT                        pathLen = 0;

  result = Ov_CreateIDedObject(PostSys_Message, pMsgRes, pinst, "Message");
  PostSys_Message_pathLen_set(pMsgRes, pMsg->v_pathLen);
  pathLen = pMsg->v_pathAddress.veclen;
  OV_STRING_VEC* a[3] = {&pMsg->v_pathAddress, &pMsg->v_pathName,
                         &pMsg->v_pathComponent};
  OV_STRING_VEC* b[3] = {&pMsgRes->v_pathAddress, &pMsgRes->v_pathName,
                         &pMsgRes->v_pathComponent};
  for(OV_UINT i = 0; i < 3; i++) {
    for(OV_UINT j = 0; j < pathLen; j++) {
      ov_string_setvalue(&b[i]->value[j], a[i]->value[pathLen - 1 - j]);
    }
  }
  /* beginning */
  if(ov_string_compare(op, "GET") == OV_STRCMP_EQUAL) {
    ov_logfile_info("ksmsg_executer: called GET");
    OV_GETVAR_PAR params = {0};
    ov_memstack_lock();
    params.identifiers_len = ids.veclen;
    params.identifiers_val = ids.value;
    OV_GETVAR_RES results = {0};
    OV_TICKET*    pticket = NULL;

    pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

    ov_ksserver_getvar(2, pticket, &params, &results);

    /*	delete Ticket	*/
    pticket->vtbl->deleteticket(pticket);

    OV_UINT       len = params.identifiers_len;
    OV_STRING*    emptyStr = ov_memstack_alloc(sizeof(OV_STRING) * len);
    OV_STRING*    valStr = ov_memstack_alloc(sizeof(OV_STRING) * len);
    OV_STRING_VEC resIds = {len, emptyStr};
    OV_STRING_VEC resUnits = {len, emptyStr};
    OV_STRING_VEC resTypes = {len, emptyStr};
    OV_STRING_VEC resVals = {len, valStr};
    for(OV_UINT i = 0; i < len; ++i) {
      if(results.items_val[i].result) {
        ov_string_print(&resVals.value[i], "%d", results.items_val[i].result);
      } else {
        cJSON* pjsvalue = cJSON_CreateArray();
        result = valueToJSON(
            &pjsvalue, results.items_val[i].var_current_props.value.vartype,
            &(results.items_val[i].var_current_props.value.valueunion.val_byte),
            0);
        OV_STRING valstr = cJSON_Print(pjsvalue);
        ov_logfile_info("ksmsg_executer: %s", valstr);
        ov_string_setvalue(&resVals.value[i], valstr);
        free(valstr);
      }
    }

    msgBody = acplt_simpleMsg_GenerateFlatBody(
        NULL, "RES:GET", 0, NULL, &resIds, &resVals, &resUnits, &resTypes);
    result = PostSys_Message_msgBody_set(pMsgRes, msgBody);
    if(Ov_Fail(result)) {
      Ov_DeleteObject(pMsgRes);
      ov_memstack_unlock();
      return;
    }
    pMsgDelivery = Ov_StaticPtrCast(
        PostSys_MsgDelivery,
        Ov_GetFirstChild(ov_instantiation, pclass_PostSys_MsgDelivery));
    if(pMsgDelivery) {
      Ov_Link(PostSys_MsgDelivery2Message, pMsgDelivery, pMsgRes);
    }

    ov_memstack_unlock();
    Ov_DeleteObject(pMsg);
  } else if(ov_string_compare(op, "SET") == OV_STRCMP_EQUAL) {
    ov_logfile_info("ksmsg_executer: called SET");
    OV_SETVAR_RES results = {0};
    OV_SETVAR_PAR params = {0};
    ov_memstack_lock();
    params.items_len = ids.veclen;
    OV_UINT len = params.items_len;
    params.items_val = ov_memstack_alloc(len * sizeof(OV_SETVAR_ITEM));
    for(OV_UINT i = 0; i < len; ++i) {
      ov_string_setvalue(&params.items_val[i].path_and_name, ids.value[i]);
      cJSON* jsval = cJSON_Parse(values.value[i]);
      if(!jsval || !cJSON_IsArray(jsval)) {
        ov_logfile_error("ksmsg_executer: %s malformed json val",
                         values.value[i]);
      }
      jsonToVarvalue(&params.items_val[i].var_current_props.value, jsval);
    }

    OV_TICKET* pticket = NULL;

    pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

    ov_ksserver_setvar(2, pticket, &params, &results);

    /*	delete Ticket	*/
    pticket->vtbl->deleteticket(pticket);

    OV_STRING* emptyStr = ov_memstack_alloc(sizeof(OV_STRING) * len);
    for(OV_UINT i = 0; i < len; ++i) {
      emptyStr[i] = NULL;
    }

    OV_STRING*    valStr = ov_memstack_alloc(sizeof(OV_STRING) * len);
    OV_STRING_VEC resIds = {len, emptyStr};
    OV_STRING_VEC resUnits = {len, emptyStr};
    OV_STRING_VEC resTypes = {len, emptyStr};
    OV_STRING_VEC resVals = {len, valStr};
    for(OV_UINT i = 0; i < len; ++i) {
      ov_string_print(&resVals.value[i], "%d", results.results_val[i]);
    }
    ov_memstack_unlock();
    return;
  }

