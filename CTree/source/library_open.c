
/******************************************************************************
*
*   FILE
*   ----
*   library_open.c
*
*   History
*   -------
*   2014-10-13   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_CTree
#define OV_COMPILE_LIBRARY_CTree
#endif
#include "CTree.h"
#ifdef ov_library_open_CTree
#undef ov_library_open_CTree
#endif

#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"

 OV_RESULT ov_library_setglobalvars_CTree_new(void) {
	OV_RESULT result = OV_ERR_OK;
		OV_INSTPTR_CTree_Upload	pupload	=	NULL;
		OV_INSTPTR_ov_domain	pCTree 	= 	NULL;
		OV_INSTPTR_ov_domain pData = NULL;
		OV_INSTPTR_fb_task pUrTask = NULL;
	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	 result = ov_library_setglobalvars_CTree();
	 if (Ov_Fail(result)){
		 return result;
	 }
	/*
	 * Your code goes here.
	 */
	pData = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &pdb->root, "data"));
	if(!pData){
		return result;
	}
	pCTree = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pData, "CTree"));
	if(!pCTree){
		result = Ov_CreateObject(ov_domain, pCTree, pData, "CTree");
		if (Ov_Fail(result)){
			ov_logfile_error("Error: %s: couldnt create CTree in /data;", ov_result_getresulttext(result));
			return result;
		}
	}
	pupload = Ov_StaticPtrCast(CTree_Upload, Ov_SearchChild(ov_containment, pCTree, "Upload"));
	if(!pupload){
		result = Ov_CreateObject(CTree_Upload, pupload, pCTree, "Upload");
		if (Ov_Fail(result)){
			ov_logfile_error("Error: %s: couldnt create upload;", ov_result_getresulttext(result));
		}
		OV_INSTPTR_ov_object ptmp = ov_path_getobjectpointer("/Tasks/UrTask", VERSION_FOR_CTREE);
		pUrTask = Ov_StaticPtrCast(fb_task, ptmp);
//		if(pUrTask)
//			Ov_Link(fb_tasklist, pUrTask, pupload);
	}

//		result = Ov_CreateObject(CTree_Upload, pupload, pCTree, "Transport");
//		if (Ov_Fail(result)){
//			ov_logfile_error("Error: %s: couldnt create transport;", ov_result_getresulttext(result));
//		}

	return result;
	}

 OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_CTree(void) {
 	/* local variables */
 	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_CTree_new;
 	/*
 	*       replace the 'setglobalvars' function created by the code generator
 	*       with a new one.
 	*/
 	OV_LIBRARY_DEF_CTree_new = ov_library_open_CTree_old();
 	OV_LIBRARY_DEF_CTree_new->setglobalvarsfnc = ov_library_setglobalvars_CTree_new;
 	return OV_LIBRARY_DEF_CTree_new;
  }

