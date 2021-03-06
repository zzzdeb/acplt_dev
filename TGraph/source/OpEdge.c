
/******************************************************************************
*
*   FILE
*   ----
*   OpEdge.c
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


OV_DLLFNCEXPORT OV_SINGLE TGraph_OpEdge_totalCost_get(
    OV_INSTPTR_TGraph_OpEdge          pobj
) {
	return (pobj->v_trafficCost + pobj->v_physicalCost);
}

OV_DLLFNCEXPORT void TGraph_OpEdge_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
//    OV_INSTPTR_TGraph_OpEdge pinst = Ov_StaticPtrCast(TGraph_OpEdge, pfb);

    return;
}

