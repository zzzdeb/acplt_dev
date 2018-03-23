/******************************************************************************
 *
 *   FILE
 *   ----
 *   CTreeCommon.c
 *
 *   History
 *   -------
 *   2018-03-20   File created
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
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

#include "ksbase.h"
#include "ksbase_helper.h"
#include "ks_logfile.h"

#include <stdarg.h>

#define CTREE_COMMON_INTERNALERROR			128
#define CTREE_COMMON_EXTERNALERROR			64
#define CTREE_COMMON_INITIAL				0
#define CTREE_COMMON_WAITINGFORANSWER		1
#define CTREE_COMMON_REQUESTCOMPLETED		2

OV_DLLFNCEXPORT void CTree_Transport_startup(OV_INSTPTR_ov_object pobj) {

	/*
	 *   local variables
	 *   */

	OV_INSTPTR_CTree_Transport pinst = Ov_StaticPtrCast(CTree_Transport, pobj);

	/*
	 do what the base class does first
	 */
	fb_functionblock_startup(pobj);

//	 do what
	pinst->v_result = OV_ERR_OK;
//    pinst->v_status = CTree_COMMON_INITIAL;

	return;
}

OV_DLLFNCEXPORT void CTree_Transport_shutdown(OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	/*

	 do what

	 set the object's state to "shut down"
	 */
	fb_functionblock_shutdown(pobj);

	return;
}

OV_RESULT parse_kspath(const OV_STRING kspath, OV_STRING * serverHost,
		OV_STRING * serverName, OV_STRING * path) {
	OV_RESULT result = OV_ERR_OK;
	OV_STRING * splited = NULL;
	OV_UINT len = 0;

	if (kspath == NULL || serverName == NULL || serverHost == NULL
			|| path == NULL)
		return OV_ERR_BADPARAM;

	splited = ov_string_split(kspath, ":", &len);
	if (len < 2)
		return OV_ERR_BADPARAM;

	result = ov_string_setvalue(serverHost, splited[0]);
	if (Ov_Fail(result))
		return OV_ERR_GENERIC;
	result = ov_string_setvalue(serverName, splited[1]);
	if (Ov_Fail(result))
		return OV_ERR_GENERIC;
	result = ov_string_setvalue(path, splited[2]);
	if (Ov_Fail(result))
		return OV_ERR_GENERIC;
	return result;
}

OV_DLLFNCEXPORT OV_RESULT CTree_Transport_Reset_set(
		OV_INSTPTR_CTree_Transport pobj, OV_BOOL value) {
	OV_INSTPTR_ksbase_ClientBase pClient = NULL;
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
	OV_RESULT result = OV_ERR_OK;

	if (value && (!pobj->v_Reset)) {
		pobj->v_status = CTREE_COMMON_INITIAL;
		pobj->v_result = OV_ERR_OK;
		Ov_ForEachChildEx(ov_containment, pobj, pClient, ksbase_ClientBase)
		{ /*	find the object in the containment which is derived from ClientBase	*/
			break;
		}

		if (pClient) {
			Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);
			if (pVtblClient) {
				result = pVtblClient->m_reset(pClient);
			}
		}
	}

	pobj->v_Reset = value;
	return result;
}

OV_DLLFNCEXPORT void CTree_Transport_submit(OV_INSTPTR_CTree_Transport pobj) {
	return;
}

OV_RESULT CTree_helper_getClientPointers(OV_INSTPTR_CTree_Transport pCommon,
		OV_INSTPTR_ksbase_ClientBase* pClient,
		OV_VTBLPTR_ksbase_ClientBase* pVtblClient) {
	OV_RESULT result;
	OV_STRING tempstr = NULL;

	if (!pClient || !pVtblClient)
		return OV_ERR_BADPARAM;

	*pClient = NULL;
	*pVtblClient = NULL;

	Ov_ForEachChildEx(ov_containment, pCommon, *pClient, ksbase_ClientBase)
	{
		break;
	}
	if (!(*pClient)) {/*	no client found --> create one	*/
		const char defaultClient[] = "xdrClient";
		OV_INSTPTR_ov_class pClassClient = NULL;
		ov_memstack_lock();
		tempstr = ov_vendortree_getcmdlineoption_value("KS_USECLIENT");
		if (!tempstr)
			tempstr = ov_vendortree_getcmdlineoption_value("CTREE_USECLIENT");

		if (!tempstr)
			tempstr = (OV_STRING) defaultClient;
		else if (!(*tempstr))
			return OV_ERR_OK;

		Ov_ForEachChild(ov_inheritance, pclass_ksbase_ClientBase, pClassClient)
		{
			if (ov_string_compare(pClassClient->v_identifier,
					tempstr) == OV_STRCMP_EQUAL) {
				result = ov_class_createobject(pClassClient,
						Ov_StaticPtrCast(ov_domain, pCommon), "Client",
						OV_PMH_DEFAULT, NULL, NULL, NULL,
						(OV_INSTPTR_ov_object*) pClient);
				if (Ov_Fail(result)) {
					ov_memstack_unlock();
					return result;
				}
				break;
			}
		}
		ov_memstack_unlock();
	}

	/*	here we definitely have a client object	*/
	Ov_GetVTablePtr(ksbase_ClientBase, *pVtblClient, *pClient);
	if (!pVtblClient)
		return OV_ERR_BADOBJTYPE;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT CTree_Transport_prepareSubmit(
		OV_INSTPTR_CTree_Transport pobj, OV_INSTPTR_ksbase_ClientBase* pClient,
		OV_VTBLPTR_ksbase_ClientBase* pVtblClient) {
	OV_RESULT result;
	OV_STRING serverHost = NULL;
	OV_STRING serverName = NULL;
	OV_STRING path = NULL;
	if (!pobj->v_targetKS) {
		ov_logfile_error("%s: no serverHost set. aborting", pobj->v_identifier);
		pobj->v_status = CTREE_COMMON_INTERNALERROR;
		pobj->v_result = OV_ERR_BADPARAM;
		return OV_ERR_BADPARAM;
	}
	result = parse_kspath(pobj->v_targetKS, &serverHost, &serverName, &path);

	if (!serverHost) {
		ov_logfile_error("%s: no serverHost set. aborting", pobj->v_identifier);
		pobj->v_status = CTREE_COMMON_INTERNALERROR;
		pobj->v_result = OV_ERR_BADPARAM;
		return OV_ERR_BADPARAM;
	}

	result = CTree_helper_getClientPointers(pobj, pClient, pVtblClient);
	if (Ov_Fail(result)) {
		ov_logfile_error("%s: submit: no Client found. Cannot submit",
				pobj->v_identifier);
		pobj->v_status = CTREE_COMMON_INTERNALERROR;
		pobj->v_result = result;
		return result;
	}

	result = ksbase_ClientBase_serverHost_set(*pClient, serverHost);
	if (Ov_Fail(result)) {
		ov_logfile_error("%s: submit: could not set serverHost at Client",
				pobj->v_identifier);
		pobj->v_status = CTREE_COMMON_INTERNALERROR;
		pobj->v_result = result;
		return result;
	}

	if (serverName) {
		result = ksbase_ClientBase_serverName_set(*pClient, serverName);
		if (Ov_Fail(result)) {
			ov_logfile_error("%s: submit: could not set serverName at Client",
					pobj->v_identifier);
			pobj->v_status = CTREE_COMMON_INTERNALERROR;
			pobj->v_result = result;
			return result;
		}
	}

	(*pClient)->v_holdConnection = pobj->v_holdConnection;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT CTree_Transport_genSetForSubmit(
		OV_INSTPTR_CTree_Transport pinst, OV_STRING serverHost,
		OV_STRING serverName, OV_STRING path) {
	OV_RESULT result = OV_ERR_OK;
	OV_STRING kstarget = "";

	result = ov_string_print(&pinst->v_targetKS, "%s:%s:%s", serverHost,
			serverName, path);
	if (Ov_Fail(result)) {
		pinst->v_status = CTREE_COMMON_INTERNALERROR;
		pinst->v_result = result;
		return result;
	}
	return result;
}
/*

 OV_DLLFNCEXPORT OV_ACCESS CTree_Transport_getaccess(OV_INSTPTR_ov_object pobj,
 const OV_ELEMENT *pelem, const OV_TICKET *pticket) {

 *	local variables



 *	switch based on the element's type

 switch (pelem->elemtype) {
 case OV_ET_VARIABLE:
 if (pelem->elemunion.pvar->v_offset
 >= offsetof(OV_INST_ov_object, __classinfo)) {
 if (pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
 return OV_AC_NONE;
 else
 return OV_AC_READWRITE;
 }
 break;
 default:
 break;
 }

 return ov_object_getaccess(pobj, pelem, pticket);
 }
 */

void CTree_Transport_exec_callback(const OV_INSTPTR_ov_domain this,
		const OV_INSTPTR_ov_domain that) {
	OV_INSTPTR_CTree_Transport thisSV = Ov_StaticPtrCast(CTree_Transport, this);
	OV_INSTPTR_ksbase_ClientBase pClient = Ov_StaticPtrCast(ksbase_ClientBase,
			that);
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
	OV_UINT itemsLength;
	OV_RESULT* itemsResults = NULL;
	OV_RESULT result;

	if (!this || !that) {
		KS_logfile_error(("callback issued with NULL pointers. aborting."));
		return;
	}

	Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

	if (!pVtblClient) {
		KS_logfile_error(
				("%s callback: could not determine Vtable of Client %s. aborting", this->v_identifier, that->v_identifier));
		thisSV->v_status = CTREE_COMMON_INTERNALERROR;
		thisSV->v_result = OV_ERR_BADOBJTYPE;
		return;
	}
	ov_memstack_lock();
	result = pVtblClient->m_processSetVar(pClient, NULL,
			(OV_RESULT*) &(thisSV->v_result), &itemsLength, &itemsResults);
	if (Ov_Fail(result)) {
		thisSV->v_status = CTREE_COMMON_INTERNALERROR;
		thisSV->v_result = result;
		ov_memstack_unlock();
		return;
	}

	if (Ov_Fail(thisSV->v_result)) {
		thisSV->v_status = CTREE_COMMON_EXTERNALERROR;
		ov_memstack_unlock();
		return;
	}

//	thisSV->v_varRes = itemsResults[0];
	thisSV->v_status = CTREE_COMMON_REQUESTCOMPLETED;

	ov_memstack_unlock();
	return;
}

OV_RESULT CTree_Transport_execute(OV_INSTPTR_CTree_Transport pinst) {
	OV_INSTPTR_ksbase_ClientBase pClient = NULL;
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;

	OV_RESULT result;
	OV_SETVAR_ITEM* items = NULL;
	OV_UINT numberOfItems = 2;
	OV_STRING path = NULL;
	OV_UINT len = 0;
	result = CTree_Transport_prepareSubmit(pinst, &pClient, &pVtblClient);
	if (Ov_Fail(result))
		return result;

	items = Ov_HeapMalloc(numberOfItems * sizeof(OV_SETVAR_ITEM));
	if (!items) {
		pinst->v_status = CTREE_COMMON_INTERNALERROR;
		pinst->v_result = OV_ERR_HEAPOUTOFMEMORY;
		return result;
	}

	path = ov_string_split(pinst->v_targetKS, ":", &len)[2];
	if (pinst->v_tree) {
		items[0].path_and_name = "";
		ov_string_print(&items[0].path_and_name, "%s.%s", path, "json"); /*	see comment below	*/
		items[0].var_current_props.value.vartype = KS_VT_STRING;
//		items[0].var_current_props.value.valueunion.val_string = "test";
		items[0].var_current_props.value.valueunion.val_string = pinst->v_tree;
		items[1].path_and_name = "";
		ov_string_print(&items[1].path_and_name, "%s.%s", path, "path");
		items[1].var_current_props.value.vartype = KS_VT_STRING;
		items[1].var_current_props.value.valueunion.val_string = "test";

	} else {
		KS_logfile_error(
				("%s: submit: own Variable has empty path", pobj->v_identifier));
		pinst->v_status = CTREE_COMMON_INTERNALERROR;
		pinst->v_result = result;
		Ov_HeapFree(items);
		return result;
	}

	/*	do the actual submit	*/
	pVtblClient->m_requestSetVar(pClient, NULL, numberOfItems, items,
			(OV_INSTPTR_ov_domain) pinst, &CTree_Transport_exec_callback);

	if (!(pClient->v_state & KSBASE_CLST_ERROR))
		pinst->v_status = CTREE_COMMON_WAITINGFORANSWER;
	else
		pinst->v_status = CTREE_COMMON_INTERNALERROR;

	Ov_HeapFree(items);

	return result;

}

OV_DLLFNCEXPORT void CTree_Transport_typemethod(OV_INSTPTR_fb_functionblock pfb,
		OV_TIME *pltc) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_CTree_Transport pinst = Ov_StaticPtrCast(CTree_Transport, pfb);
	OV_RESULT result = { 0 };

	CTree_Download_typemethod(pfb, pltc);
	if (Ov_Fail(pinst->v_result)) {
		ov_logfile_error("Download failed.");
		return;
	}
	result = CTree_Transport_execute(pinst);
	if (Ov_Fail(result)) {
		ov_logfile_error("Transport failed. ");
		pinst->v_result = !!result;
		return;
	}

	return;
}

