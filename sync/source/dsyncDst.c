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
#include "sync.h"

#include "ksapi.h"
#include "ksbase_helper.h"

OV_DLLFNCEXPORT OV_RESULT
                sync_dsyncDst_switch_set(OV_INSTPTR_sync_dsyncDst pobj, const OV_BOOL value) {
  OV_STRING srcHost = NULL;
  OV_STRING srcHostPort = NULL;
  OV_STRING srcName = NULL;
  OV_STRING srcNamePort = NULL;
  OV_STRING srcPath = NULL;

  if(value) {
    if(!pobj->v_srcKS) {
      ov_logfile_error("sync_dsyncDst: srcKS is Null");
      return OV_ERR_BADPARAM;
    }
    ks_splitOneStringPath(pobj->v_srcKS, &srcHost, &srcHostPort, &srcName,
                          &srcNamePort, &srcPath);
    // FIX: zzz: check parameters Mo 03 Dez 2018 23:54:45 CET

    ov_string_print(&pobj->p_kssetter.v_path, "%s.shutdown", srcPath);
    ov_string_setvalue(&pobj->p_kssetter.v_serverHost, srcHost);
    ov_string_setvalue(&pobj->p_kssetter.v_serverName, srcName);
    pobj->p_kssetter.v_varValue.value.vartype = OV_VT_BOOL;
    pobj->p_kssetter.v_varValue.value.valueunion.val_bool = 1;
    ksapi_setVar_submit(&pobj->p_kssetter);
  }
  return 0;
}

OV_DLLFNCEXPORT void sync_dsyncDst_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                              OV_TIME* pltc) {
  /*
   *   local variables
   */
  /* OV_INSTPTR_sync_dsyncDst pinst = Ov_StaticPtrCast(sync_dsyncDst, pfb); */

  return;
}

