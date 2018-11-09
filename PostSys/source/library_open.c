/******************************************************************************
 *
 *   FILE
 *   ----
 *   library_open.c
 *
 *   Author: C. Hansen, D. Kampert
 *
 *   History
 *   -------
 *   2012-10-19   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_PostSys
#define OV_COMPILE_LIBRARY_PostSys
#endif

#include "PostSys.h"
#include "PostSys_helpers.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_path.h"
#include "libov/ov_time.h"
#include "libov/ov_result.h"
#include "libov/ov_library.h"

#ifdef ov_library_open_PostSys
#undef ov_library_open_PostSys
#endif

/*
 * This function will be called, when the library is loaded.
 * It could generate components and initializes the startup procedure
 */
OV_RESULT ov_library_setglobalvars_PostSys_new(void) {
	OV_RESULT result;
	OV_INSTPTR_ov_domain domain = NULL;
	OV_INSTPTR_PostSys_MsgDelivery PostSysDelivery = NULL;
	OV_INSTPTR_ov_library pLibKsapi = NULL;
	OV_INSTPTR_PostSys_msgIdentificator pIdentificator = NULL;
	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */

	result = ov_library_setglobalvars_PostSys();

	ov_memstack_lock();

	//MsgDelivery
	domain = (OV_INSTPTR_ov_domain) ov_path_getobjectpointer(COMPATH, 2);
	PostSysDelivery = Ov_DynamicPtrCast(PostSys_MsgDelivery,
		Ov_SearchChild(ov_containment,domain,"PostSys"));
	if(PostSysDelivery == NULL) {
		result = Ov_CreateObject(PostSys_MsgDelivery, PostSysDelivery, domain,
			"PostSys");
		if(Ov_Fail(result) && (result != OV_ERR_ALREADYEXISTS)) {
			ov_memstack_lock();
			ov_logfile_error(
				"PostSys: Fatal: Couldn't create Object 'PostSys' Reason: %s",
				ov_result_getresulttext(result));
			ov_memstack_unlock();
			return result;
		}
		if(Ov_OK(result)) PostSysDelivery->v_cycInterval = 1;
	}
	/*	create protocol identificator for msgs	*/
	pIdentificator = Ov_StaticPtrCast(PostSys_msgIdentificator,
		Ov_SearchChild(ov_containment, PostSysDelivery, "Identificator"));
	if(pIdentificator)
	Ov_DeleteObject(pIdentificator);
	pIdentificator = NULL;

	result = Ov_CreateObject(PostSys_msgIdentificator, pIdentificator,
		PostSysDelivery, "Identificator");
	if(Ov_Fail(result)) {
		ov_logfile_error("Fatal: could not create Identificator object");
		return result;
	}

	ov_memstack_unlock();
	return OV_ERR_OK;
}
/*
 *       Replace the 'setglobalvars' function of a library with this
 *       previous one, which additionally creates instances.
 * 	This is called by the OV system upon library load.
 */
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_PostSys(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_PostSys_new;

	/*
	 *       replace the 'setglobalvars' function created by the code generator
	 *       with a new one.
	 */
	OV_LIBRARY_DEF_PostSys_new = ov_library_open_PostSys_old();
	OV_LIBRARY_DEF_PostSys_new->setglobalvarsfnc =
			ov_library_setglobalvars_PostSys_new;
	return OV_LIBRARY_DEF_PostSys_new;
}
