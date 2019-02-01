
/******************************************************************************
 *
 *   FILE
 *   ----
 *   reqSender.c
 *
 *   History
 *   -------
 *   2018-12-28   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "PostSys.h"
#include "PostSys_helper.h"
#include "ksbase_helper.h"
#include "lbalance.h"
#include "lbalance_helper.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"

OV_DLLFNCEXPORT OV_RESULT
                lbalance_reqSender_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT                     result = OV_ERR_OK;
  OV_INSTPTR_lbalance_reqSender pinst =
      Ov_StaticPtrCast(lbalance_reqSender, pobj);

  result = fb_functionblock_constructor(pobj);
  result |= Ov_Link(fb_tasklist, pinst, &pinst->p_sender);
  pinst->p_sender.v_actimode = 1;
  pinst->p_sender.v_iexreq = 1;

  return result;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_reqSender_reset_set(
    OV_INSTPTR_lbalance_reqSender pinst, const OV_BOOL value) {
  OV_RESULT result = OV_ERR_OK;
  if(value && !pinst->v_reset) {
    pinst->v_status = 0;
    pinst->v_result = 0;
  }
  pinst->v_reset = value;
  return result;
}

OV_DLLFNCEXPORT OV_UINT
                lbalance_reqSender_outLoad_get(OV_INSTPTR_lbalance_reqSender pobj) {
  return pobj->v_outLoad;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_reqSender_outLoad_set(
    OV_INSTPTR_lbalance_reqSender pobj, const OV_UINT value) {
  pobj->v_outLoad = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_reqSender_R_set(
    OV_INSTPTR_lbalance_reqSender pobj, const OV_BOOL value) {
  pobj->v_R = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void
lbalance_reqSender_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_RESULT                     result = OV_ERR_OK;
  OV_STRING                     dstKS = NULL;
  OV_INSTPTR_lbalance_reqSender pinst =
      Ov_StaticPtrCast(lbalance_reqSender, pfb);

  OV_INSTPTR_PostSys_msgCreator psender =
      Ov_DynamicPtrCast(PostSys_msgCreator, &pinst->p_sender);
  OV_STRING order = NULL;
  OV_STRING myIP = NULL;

  OV_ANY servername = {0};
  servername.value.vartype = OV_VT_VOID;
  servername.value.valueunion.val_string = NULL;
  ov_vendortree_getservername(&servername, NULL);

  switch(pinst->v_status) {
    case LB_REQSENDER_INIT:
      if(pinst->v_R) {
        /* pinst->v_sysInfo */
        if(!pinst->v_outApp || !pinst->v_destination) {
          ov_logfile_warning(
              "lbalance_reqSender: outApp or v_outRequirements  or "
              "v_destination NULL in this Round");
          pinst->v_status = LB_REQSENDER_SENT;
          return;
        }
        ov_memstack_lock();
        OV_STRING targetHost = NULL;
        OV_STRING targetServer = NULL;
        OV_STRING targetPath = NULL;
        OV_STRING targetHostPort = NULL;
        OV_STRING targetServerPort = NULL;
        ks_splitOneStringPath(pinst->v_destination, &targetHost,
                              &targetHostPort, &targetServer, &targetServerPort,
                              &targetPath);
        /* if(!targetPath) { */
        /* ov_logfile_error("No path is given in targetKS"); */
        /* pinst->v_status = LB_INTERNALERROR; */
        /* ov_memstack_unlock(); */
        /* return; */
        /* } */

        if(!targetHost || !targetServer) {
          ov_logfile_error("No targetHost or server is given in targetKS %s",
                           pinst->v_destination);
          pinst->v_status = LB_INTERNALERROR;
          ov_memstack_unlock();
          return;
        }

        /* create neighbor ks*/
        if(targetHostPort) {
          result |=
              ov_string_print(&dstKS, "//%s:%d/%s%s", targetHost,
                              targetHostPort, targetServer, LB_REQREC_PATH);
        } else {
          result |= ov_string_print(&dstKS, "//%s/%s%s", targetHost,
                                    targetServer, LB_REQREC_PATH);
        }
        /* send to neighbor */
        result |= PostSys_msgCreator_dst_set(psender, dstKS);
        result |= ov_string_append(&order, "a;a;");
        /* creating myIP */
        if(!pinst->v_selfHost || !servername.value.valueunion.val_string) {
          ov_logfile_error("lbalance_reqSender: selfHost | server = NULL");
          ov_memstack_unlock();
          return;
        }
        ov_string_print(&myIP, "//%s/%s", pinst->v_selfHost,
                        servername.value.valueunion.val_string);
        /* result |= ov_string_append(&order, ";"); */
        cJSON* jsMsg = cJSON_CreateArray();
        /* order is important. see helper */
        cJSON_AddItemToArray(jsMsg, cJSON_CreateString(myIP));
        cJSON_AddItemToArray(jsMsg, cJSON_CreateNumber(pinst->v_outLoad));

        OV_STRING tmpStr = cJSON_Print(jsMsg);
        cJSON_Delete(jsMsg);
        result |= ov_string_append(&order, tmpStr);
        ov_logfile_debug("lbalance_reqSender: to: %s; %s", dstKS, tmpStr);
        free(tmpStr);
        result |= PostSys_msgCreator_order_set(psender, order);
        if(Ov_Fail(result)) {
          ov_logfile_error("%u: %s: ", result, ov_result_getresulttext(result));
          pinst->v_status = LB_INTERNALERROR;
          ov_memstack_unlock();
          return;
        }
        ov_memstack_unlock();
        pinst->v_status = LB_REQSENDER_SENT;
      }
      break;
    case LB_REQSENDER_SENT:
      if(!pinst->v_R) {
        pinst->v_status = LB_REQSENDER_INIT;
        ov_string_setvalue(&pinst->v_outApp, NULL);
        pinst->v_outLoad = 0;
      }
      break;

      LB_CASEERROR
  }

  return;
}

