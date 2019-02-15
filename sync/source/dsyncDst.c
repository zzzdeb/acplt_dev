/******************************************************************************
 *
 *   FILE
 *   ----
 *   dsyncDst.c
 *
 *   History
 *   -------
 *   Mi 28 Nov 2018 22:05:18 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_sync
#define OV_COMPILE_LIBRARY_sync
#endif

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include "object_helper.h"
#include "ovhelper.h"
#include "sync.h"
#include "sync_helper.h"

#include "ksapi.h"
#include "ksbase_helper.h"

OV_DLLFNCEXPORT OV_RESULT sync_dsyncDst_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT result = OV_ERR_OK;
  /* OV_INSTPTR_sync_dsyncDst pinst = Ov_StaticPtrCast(sync_dsyncDst, pobj); */

  result = ksbase_ComTask_constructor(pobj);

  return result;
}

OV_DLLFNCEXPORT OV_RESULT
                sync_dsyncDst_dstKS_set(OV_INSTPTR_sync_dsyncDst pinst, const OV_STRING value) {
  OV_RESULT result = OV_ERR_OK;
  if(!value) {
    result = ov_string_setvalue(&pinst->v_dstKS, NULL);
    return result;
  }
  OV_STRING host = NULL;
  OV_STRING serv = NULL;
  OV_STRING path = NULL;
  OV_STRING hostPort = NULL;
  OV_STRING servPort = NULL;
  ov_memstack_lock();
  ks_splitOneStringPath(value, &host, &hostPort, &serv, &servPort, &path);
  result = ov_string_setvalue(&pinst->v_dstKS, value);
  result |= ov_string_setvalue(&pinst->v_path, path);
  result |= ov_string_setvalue(&pinst->p_syncDownload.v_path, path);
  ov_memstack_unlock();
  return result;
}

OV_DLLFNCEXPORT OV_RESULT
                sync_dsyncDst_srcKS_set(OV_INSTPTR_sync_dsyncDst pinst, const OV_STRING value) {
  OV_RESULT result = OV_ERR_OK;
  if(!value) {
    result = ov_string_setvalue(&pinst->v_srcKS, NULL);
    return result;
  }
  OV_STRING host = NULL;
  OV_STRING serv = NULL;
  OV_STRING path = NULL;
  OV_STRING hostPort = NULL;
  OV_STRING servPort = NULL;
  ov_memstack_lock();
  ks_splitOneStringPath(value, &host, &hostPort, &serv, &servPort, &path);
  result = ov_string_setvalue(&pinst->v_srcKS, value);
  ov_memstack_unlock();
  return result;
}

OV_DLLFNCEXPORT OV_RESULT
                sync_dsyncDst_switch_set(OV_INSTPTR_sync_dsyncDst pobj, const OV_BOOL value) {
  OV_RESULT result = OV_ERR_OK;
  OV_STRING srcHost = NULL;
  OV_STRING srcHostPort = NULL;
  OV_STRING srcName = NULL;
  OV_STRING srcNamePort = NULL;
  OV_STRING srcPath = NULL;

  if(!pobj->v_switch && value) {
    OV_INSTPTR_ov_object proot = ov_path_getobjectpointer(pobj->v_path, 2);
    if(!proot) {
      ov_logfile_error("sync_dsyncDst: root couldt be found: %s", pobj->v_path);
      pobj->v_result = OV_ERR_GENERIC;
      return 0;
    }
    const OV_STRING classesToConfiugure[] = {[KSAPISET] = "ksapi/setVar",
                                             [KSAPIGET] = "ksapi/getVar"};
    /*
     * refresh nodes
     */
    for(OV_UINT i = 0; i < SYNC_CONFIGURE_LEN; ++i) {
      if(i != KSAPISET && i != KSAPIGET) {
        ov_logfile_warning("sync_dsync: cant handle %d", i);
        continue;
      }
      OV_INSTPTR_ov_class pssc = ov_class_search(classesToConfiugure[i]);
      if(pssc) {
        OV_INSTPTR_ov_object pobj = NULL;
        Ov_ForEachChild(ov_instantiation, pssc, pobj) {
          /* checking if right one */
          if(object_isDescendant(proot, Ov_StaticPtrCast(ov_object, pobj))) {
            if(pobj->v_pouterobject) {
              if(Ov_CanCastTo(fbcomlib_setVar, pobj->v_pouterobject)) {
                OV_INSTPTR_fbcomlib_setVar pOobj =
                    Ov_StaticPtrCast(fbcomlib_setVar, pobj->v_pouterobject);
                pOobj->v_doReset = 0;
                fbcomlib_setVar_doReset_set(pOobj, 1);
              } else if(Ov_CanCastTo(fbcomlib_getVar, pobj->v_pouterobject)) {
                OV_INSTPTR_fbcomlib_getVar pOobj =
                    Ov_StaticPtrCast(fbcomlib_getVar, pobj->v_pouterobject);
                pOobj->v_doReset = 0;
                fbcomlib_getVar_doReset_set(pOobj, 1);
              } else {
                ov_logfile_warning(
                    "sync_dsync: ksapi not in fbcomlib %s",
                    ov_path_getcanonicalpath(
                        Ov_StaticPtrCast(ov_object, pobj->v_pouterobject), 2));
              }
            } else {
              Ov_StaticPtrCast(ksapi_KSApiCommon, pobj)->v_Reset = 0;
              ksapi_KSApiCommon_Reset_set(
                  Ov_StaticPtrCast(ksapi_KSApiCommon, pobj), 1);
            }
          }
        }
      }
    }

    /*
     * shut down sync on src site
     */
    ov_logfile_info("dsync shutdown on src site.");
    if(!pobj->v_srcKS) {
      ov_logfile_error("sync_dsyncDst: srcKS is Null");
      return OV_ERR_BADPARAM;
    }
    ks_splitOneStringPath(pobj->v_srcKS, &srcHost, &srcHostPort, &srcName,
                          &srcNamePort, &srcPath);
    ov_logfile_info("sync_dsyncDst: requesting shutdown to %s", pobj->v_srcKS);
    ov_string_print(&pobj->p_kssetter.v_path, "%s.shutdown", pobj->v_syncPath);
    ov_string_setvalue(&pobj->p_kssetter.v_serverHost, srcHost);
    ov_string_setvalue(&pobj->p_kssetter.v_serverName, srcName);
    pobj->p_kssetter.v_varValue.value.vartype = OV_VT_BOOL;
    pobj->p_kssetter.v_varValue.value.valueunion.val_bool = 1;
    ksapi_setVar_submit(Ov_StaticPtrCast(ksapi_KSApiCommon, &pobj->p_kssetter));
  }
  pobj->v_status = DSYNC_DST_DONE;
  return result;
}

OV_RESULT sync_dsyncDst_opTerm(OV_INSTPTR_sync_dsyncDst pinst, OV_RESULT res) {
  pinst->v_result = res;
  OV_RESULT result = OV_ERR_OK;
  pinst->p_syncDownload.v_actimode = 0;
  pinst->v_actimode = 0;
  // TODO: zzz: notify src site :2019 Jan 18 18:58
  // TODO: zzz: delete image :2019 Jan 18 18:58
  pinst->v_status = SYNC_INTERNALERROR;
  return result;
}

OV_DLLFNCEXPORT void sync_dsyncDst_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  ov_memstack_lock();
  OV_INSTPTR_sync_dsyncDst pinst = Ov_StaticPtrCast(sync_dsyncDst, this);
  switch(pinst->v_status) {
    case DSYNC_DST_INIT:
      break;
    case SYNC_INTERNALERROR:
    case SYNC_EXTERNALERROR:
      pinst->v_actimode = 0;
      break;
    case DSYNC_DST_ACTIVE:
      if(pinst->p_syncDownload.v_state == SYNC_DOWNLOAD_DONE) {
        if(Ov_Fail(pinst->p_syncDownload.v_result)) {
          sync_dsyncDst_opTerm(pinst, pinst->p_syncDownload.v_result);
        } else {
          pinst->v_switch = 0;
          sync_dsyncDst_switch_set(pinst, 1);
        }
      }
      break;
    case DSYNC_DST_DONE:
      pinst->v_actimode = 0;
      ov_logfile_info("sync_dsyncDst: done.");
      ov_memstack_unlock();
      break;
    default:
      ov_logfile_error("sync_dsyncDst: unknown state");
      ov_memstack_unlock();
      return;
  }
  ov_memstack_unlock();
  return;
}

