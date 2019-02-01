
/******************************************************************************
*
*   FILE
*   ----
*   inLSEOptimizer.c
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


OV_DLLFNCEXPORT void lbalance_inLSEOptimizer_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_lbalance_inLSEOptimizer pinst = Ov_StaticPtrCast(lbalance_inLSEOptimizer, pfb);

    if (!pinst->v_enable)
    	return;

    // Append local load and capacity to load/capacity vectors
    OV_UINT localIndex = pinst->v_neighbourLoads.veclen;
    Ov_SetDynamicVectorLength(&pinst->v_neighbourLoads,pinst->v_neighbourLoads.veclen+1, UINT);
    pinst->v_neighbourLoads.value[localIndex] = pinst->v_localLoad;
    Ov_SetDynamicVectorLength(&pinst->v_neighbourCaps,pinst->v_neighbourCaps.veclen+1, UINT);
    pinst->v_neighbourCaps.value[localIndex] = pinst->v_localCap;

    // Map requests to neighbours
    OV_UINT_VEC reqNeighbourIndexes = lse_optimization_lookup_requesting_nodes(pinst->v_reqIPs, pinst->v_neighbourIPs);

    // Calculate optimal request
    OV_UINT requestToAccept = lse_optimization_chose_request(
            pinst->v_reqLoads, reqNeighbourIndexes, pinst->v_neighbourLoads, pinst->v_neighbourCaps, localIndex);

    // Set output variable
    // We need to transform the error/FALSE value from MAX_UINT to -1
    if (requestToAccept < pinst->v_reqLoads.veclen) {
        pinst->v_reqIndex = requestToAccept;
    } else {
        pinst->v_reqIndex = -1;
    }

    // Remove local load and capacity from load/capacity vectors
    Ov_SetDynamicVectorLength(&pinst->v_neighbourLoads,pinst->v_neighbourLoads.veclen-1, UINT);
    Ov_SetDynamicVectorLength(&pinst->v_neighbourCaps,pinst->v_neighbourCaps.veclen-1, UINT);

	// Free memory
	Ov_SetDynamicVectorLength(&reqNeighbourIndexes, 0, UINT);
}

