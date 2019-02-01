
/******************************************************************************
*
*   FILE
*   ----
*   outLSEOptimizer.c
*
*   History
*   -------
*   2019-01-17   File created
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
#include "libov/ov_macros.h"

#include "lse_optimization.h"


OV_DLLFNCEXPORT void lbalance_outLSEOptimizer_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_lbalance_outLSEOptimizer pinst = Ov_StaticPtrCast(lbalance_outLSEOptimizer, pfb);

    // Append local load and capacity to load/capacity vectors
    OV_UINT localIndex = pinst->v_neighbourLoads.veclen;
    Ov_SetDynamicVectorLength(&pinst->v_neighbourLoads,pinst->v_neighbourLoads.veclen+1, UINT);
    pinst->v_neighbourLoads.value[localIndex] = pinst->v_localLoad;
    Ov_SetDynamicVectorLength(&pinst->v_neighbourCaps,pinst->v_neighbourCaps.veclen+1, UINT);
    pinst->v_neighbourCaps.value[localIndex] = pinst->v_localCap;

    // Calculate optimal movement
    AppMovementSpec appToMove = lse_optimization_chose_app(
            pinst->v_localAppLoads, pinst->v_neighbourLoads, pinst->v_neighbourCaps, localIndex);
    // TODO implement checking of localAppReq against neighbourSysInfos

    // Calculate set output variables
    if (appToMove.valid) {
        ov_string_setvalue(&pinst->v_outApp, pinst->v_localApps.value[appToMove.appId]);
        pinst->v_outLoad = pinst->v_localAppLoads.value[appToMove.appId];
        ov_string_setvalue(&pinst->v_outIP, pinst->v_neighbourIPs.value[appToMove.nodeId]);
    } else {
        ov_string_setvalue(&pinst->v_outApp, NULL);
        pinst->v_outLoad = 0;
        ov_string_setvalue(&pinst->v_outIP, NULL);
    }
}

