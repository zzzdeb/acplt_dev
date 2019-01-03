/******************************************************************************
 *
 *   FILE
 *   ----
 *   sendInitiator.c
 *
 *   History
 *   -------
 *   Do 03 Jan 2019 00:34:31 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT lbalance_sendInitiator_reset_set(
    OV_INSTPTR_lbalance_sendInitiator pinst, const OV_BOOL value) {
  pinst->v_reset = value;
  return 0;
}

OV_DLLFNCEXPORT void
lbalance_sendInitiator_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                  OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  /* OV_INSTPTR_lbalance_sendInitiator pinst = */
  /* Ov_StaticPtrCast(lbalance_sendInitiator, pfb); */

  return;
}

