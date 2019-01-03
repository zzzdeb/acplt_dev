/******************************************************************************
 *
 *   FILE
 *   ----
 *   Common.c
 *
 *   History
 *   -------
 *   Mi 02 Jan 2019 23:10:42 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT lbalance_Common_reset_set(
    OV_INSTPTR_lbalance_Common pinst, const OV_BOOL value) {
  pinst->v_reset = value;
  return 0;
}

OV_DLLFNCEXPORT void lbalance_Common_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                                OV_TIME* pltc) {
  /*
   *   local variables
   */
  /* OV_INSTPTR_lbalance_Common pinst = Ov_StaticPtrCast(lbalance_Common, pfb);
   */
  return;
}

