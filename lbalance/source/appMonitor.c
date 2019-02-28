/******************************************************************************
 *
 *   FILE
 *   ----
 *   appMonitor.c
 *
 *   History
 *   -------
 *   Fr 28 Dez 2018 13:06:43 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include <string.h>

#include "lbalance.h"
#include "lbalance_helper.h"

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"

#include "object_helper.h"

OV_DLLFNCEXPORT OV_RESULT
                lbalance_appMonitor_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT                      result = OV_ERR_OK;
  OV_INSTPTR_lbalance_appMonitor pinst =
      Ov_StaticPtrCast(lbalance_appMonitor, pobj);

  result = fb_functionblock_constructor(pobj);

  OV_STRING paths[] = {"/TechUnits"};
  Ov_SetDynamicVectorValue(&pinst->v_appPaths, paths, 1, STRING);

  OV_STRING weights[] = {"GSE1&10", "GSE2&20","GSE3&30"};
  Ov_SetDynamicVectorValue(&pinst->v_weights, weights, 3, STRING);
  return result;
}

OV_DLLFNCEXPORT OV_UINT
                lbalance_appMonitor_sum_get(OV_INSTPTR_lbalance_appMonitor pinst) {
  OV_UINT sum = 0;
  for(OV_UINT i = 0; i < pinst->v_loads.veclen; ++i) {
    sum += pinst->v_loads.value[i];
  }
  return sum;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_appMonitor_reset_set(
    OV_INSTPTR_lbalance_appMonitor pinst, const OV_BOOL value) {
  OV_RESULT result = OV_ERR_OK;
  if(value && !pinst->v_reset) {
    pinst->v_status = 0;
    pinst->v_result = 0;
  }
  pinst->v_reset = value;
  return result;
}

OV_DLLFNCEXPORT void
lbalance_appMonitor_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_appMonitor pinst =
      Ov_StaticPtrCast(lbalance_appMonitor, pfb);

  OV_UINT mappsLen = 0;
  OV_UINT appsLen = 0;
  OV_UINT wlen = 0;

  // TODO: zzz: ask if it is of value uint :2018 Dez 28 13:58
  OV_UINT_VEC   wVec = {0};
  OV_STRING_VEC wNameVec = {0};

  OV_STRING reqStr = NULL;

  ov_memstack_lock();

  wlen = pinst->v_weights.veclen;
  Ov_SetDynamicVectorLength(&wNameVec, wlen, STRING);
  Ov_SetDynamicVectorLength(&wVec, wlen, UINT);
  for(OV_UINT i = 0; i < wlen; ++i) {
    OV_UINT    slen = 0;
    OV_STRING* splited =
        ov_string_split(pinst->v_weights.value[i], LB_APPMON_W_SEP, &slen);
    if(slen != 2) {
      pinst->v_status = LB_APPMON_ERROR;
      pinst->v_result = OV_ERR_BADPARAM;
      ov_logfile_error("lbalance_appMonitor: bad weights value");
      ov_memstack_unlock();
      return;
    }
    ov_string_setvalue(&wNameVec.value[i], splited[0]);
    wVec.value[i] = atoi(splited[1]);
    ov_string_freelist(splited);
  }

  /* getting max appslen */
  for(OV_UINT i = 0; i < pinst->v_appPaths.veclen; ++i) {
    OV_INSTPTR_ov_domain pdom = NULL;
    OV_INSTPTR_ov_object pobj = NULL;
    pdom = Ov_StaticPtrCast(
        ov_domain, ov_path_getobjectpointer(pinst->v_appPaths.value[i], 2));
    if(!pdom) {
      ov_logfile_error("lbalance_appMonitor: bad path appPaths[%u]=%s", i,
                       pinst->v_appPaths.value[i]);
      ov_memstack_unlock();
      return;
    }
    Ov_ForEachChild(ov_containment, pdom, pobj) { mappsLen++; }
  }

  Ov_SetDynamicVectorLength(&pinst->v_apps, mappsLen, STRING);
  Ov_SetDynamicVectorLength(&pinst->v_appReq, mappsLen, STRING);
  Ov_SetDynamicVectorLength(&pinst->v_loads, mappsLen, UINT);

  for(OV_UINT i = 0; i < pinst->v_appPaths.veclen; ++i) {
    // TODO: zzz: check if break breaks everything :2018 Dez 28 14:10
    OV_INSTPTR_ov_domain pdom = NULL;
    OV_INSTPTR_ov_object pobj = NULL;
    pdom = Ov_StaticPtrCast(
        ov_domain, ov_path_getobjectpointer(pinst->v_appPaths.value[i], 2));
    Ov_ForEachChild(ov_containment, pdom, pobj) {
      OV_STRING appName = NULL;
      for(OV_UINT j = 0; j < wlen; ++j) {
        ov_string_get(&appName, pobj->v_identifier, 0,
                      strcspn(pobj->v_identifier, LB_APPMON_GSENAME_SEP));
        /* ov_logfile_debug("lbalance_appMonitor: appName=%s", appName); */
        if(ov_string_compare(appName, wNameVec.value[j]) == OV_STRCMP_EQUAL) {
          pinst->v_loads.value[appsLen] = wVec.value[j];
          ov_string_print(&pinst->v_apps.value[appsLen], "%s/%s",
                          pinst->v_appPaths.value[i], pobj->v_identifier);
          /* getting req */
          pinst->p_upload.v_getVar = 0;
          ov_string_setvalue(
              &pinst->p_upload.v_path,
              ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pobj), 2));
          CTree_Upload_execute(&pinst->p_upload);
          if(pinst->p_upload.v_result) {
            ov_logfile_error("lbalance_appMonitor: upload failed to get reqs");
            ov_memstack_unlock();
            return;
          }
          if(pinst->p_upload.v_libs.veclen) {
            ov_string_setvalue(&reqStr, pinst->p_upload.v_libs.value[0]);
            for(OV_UINT k = 1; k < pinst->p_upload.v_libs.veclen; ++k) {
              ov_string_append(&reqStr, LB_APPMON_APPREQ_SEP);
              ov_string_append(&reqStr, pinst->p_upload.v_libs.value[k]);
            }
            ov_string_setvalue(&pinst->v_appReq.value[appsLen], reqStr);
            ov_string_setvalue(&reqStr, NULL);
          }
          appsLen++;
          break;
        }
      }
      ov_string_setvalue(&appName, NULL);
    }
  }

  Ov_SetDynamicVectorLength(&pinst->v_apps, appsLen, STRING);
  Ov_SetDynamicVectorLength(&pinst->v_appReq, appsLen, STRING);
  Ov_SetDynamicVectorLength(&pinst->v_loads, appsLen, UINT);

  ov_memstack_unlock();
  return;
}

