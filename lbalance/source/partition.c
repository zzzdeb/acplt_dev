/******************************************************************************
 *
 *   file
 *   ----
 *   partition.c
 *
 *   history
 *   -------
 *   Tue 16 Apr 2019 12:46:22 PM CEST   file created
 *
 ******************************************************************************/

#ifndef ov_compile_library_lbalance
#define ov_compile_library_lbalance
#endif

#include "cJSON.h"
#include "lbalance.h"
#include "lbalance_helper.h"

#include "libov/ov_macros.h"
#include "libov/ov_path.h"

OV_DLLFNCEXPORT void
lbalance_partition_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_partition pinst =
      Ov_StaticPtrCast(lbalance_partition, pfb);

  cJSON*                     map = NULL;
  cJSON*                     newIPjson = NULL;
  OV_STRING                  mapstr = NULL;
  OV_INSTPTR_ov_domain       pgse = NULL;
  OV_INSTPTR_fb_controlchart pPaletteControlchart = NULL;
  OV_STRING                  currentTable = NULL;

  ov_string_print(&mapstr, "{%s}", pinst->v_map);
  map = cJSON_Parse(mapstr);
  if(!map) {
    ov_logfile_error("lbalance_partition: couldnt parse v_map %s",
                     pinst->v_map);
    pinst->v_status = LB_INTERNALERROR;
    return;
  }
  ov_memstack_lock();

  // 1. Iterate over children with PA* of GSE
  Ov_ForEachChildEx(ov_containment, pgse, pPaletteControlchart,
                    fb_controlchart) {
    // 2. Get Current of child and see if it is needed to transported
    OV_INSTPTR_fb_stringport pPaletteCurrentPort = Ov_SearchChildEx(
        ov_containment, pPaletteControlchart, "Current", fb_stringport);
    if(pPaletteCurrentPort == NULL || pPaletteCurrentPort->v_value == NULL) {
      ov_logfile_warning("lbalance_partition: NULL at value %s %s",
                         pPaletteControlchart->v_identifier);
      continue;
    }
    ov_string_setvalue(&currentTable, pPaletteCurrentPort->v_value);

    newIPjson = cJSON_GetObjectItem(map, currentTable);
    if(!cJSON_IsString(newIPjson)) {
      ov_logfile_error("lbalance_partition: wrong format");
      pinst->v_status = LB_INTERNALERROR;
      ov_memstack_unlock();
      return;
    }
    if(ov_string_compare(pinst->v_currentIP, newIPjson->valuestring) ==
       OV_STRCMP_EQUAL) {
      continue;
    }
    ov_logfile_debug("lbalance_partition: syncing %s from %s to %s",
                     pPaletteControlchart->v_identifier, pinst->v_currentIP,
                     newIPjson->valuestring);
    OV_INSTPTR_sync_dsync pdsync = &pinst->p_dsync;

    // 2.1. Transport this with dsync
    pdsync->v_EN = 1;
    OV_STRING palettePath = ov_path_getcanonicalpath(
        Ov_StaticPtrCast(ov_object, pPaletteControlchart), 2);
    ov_string_setvalue(&pdsync->v_srcPath, palettePath);
    ov_string_setvalue(&pdsync->v_selfHost, pinst->v_currentIP);
    sync_dsync_trigger_set(pdsync, 1);

    // 2.2. Adjust TopologyAgent on dst
  }

  ov_memstack_unlock();
  return;
}

