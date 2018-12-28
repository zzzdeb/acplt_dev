
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
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_init_set(
    OV_INSTPTR_lbalance_BRAD          pobj,
    const OV_BOOL  value
) {
    pobj->v_init = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_B_set(
    OV_INSTPTR_lbalance_BRAD          pobj,
    const OV_BOOL  value
) {
    pobj->v_B = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_R_set(
    OV_INSTPTR_lbalance_BRAD          pobj,
    const OV_BOOL  value
) {
    pobj->v_R = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_A_set(
    OV_INSTPTR_lbalance_BRAD          pobj,
    const OV_BOOL  value
) {
    pobj->v_A = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_D_set(
    OV_INSTPTR_lbalance_BRAD          pobj,
    const OV_BOOL  value
) {
    pobj->v_D = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void lbalance_BRAD_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_lbalance_BRAD pinst = Ov_StaticPtrCast(lbalance_BRAD, pfb);

    return;
}
