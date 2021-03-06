/******************************************************************************
 *
 *   FILE
 *   ----
 *   dsync.c
 *
 *   History
 *   -------
 *   2018-08-02   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_sync
#define OV_COMPILE_LIBRARY_sync
#endif

#include "CTree.h"
#include "CTree_helper.h"
#include "ksbase_helper.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include "object_helper.h"
#include "sync.h"

#include "sync_helper.h"

#include "ksapi_commonFuncs.h"

#define DEBUGSTATE 16

OV_DLLFNCEXPORT OV_RESULT sync_dsync_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT             result = OV_ERR_OK;
  OV_INSTPTR_sync_dsync pinst = Ov_StaticPtrCast(sync_dsync, pobj);

  result = fb_functionblock_constructor(pobj);
  pinst->p_dsyncDstTemp.v_actimode = 0;

  ov_memstack_lock();
  OV_ANY servername = {0};
  servername.value.vartype = OV_VT_VOID;
  servername.value.valueunion.val_string = NULL;
  result |= ov_vendortree_getservername(&servername, NULL);

  result |= ov_string_setvalue(&pinst->v_selfServer,
                               servername.value.valueunion.val_string);
  ov_string_setvalue(&servername.value.valueunion.val_string, NULL);
  result |= Ov_Link(fb_tasklist, pinst, &pinst->p_transport);
  ov_memstack_unlock();
  return result;
}

OV_DLLFNCEXPORT OV_RESULT sync_dsync_trigger_set(OV_INSTPTR_sync_dsync pinst,
                                                 const OV_BOOL         value) {
  if(value && !pinst->v_trigger && pinst->v_EN) {
    pinst->v_iexreq = 1;
    pinst->v_actimode = 1;
    pinst->v_status = SYNC_SRC_ACTIVE;
  }
  pinst->v_trigger = value;
  return 0;
}

OV_DLLFNCEXPORT OV_RESULT sync_dsync_reset_set(OV_INSTPTR_sync_dsync pobj,
                                               const OV_BOOL         value) {
  if(value && !pobj->v_reset) {
    pobj->v_status = SYNC_SRC_INIT;
    ksapi_KSApiCommon_Reset_set(
        Ov_StaticPtrCast(ksapi_KSApiCommon, &pobj->p_kssetter), 0);
    ksapi_KSApiCommon_Reset_set(
        Ov_StaticPtrCast(ksapi_KSApiCommon, &pobj->p_kssetter), 1);
    ksapi_KSApiCommon_Reset_set(
        Ov_StaticPtrCast(ksapi_KSApiCommon, &pobj->p_ksCrtObj), 0);
    ksapi_KSApiCommon_Reset_set(
        Ov_StaticPtrCast(ksapi_KSApiCommon, &pobj->p_ksCrtObj), 1);
    ov_string_setvalue(&pobj->p_transport.v_targetDownloadPath,
                       "data/CTree/Download");
    CTree_Transport_reset_set(&pobj->p_transport, 0);
    CTree_Transport_reset_set(&pobj->p_transport, 1);
  }
  pobj->v_reset = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT sync_dsync_shutdown_set(OV_INSTPTR_sync_dsync pobj,
                                                  const OV_BOOL         value) {
  OV_RESULT result = OV_ERR_OK;
  pobj->v_reset = value;

  ov_logfile_info("dsync shutdown on src site.");
  if(value && !pobj->v_shutdown) {
    /*check if request expected*/
    if(pobj->v_status != SYNC_SRC_WAITINGFORSHUTDOWN &&
       pobj->v_status != SYNC_SRC_TRANSPORTREQUESTED) {
      ov_logfile_error(
          "sync_dsync: shutdown requested, where no request expected");
      return OV_ERR_BADVALUE;
    }
    /*go on */
    OV_INSTPTR_ov_object proot = ov_path_getobjectpointer(pobj->v_srcPath, 2);
    if(!proot) {
      ov_logfile_error("sync_dsync: root coudnt be found %s", pobj->v_srcPath);
      pobj->v_status = SYNC_INTERNALERROR;
      pobj->v_result = OV_ERR_GENERIC;
      return OV_ERR_GENERIC;
    }
    if(!pobj->v_copy) {
      result |= Ov_DeleteObject(ov_path_getobjectpointer(pobj->v_srcPath, 2));
      return result;
    }
  }
  pobj->v_trigger = 0;
  pobj->v_status = SYNC_SRC_INIT;
  ov_logfile_info("dsync shutdown on src site.");
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void sync_dsync_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                           OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_dsync pinst = Ov_StaticPtrCast(sync_dsync, pfb);

  OV_RESULT result = OV_ERR_OK;

  OV_STRING                  dsyncDstPath = "/data/dsyncDst";
  OV_STRING                  dstName = NULL;
  OV_STRING                  dstHost = NULL;
  OV_INSTPTR_CTree_Transport ptrans =
      Ov_StaticPtrCast(CTree_Transport, &pinst->p_transport);
  OV_INSTPTR_sync_dsyncDst pdsyncDst = &pinst->p_dsyncDstTemp;
  OV_INSTPTR_ov_object     proot = NULL;

  ov_memstack_lock();
  /* Getting Servername  */
  OV_ANY servername = {0};
  servername.value.vartype = OV_VT_VOID;
  servername.value.valueunion.val_string = NULL;
  ov_vendortree_getservername(&servername, NULL);

  switch(pinst->v_status) {
    case SYNC_SRC_INIT:
      break;
    case SYNC_SRC_ACTIVE:
      if(!pinst->v_EN) {
        ov_logfile_warning("sync_dsync: not enabled");
        ov_memstack_unlock();
        return;
      }
      /* check */
      proot = ov_path_getobjectpointer(pinst->v_srcPath, 2);
      if(!proot) {
        ov_logfile_error("dsync_sync: root couldt be found %s",
                         pinst->v_srcPath);
        pinst->v_status = SYNC_INTERNALERROR;
        ov_memstack_unlock();
        return;
      } /* configure dest server */
      pinst->p_ksCrtObj.v_path = NULL;
      ks_splitOneStringPath(pinst->v_destKS, &dstHost, NULL, &dstName, NULL,
                            NULL);
      ov_string_print(&ptrans->v_targetKS, "//%s/%s%s", dstHost, dstName,
                      dsyncDstPath);

      OV_STRING srcKS = NULL;
      ov_string_print(&srcKS, "//%s/%s%s", pinst->v_selfHost,
                      servername.value.valueunion.val_string, pinst->v_srcPath,
                      2);
      ov_string_setvalue(
          &pdsyncDst->v_syncPath,
          ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pinst), 2));
      result = sync_dsyncDst_srcKS_set(pdsyncDst, srcKS);
      OV_STRING dstKS = NULL;
      result |=
          ov_string_print(&dstKS, "%s%s", pinst->v_destKS, pinst->v_srcPath);
      result |= sync_dsyncDst_dstKS_set(pdsyncDst, dstKS);
      result |= ov_string_setvalue(&dstKS, NULL);
      if(Ov_Fail(result)) {
        ov_logfile_error("%u: %s: srcKS set failed", result,
                         ov_result_getresulttext(result));
        pinst->v_status = SYNC_INTERNALERROR;
        ov_memstack_unlock();
        return;
      }
      ov_string_setvalue(&srcKS, NULL);

      ov_string_setvalue(&pdsyncDst->p_syncDownload.v_srcPath,
                         pinst->v_srcPath);
      ov_string_setvalue(
          &ptrans->v_path,
          ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pdsyncDst), 2));
      pdsyncDst->v_actimode = 1;
      pdsyncDst->v_status = DSYNC_DST_ACTIVE;
      ptrans->v_trigger = 0;
      CTree_Common_trigger_set(Ov_StaticPtrCast(CTree_Common, ptrans), 1);
      pdsyncDst->v_actimode = 0;
      pinst->v_status = SYNC_SRC_SYNCCREATEREQUESTED;
      ov_logfile_info("sync on destination requested");
      ov_memstack_unlock();
      return;
      break;
    case SYNC_SRC_SYNCCREATEREQUESTED:
      /* if(pkscrt->v_status & KSAPI_COMMON_EXTERNALERROR & */
      /*    KSAPI_COMMON_INTERNALERROR) { */
      /*   pinst->v_status = SYNC_SRC_ERROR; */
      /*   ov_logfile_error("Error at ksapi"); */
      /*   pinst->v_result = OV_ERR_GENERIC; */
      /*   return; */
      /* } else if(pkscrt->v_status & KSAPI_COMMON_WAITINGFORANSWER) { */
      /*   return; */
      /* } else if(pkscrt->v_status & KSAPI_COMMON_REQUESTCOMPLETED) { */
      /* configure setvar & getvar */
      if(ptrans->v_status &
         (CTREE_COMMON_EXTERNALERROR | CTREE_COMMON_EXTERNALERROR)) {
        ov_logfile_error("sync_dsync: transport failed");
        pinst->v_status = SYNC_INTERNALERROR;
        ov_memstack_unlock();
        return;
      } else if(ptrans->v_status != CTREE_TR_DONE) {
        ov_memstack_unlock();
        return;
      }
      result = ptrans->v_result;
      if(Ov_Fail(result)) {
        if(result != OV_ERR_ALREADYEXISTS) {
          ov_logfile_error("%u: %s: ksapi dsyncDst creation unsuccessful",
                           result, ov_result_getresulttext(result));
          pinst->v_result = result;
          ov_memstack_unlock();
          return;
        } else {
          ov_logfile_warning("%u: %s: dsyncDst", result,
                             ov_result_getresulttext(result));
        }
      }
      ov_logfile_info("dsyncDst created successfully. Transporting...");
      /* run transport */
      OV_INSTPTR_CTree_Transport ptrans =
          Ov_StaticPtrCast(CTree_Transport, &pinst->p_transport);
      ov_string_setvalue(&ptrans->v_path, pinst->v_srcPath);
      ov_string_print(&ptrans->v_targetKS, "%s%s", pinst->v_destKS,
                      pinst->v_srcPath);
      ov_string_print(&ptrans->v_targetDownloadPath, "%s%s", DSYNC_PATH_DEST,
                      DSYNC_DOWNLOAD_PATH_DEST_EXT);
      ptrans->v_trigger = 0;
      CTree_Common_trigger_set(Ov_StaticPtrCast(CTree_Common, ptrans), 1);
      pinst->v_status = SYNC_SRC_TRANSPORTREQUESTED;
      ov_logfile_info("transport requested");
      ov_memstack_unlock();
      return;
      break;
    case SYNC_SRC_TRANSPORTREQUESTED:
      switch(pinst->p_transport.v_status) {
        case CTREE_TR_DONE:
          ov_logfile_info("transport done successfully");
          ov_string_setvalue(&pinst->p_transport.v_targetDownloadPath,
                             NULL);
          pinst->v_status = SYNC_SRC_WAITINGFORSHUTDOWN;
          break;
        case CTREE_TR_TREESENT_WAITING:
        case CTREE_TR_LIBSSENT_WAITING:
          break;
        case CTREE_TR_INIT:
        case CTREE_COMMON_EXTERNALERROR:
        case CTREE_COMMON_INTERNALERROR:
        default:
          ov_string_setvalue(&pinst->p_transport.v_targetDownloadPath,
                             "data/CTree/Download");
          ov_logfile_error("sync failed at transport");
          pinst->v_status = SYNC_INTERNALERROR;
          break;
      }
      break;
    case SYNC_SRC_WAITINGFORSHUTDOWN:
      break;
    case SYNC_INTERNALERROR:
    case SYNC_EXTERNALERROR:
      pinst->v_actimode = 0;
      break;
    default:
      ov_logfile_error("sync_dsync: unknown status %d", pinst->v_status);
      ov_memstack_unlock();
      return;
  }
  /* wait for command */
  ov_memstack_unlock();
  return;
}
