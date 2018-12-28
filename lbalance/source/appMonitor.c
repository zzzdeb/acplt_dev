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

#include "object_helper.h"

OV_DLLFNCEXPORT OV_UINT
                lbalance_appMonitor_sum_get(OV_INSTPTR_lbalance_appMonitor pinst) {
  OV_UINT sum = 0;
  for(OV_UINT i = 0; i < pinst->v_loads.veclen; ++i) {
    sum += pinst->v_loads.value[i];
  }
  return sum;
}

OV_DLLFNCEXPORT void
lbalance_appMonitor_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_appMonitor pinst =
      Ov_StaticPtrCast(lbalance_appMonitor, pfb);

  OV_UINT appsLen = 0;
  OV_UINT wlen = 0;

  // TODO: zzz: ask if it is of value uint :2018 Dez 28 13:58
  OV_UINT_VEC   wVec = {0};
  OV_STRING_VEC wNameVec = {0};

  wlen = pinst->v_weights.veclen;
  Ov_SetDynamicVectorLength(&wNameVec, appsLen, STRING);
  Ov_SetDynamicVectorLength(&wVec, appsLen, UINT);
  for(OV_UINT i = 0; i < wlen; ++i) {
    OV_UINT    slen = 0;
    OV_STRING* splited =
        ov_string_split(pinst->v_weights.value[i], LB_APPMON_W_SEP, &slen);
    if(slen != 2) {
      pinst->v_status = LB_APPMON_ERROR;
      pinst->v_result = OV_ERR_BADPARAM;
      ov_logfile_error("lbalance_appMonitor: bad weights value");
      return;
    }
    ov_string_setvalue(&wNameVec.value[i], splited[0]);
    wVec.value[i] = atoi(splited[1]);
    ov_string_freelist(splited);
  }

  Ov_SetDynamicVectorLength(&pinst->v_apps, appsLen, STRING);
  Ov_SetDynamicVectorLength(&pinst->v_appReq, appsLen, STRING);
  Ov_SetDynamicVectorLength(&pinst->v_loads, appsLen, UINT);

  for(OV_UINT i = 0; i < appsLen; ++i) {
    // TODO: zzz: check if break breaks everything :2018 Dez 28 14:10
    OV_STRING appName = NULL;
    for(OV_UINT j = 0; j < wlen; ++j) {
      ov_string_get(&appName, pinst->v_apps.value[i], 0,
                    strcspn(pinst->v_apps.value[i], LB_APPMON_GSENAME_SEP));
      ov_logfile_debug("lbalance_appMonitor: appName=%s", appName);
      appName = pinst->v_apps.value[i];
      if(ov_string_compare(appName, wNameVec.value[i]) == OV_STRCMP_EQUAL) {
        pinst->v_loads.value[i] = wVec.value[j];
        break;
      }
    }
    ov_string_setvalue(&appName, NULL);
  }

  for(OV_UINT i = 0; i < 5; ++i) {
    for(OV_UINT j = 0; j < 5; ++j) {
      if(j == 3) {
        ov_logfile_info("lbalance_appMonitor: %d", j);
        break;
      }
    }
  }
  return;
}

