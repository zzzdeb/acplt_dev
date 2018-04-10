/******************************************************************************
 *
 *   FILE
 *   ----
 *   client.c
 *
 *   History
 *   -------
 *   2018-04-01   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_eks
#define OV_COMPILE_LIBRARY_eks
#endif

#include "eks.h"
#include "libov/ov_macros.h"
#include "libov/ov_ov.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "libov/ov_result.h"

#include "ksbase.h"
#include "ksbase_helper.h"

#define EKS_COMMON_INTERNALERROR			128
#define EKS_COMMON_EXTERNALERROR			64
#define EKS_COMMON_INITIAL					0
#define EKS_COMMON_WAITINGFORANSWER			1
#define EKS_COMMON_REQUESTCOMPLETED			2

/*
 * ov functions
 */
OV_DLLFNCEXPORT void eks_client_startup(OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_eks_client pinst = Ov_StaticPtrCast(eks_client, pobj);

	/* do what the base class does first */
	ksxdr_xdrClient_startup(pobj);

	/* do what */

	return;
}

OV_DLLFNCEXPORT void eks_client_shutdown(OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_eks_client pinst = Ov_StaticPtrCast(eks_client, pobj);

	/* do what */

	/* set the object's state to "shut down" */
	ksxdr_xdrClient_shutdown(pobj);

	return;
}

/*
 * set param
 */
OV_DLLFNCEXPORT OV_RESULT eks_client_setParam(OV_INSTPTR_eks_client pobj,
		const OV_STRING serverHost, const OV_STRING serverName) {
	OV_RESULT result = 0;

	if (!serverHost) {
		ov_logfile_error("%s: no serverHost set. aborting", pobj->v_identifier);
		pobj->v_status = EKS_COMMON_INTERNALERROR;
		pobj->v_result = OV_ERR_BADPARAM;
		return OV_ERR_BADPARAM;
	}

	result = ksbase_ClientBase_serverHost_set(
			(OV_INSTPTR_ksbase_ClientBase) pobj, serverHost);
	if (Ov_Fail(result)) {
		ov_logfile_error("%s: submit: could not set serverHost at Client",
				pobj->v_identifier);
		pobj->v_status = EKS_COMMON_INTERNALERROR;
		pobj->v_result = result;
		return result;
	}
	if (serverName) {
		result = ksbase_ClientBase_serverName_set(
				(OV_INSTPTR_ksbase_ClientBase) pobj, serverName);
		if (Ov_Fail(result)) {
			ov_logfile_error("%s: submit: could not set serverName at Client",
					pobj->v_identifier);
			pobj->v_status = EKS_COMMON_INTERNALERROR;
			pobj->v_result = result;
			return result;
		}
	}
	return OV_ERR_OK;
}

/*
 * Getvar
 */

OV_DLLFNCEXPORT void eks_client_getVar(OV_INSTPTR_eks_client pobj,
		const OV_GETVAR_PAR * gvParams, OV_GETVAR_RES * gvResult) {
	/*
	 *	local variables
	 */
	OV_RESULT result = OV_ERR_OK;
	OV_UINT numberOfItems = gvParams->identifiers_len;
	OV_GETVAR_ITEM *pitem = NULL;

	OV_INSTPTR_ksbase_ClientBase pClient = Ov_StaticPtrCast(ksbase_ClientBase,
			pobj);
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;

	Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

	/*
	 *	if there are no items, we are done
	 */
	if (!numberOfItems) {
		return;
	}

	pobj->v_pGetVarRes = gvResult;

	result = pVtblClient->m_requestGetVar(pClient, NULL, numberOfItems,
			gvParams->identifiers_val, (OV_INSTPTR_ov_domain) pobj,
			&eks_client_getVarCallback);
	if(Ov_Fail(result)){
		ov_logfile_error("%s", ov_result_getresulttext(result));
	}

	if (!(pClient->v_state & KSBASE_CLST_ERROR))
		pobj->v_status = EKS_COMMON_WAITINGFORANSWER;
	else
		pobj->v_status = EKS_COMMON_INTERNALERROR;
	/*
	 *	we are finished.
	 */
	return;
}

OV_DLLFNCEXPORT void eks_client_setParAndGetvar(OV_INSTPTR_eks_client pobj,
		OV_STRING serverHost, OV_STRING serverName,
		const OV_GETVAR_PAR * gvParam, OV_GETVAR_RES * gvResult) {
	eks_client_setParam(pobj, serverHost, serverName);
	eks_client_getVar(pobj, gvParam, gvResult);
	return;
}

OV_DLLFNCEXPORT void eks_client_getVarCallback(const OV_INSTPTR_ov_domain this,
		const OV_INSTPTR_ov_domain that) {
	OV_INSTPTR_eks_client pinst = Ov_StaticPtrCast(eks_client, this);
	OV_INSTPTR_ksbase_ClientBase pClient = Ov_StaticPtrCast(ksbase_ClientBase,
			that);
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
	OV_UINT itemsLength;
	OV_GETVAR_ITEM* itemsVals = NULL;
//	OV_GETVAR_RES
	OV_RESULT result;

	if (!this || !that) {
		ov_logfile_error("callback issued with NULL pointers. aborting.");
		return;
	}

	Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

	if (!pVtblClient) {
		ov_logfile_error(
				"%s callback: could not determine Vtable of Client %s. aborting",
				this->v_identifier, that->v_identifier);
		pinst->v_status = EKS_COMMON_INTERNALERROR;
		pinst->v_result = OV_ERR_BADOBJTYPE;
		return;
	}
	ov_memstack_lock();
	result = pVtblClient->m_processGetVar(pClient, NULL,
			(OV_RESULT*) &(pinst->v_result), &itemsLength, &itemsVals);
	if (Ov_Fail(result)) {
		pinst->v_status = EKS_COMMON_INTERNALERROR;
		pinst->v_result = result;
		ov_memstack_unlock();
		return;
	}

	pinst->v_status = EKS_COMMON_REQUESTCOMPLETED;

	/*
	 *	allocate memory for the result items
	 */
	pinst->v_pGetVarRes->result = result;
	pinst->v_pGetVarRes->items_len = itemsLength;
	pinst->v_pGetVarRes->items_val = (OV_GETVAR_ITEM*) ov_memstack_alloc(
			itemsLength * sizeof(OV_GETVAR_ITEM));
	if (!pinst->v_pGetVarRes->items_val) {
		pinst->v_pGetVarRes->items_val->result = OV_ERR_TARGETGENERIC;
		return;
	}
	for (OV_UINT i = 0; i < itemsLength; i++) {
		pinst->v_pGetVarRes->items_val[i].result = itemsVals[i].result;
		result = Ov_SetAnyValue(
				&(pinst->v_pGetVarRes->items_val[i].var_current_props),
				&(itemsVals[i].var_current_props));
	}

//	pinst->v_pGetVarRes = NULL;
	ov_memstack_unlock();
	return;
}

/*
 * Getvar
 */

OV_DLLFNCEXPORT void eks_client_setVar(OV_INSTPTR_eks_client pinst,
		const OV_SETVAR_PAR * svParam, OV_SETVAR_RES * svResult) {
	OV_INSTPTR_ksbase_ClientBase pClient = (OV_INSTPTR_ksbase_ClientBase) pinst;
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
	Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

	OV_RESULT result = OV_ERR_OK;
	OV_UINT numberOfItems = svParam->items_len;

	/* svResult*/
	pinst->v_pSetVarRes = svResult;

	/*	do the actual submit	*/
	result = pVtblClient->m_requestSetVar(pClient, NULL, numberOfItems,
			svParam->items_val, (OV_INSTPTR_ov_domain) pinst,
			&eks_client_setVarCallback);
	if(Ov_Fail(result)){
		ov_logfile_error("%s", ov_result_getresulttext(result));
	}

	if (!(pClient->v_state & KSBASE_CLST_ERROR))
		pinst->v_status = EKS_COMMON_WAITINGFORANSWER;
	else
		pinst->v_status = EKS_COMMON_INTERNALERROR;

	return;
}

OV_DLLFNCEXPORT void eks_client_setParAndSetvar(OV_INSTPTR_eks_client pobj,
		OV_STRING serverHost, OV_STRING serverName,
		const OV_SETVAR_PAR * svParam, OV_SETVAR_RES * svResult) {
	eks_client_setParam(pobj, serverHost, serverName);
	eks_client_setVar(pobj, svParam, svResult);
	return;
}

OV_DLLFNCEXPORT void eks_client_setVarCallback(const OV_INSTPTR_ov_domain this,
		const OV_INSTPTR_ov_domain that) {
	OV_INSTPTR_eks_client pinst = Ov_StaticPtrCast(eks_client, this);
	OV_INSTPTR_ksbase_ClientBase pClient = Ov_StaticPtrCast(ksbase_ClientBase,
			that);
	OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;

	OV_UINT itemsLength = 0;
	OV_RESULT* itemsResults = NULL;
	OV_RESULT result;

	if (!this || !that) {
		ov_logfile_error("callback issued with NULL pointers. aborting.");
		return;
	}

	Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

	if (!pVtblClient) {
		ov_logfile_error(
				"%s callback: could not determine Vtable of Client %s. aborting",
				this->v_identifier, that->v_identifier);
		pinst->v_status = EKS_COMMON_INTERNALERROR;
		pinst->v_result = OV_ERR_BADOBJTYPE;
		return;
	}
	ov_memstack_lock();
	result = pVtblClient->m_processSetVar(pClient, NULL,
			(OV_RESULT*) &(pinst->v_result), &itemsLength, &itemsResults);
	if (Ov_Fail(result)) {
		pinst->v_status = EKS_COMMON_INTERNALERROR;
		pinst->v_result = result;
		ov_memstack_unlock();
		return;
	}

	if (Ov_Fail(pinst->v_result)) {
		pinst->v_status = EKS_COMMON_EXTERNALERROR;
		ov_memstack_unlock();
		return;
	}

	pinst->v_pSetVarRes->result = result;
	pinst->v_pSetVarRes->results_len = itemsLength;
	pinst->v_pSetVarRes->results_val = (OV_RESULT*) ov_memstack_alloc(
			itemsLength * sizeof(OV_RESULT));
	if (!pinst->v_pSetVarRes->results_val) {
		pinst->v_pSetVarRes->result = OV_ERR_TARGETGENERIC;
		return;
	}

	for (OV_UINT i = 0; i < itemsLength; i++) {
		pinst->v_pSetVarRes->results_val[i] = itemsResults[i];
	}

	pinst->v_status = EKS_COMMON_REQUESTCOMPLETED;
	pinst->v_pSetVarRes = NULL;
	ov_memstack_unlock();
	return;
}

