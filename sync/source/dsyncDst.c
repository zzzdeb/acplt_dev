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

OV_DLLFNCEXPORT OV_RESULT sync_dsyncDst_switch_set(OV_INSTPTR_sync_dsync pobj,
                                                   const OV_BOOL value) {
  if(value) {
  }
  pobj->v_reset = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void sync_dsyncDst_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                              OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_dsyncDst pinst = Ov_StaticPtrCast(sync_dsyncDst, pfb);

  return;
}

