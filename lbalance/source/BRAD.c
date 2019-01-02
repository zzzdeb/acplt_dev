
/******************************************************************************
 *
 *   FILE
 *   ----
 *   BRAD.c
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

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_init_set(OV_INSTPTR_lbalance_BRAD pobj,
                                                 const OV_BOOL value) {
  if(value && !pobj->v_init) {
    Ov_WarnIf(LB_BRAD_D != pobj->v_status);
    pobj->v_status = LB_BRAD_B;
    pobj->v_B = 1;
    pobj->v_R = 0;
    pobj->v_A = 0;
    pobj->v_D = 0;
    ov_time_gettime(&pobj->v_timeLastEvent);
  }
  pobj->v_init = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_B_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_B = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_R_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_R = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_A_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_A = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_D_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_D = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void lbalance_BRAD_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                              OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_BRAD pinst = Ov_StaticPtrCast(lbalance_BRAD, pfb);
  OV_TIME                  now = {0};
  OV_TIME_SPAN             tstemp = {0};
  OV_BOOL                  changePhase = 0;

  ov_time_gettime(&now);
  ov_time_diff(&tstemp, &now, &(pinst->v_timeLastEvent));
  if((tstemp.secs > (OV_INT)pinst->v_T) ||
     ((tstemp.secs == (OV_INT)pinst->v_T) && (tstemp.usecs > 0))) {
    changePhase = 1;
  }
  switch(pinst->v_status) {
    case LB_BRAD_B:
      if(changePhase) {
        pinst->v_B = 0;
        pinst->v_R = 1;
        pinst->v_status = LB_BRAD_R;
        pinst->v_timeLastEvent = now;
      }
      return;
      break;
    case LB_BRAD_R:
      if(changePhase) {
        pinst->v_R = 0;
        pinst->v_A = 1;
        pinst->v_status = LB_BRAD_A;
        pinst->v_timeLastEvent = now;
      }
      break;
    case LB_BRAD_A:
      if(changePhase) {
        pinst->v_A = 0;
        pinst->v_D = 1;
        pinst->v_init = 0;
        pinst->v_status = LB_BRAD_D;
        pinst->v_timeLastEvent = now;
      }
      break;
    case LB_BRAD_D:
      if(changePhase) {
        pinst->v_D = 0;
        pinst->v_B = 1;
        pinst->v_status = LB_BRAD_B;
        pinst->v_timeLastEvent = now;
      }
      break;

      LB_CASEERROR
  }
  return;
}

