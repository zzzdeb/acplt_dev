/******************************************************************************
 *
 *   FILE
 *   ----
 *   Edge.c
 *
 *   History
 *   -------
 *   2018-07-11   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_TGraph
#define OV_COMPILE_LIBRARY_TGraph
#endif

#include "TGraph.h"
#include "libov/ov_macros.h"
#include "libov/ov_vector.h"
#include "math.h"

/**
 * @brief calculates cost of moving object on band for shift and rotation. (shift
 * and rotations are done simultaniously)
 *
 * @param pobj
 * @param pveclen
 *
 * @return { x+y/v_dir, rot/v_rot }
 */
OV_DLLFNCEXPORT OV_SINGLE*
                TGraph_Edge_physicalCost_get(OV_INSTPTR_TGraph_Edge pobj, OV_UINT* pveclen) {
  pobj->v_physicalCost.value[0] =
      fabs(pobj->v_Direction.value[0] + pobj->v_Direction.value[1]) /
      pobj->v_actVelocity.value[0];
  pobj->v_physicalCost.value[1] =
      fabs(pobj->v_Direction.value[2]) / pobj->v_actVelocity.value[1];

  *pveclen = pobj->v_physicalCost.veclen;
  return pobj->v_physicalCost.value;
}

/**
 * @brief physicalcost + trafficcost
 *
 * @param pobj
 * @param pveclen
 *
 * @return
 */
OV_DLLFNCEXPORT OV_SINGLE*
                TGraph_Edge_totalCost_get(OV_INSTPTR_TGraph_Edge pobj, OV_UINT* pveclen) {
  *pveclen = pobj->v_totalCost.veclen;
  OV_UINT    veclen;
  OV_SINGLE* physCost = TGraph_Edge_physicalCost_get(pobj, &veclen);
  pobj->v_totalCost.value[0] = physCost[0] + pobj->v_trafficCost.value[0];
  pobj->v_totalCost.value[1] = physCost[1] + pobj->v_trafficCost.value[1];
  return pobj->v_totalCost.value;
}

OV_DLLFNCEXPORT OV_RESULT TGraph_Edge_constructor(OV_INSTPTR_ov_object pobj) {
  /*
   *   local variables
   */
  OV_INSTPTR_TGraph_Edge pinst = Ov_StaticPtrCast(TGraph_Edge, pobj);
  OV_RESULT              result;

  /* do what the base class does first */
  result = fb_functionblock_constructor(pobj);
  if(Ov_Fail(result))
    return result;

  /* do what */
  OV_SINGLE val[3] = {0};
  Ov_SetDynamicVectorValue(&pinst->v_Direction, val, 3, SINGLE);
  OV_SINGLE costVal[2] = {0};
  Ov_SetDynamicVectorValue(&pinst->v_trafficCost, costVal, 2, SINGLE);
  Ov_SetDynamicVectorValue(&pinst->v_physicalCost, costVal, 2, SINGLE);
  Ov_SetDynamicVectorValue(&pinst->v_totalCost, costVal, 2, SINGLE);
  OV_SINGLE actVel[2] = {5, 5};
  Ov_SetDynamicVectorValue(&pinst->v_actVelocity, actVel, 2, SINGLE);
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void TGraph_Edge_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                            OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  return;
}

