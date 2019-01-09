/******************************************************************************
 *
 *   FILE
 *   ----
 *   mock.c
 *
 *   History
 *   -------
 *   Do 03 Jan 2019 11:50:41 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"
#include "lbalance_helper.h"

#include "libov/ov_macros.h"

/**
 * Uninlined function to update the list of currently loaded ov libraries on a
 * monitor object.
 *
 * Code based on ov_vendortree_getlibraries()
 *
 * @param pinst The monitor instantce to update its library list
 */
static void monitor_update_libs(OV_INSTPTR_lbalance_mock pinst) {
  /* count associations */
  OV_UINT               libs = 0;
  OV_INSTPTR_ov_library plib;
  Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
    libs++;
  }
  OV_INSTPTR_ov_class pclass;
  Ov_ForEachChild(ov_inheritance, pclass_ov_library, pclass) {
    Ov_ForEachChildEx(ov_instantiation, pclass, plib, ov_library) { libs++; }
  }
  Ov_SetDynamicVectorLength(&pinst->v_ovLibs, libs, STRING);

  /* enter library paths into a string vector */
  libs = 0;
  Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
    ov_string_setvalue(&(pinst->v_ovLibs.value[libs++]), plib->v_identifier);
  }
  Ov_ForEachChild(ov_inheritance, pclass_ov_library, pclass) {
    Ov_ForEachChildEx(ov_instantiation, pclass, plib, ov_library) {
      ov_string_setvalue(&(pinst->v_ovLibs.value[libs++]), plib->v_identifier);
    }
  }
}

OV_DLLFNCEXPORT OV_RESULT lbalance_mock_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT                result = OV_ERR_OK;
  OV_INSTPTR_lbalance_mock pinst = Ov_StaticPtrCast(lbalance_mock, pobj);

  result = ov_object_constructor(pobj);

  ov_string_setvalue(&pinst->v_sysOS, "linux");
  OV_STRING servers[] = {"localhost\tlocalhost\t127.0.0.1\t7509\tMANAGER"};
  Ov_SetDynamicVectorValue(&pinst->v_ovServers, servers, 1, STRING);
  OV_STRING appPaths[] = {"/TechUnits"};
  Ov_SetDynamicVectorValue(&pinst->v_appPaths, appPaths, 1, STRING);

  monitor_update_libs(pinst);

  return result;
}

OV_DLLFNCEXPORT void lbalance_mock_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                              OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_mock pinst = Ov_StaticPtrCast(lbalance_mock, pfb);
  for(OV_UINT i = 0; i < pinst->v_apps.veclen; ++i) {
    /* if(ov_string_match(pinst->v_apps.value[i], " GSE1
        ") == OV_STRCMP_EQUAL) { */
    ov_string_setvalue(&pinst->v_outApp, pinst->v_apps.value[i]);
    pinst->v_outLoad = pinst->v_loads.value[i];
    OV_UINT    slen = 0;
    OV_STRING* splited =
        ov_string_split(pinst->v_appReq.value[i], LB_APPMON_APPREQ_SEP, &slen);
    ov_string_setvalue(&pinst->v_outRequirements, pinst->v_appReq.value[i]);
    ov_string_freelist(splited);
    if(pinst->v_apps.veclen && pinst->v_neighborIPs.veclen) {
      ov_string_print(&pinst->v_destination, "//%s/%s%s",
                      pinst->v_neighborIPs.value[0],
                      pinst->v_serverNames.value[0], pinst->v_apps.value[0]);
    }
    break;
    /* } */
  }

  return;
}

