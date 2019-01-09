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
#include "ksmsg_helper.h"

OV_DLLFNCEXPORT OV_RESULT sync_dsyncDst_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT                result = OV_ERR_OK;
  OV_INSTPTR_sync_dsyncDst pinst = Ov_StaticPtrCast(sync_dsyncDst, pobj);

  result = ksbase_ComTask_constructor(pobj);

  pinst->p_player.v_actimode = 0;
  return result;
}

OV_DLLFNCEXPORT OV_RESULT
                sync_dsyncDst_dstKS_set(OV_INSTPTR_sync_dsyncDst pinst, const OV_STRING value) {
  OV_RESULT result = OV_ERR_OK;
  if(!value) {
    result = ov_string_setvalue(&pinst->v_dstKS, NULL);
    result |= ov_string_setvalue(&pinst->p_player.v_dstHost, NULL);
    result |= ov_string_setvalue(&pinst->p_player.v_dstServer, NULL);
    result |= ov_string_setvalue(&pinst->p_player.v_dstPath, NULL);
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
  result |= ov_string_setvalue(&pinst->p_player.v_dstHost, host);
  result |= ov_string_setvalue(&pinst->p_player.v_dstServer, serv);
  result |= ov_string_setvalue(&pinst->p_player.v_dstPath, path);
  ov_memstack_unlock();
  return result;
}

OV_DLLFNCEXPORT OV_RESULT
                sync_dsyncDst_srcKS_set(OV_INSTPTR_sync_dsyncDst pinst, const OV_STRING value) {
  OV_RESULT result = OV_ERR_OK;
  if(!value) {
    result = ov_string_setvalue(&pinst->v_srcKS, NULL);
    result |= ov_string_setvalue(&pinst->p_player.v_srcHost, NULL);
    result |= ov_string_setvalue(&pinst->p_player.v_srcServer, NULL);
    result |= ov_string_setvalue(&pinst->p_player.v_srcPath, NULL);
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
  result |= ov_string_setvalue(&pinst->p_player.v_srcHost, host);
  result |= ov_string_setvalue(&pinst->p_player.v_srcServer, serv);
  result |= ov_string_setvalue(&pinst->p_player.v_srcPath, path);
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
    if(!proot) { // todo badparam info
      ov_logfile_error("root couldt be found: %s", pobj->v_path);
      pobj->v_result = OV_ERR_GENERIC;
      return 0;
    }
    const OV_STRING classesToConfiugure[] = {[KSAPISET] = "ksapi/setVar",
                                             [KSAPIGET] = "ksapi/getVar"};
    /*
     * delete second element from path
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
            OV_INSTPTR_ksmsg_msgClient pMsgClnt = Ov_SearchChildEx(
                ov_containment, Ov_StaticPtrCast(ov_domain, pobj), "msgClient",
                ksmsg_msgClient);
            if(!pMsgClnt) {
              ov_logfile_warning("dsyncDst: %s has no msgClient",
                                 pobj->v_identifier);
              continue;
            }
            result = ksmsg_msgClient_path_deleteElement(pMsgClnt, 1);
            if(Ov_Fail(result)) {
              ov_logfile_error("%u: %s: failed to delete path element", result,
                               ov_result_getresulttext(result));
              return result;
            }
          }
        }
      }
    }

    // TODO: zzz: status :2018 Dez 20 12:56
    /*
     * stop buffering msgs
     */
    result = sync_player_switch_set(&pobj->p_player, 1);
    if(Ov_Fail(result)) {
      ov_logfile_error("%u: %s: ", result, ov_result_getresulttext(result));
      return result;
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
    // FIX: zzz: check parameters: Mo 03 Dez 2018 23:54:45 CET

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
  pinst->p_player.v_actimode = 0;
  pinst->p_syncDownload.v_actimode = 0;
  pinst->v_actimode = 0;
  pinst->v_status = DSYNC_DST_ERROR;
  return result;
}

OV_DLLFNCEXPORT void sync_dsyncDst_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_dsyncDst pinst = Ov_StaticPtrCast(sync_dsyncDst, this);
  switch(pinst->v_status) {
    case DSYNC_DST_INIT:
      break;
    case DSYNC_DST_ERROR:
      pinst->v_actimode = 0;
      break;
    case DSYNC_DST_ACTIVE:
      if(pinst->p_syncDownload.v_state == SYNC_DOWNLOAD_DONE) {
        if(Ov_Fail(pinst->p_syncDownload.v_result)) {
          sync_dsyncDst_opTerm(pinst, pinst->p_syncDownload.v_result);
        } else {
          pinst->p_player.v_actimode = 1;
          pinst->v_status = DSYNC_DST_WAITINGPLAYER;
        }
      }
      break;
    case DSYNC_DST_WAITINGPLAYER:
      break;
    case DSYNC_DST_DONE:
      pinst->v_actimode = 0;
      break;
    default:
      ov_logfile_error("sync_dsyncDst: unknown state");
      return;
  }
  return;
}

