
/******************************************************************************
 *
 *   FILE
 *   ----
 *   acceptNotifier.c
 *
 *   History
 *   -------
 *   2018-12-28   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"
#include "lbalance_helper.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT lbalance_acceptNotifier_reset_set(
    OV_INSTPTR_lbalance_acceptNotifier pinst, const OV_BOOL value) {
  pinst->v_reset = value;
  return 0;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_acceptNotifier_A_set(
    OV_INSTPTR_lbalance_acceptNotifier pobj, const OV_BOOL value) {
  pobj->v_A = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void
lbalance_acceptNotifier_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                   OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_acceptNotifier pinst =
      Ov_StaticPtrCast(lbalance_acceptNotifier, pfb);
  switch(pinst->v_status) {
    case LB_ACCEPTNOTIFIER_INIT:

      break;

      LB_CASEERROR;
  }

  return;
}

