/******************************************************************************
 *
 *   FILE
 *   ----
 *   Download.c
 *
 *   History
 *   -------
 *   2018-02-27   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#define OV_DEBUG

#ifndef OV_COMPILE_LIBRARY_CTree
#define OV_COMPILE_LIBRARY_CTree
#endif

#include "CTree.h"
#include "libov/ov_macros.h"
#include "libov/ov_string.h"
#include "libov/ov_library.h"
#include "libov/ov_path.h"
#include "libov/ov_element.h"
#include "libov/ov_result.h"
#include "libov/ov_object.h"
#include "libov/ov_variable.h"

#include "ksbase.h"
#include "NoneTicketAuthenticator.h"

#include "cJSON.h"

#include "libov/ov_debug.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define VERSION_FOR_CTREE 	2

OV_RESULT crawl_tree(OV_INSTPTR_CTree_Download pinst,
		const OV_INSTPTR_ov_domain pobj, cJSON* jsobj);

//OV_RESULT get_variables(const OV_INSTPTR_ov_object pobj, cJSON* jsvars,
//		OV_ELEM_TYPE mask);

/*
 * Helpers
 */
//checks if variable is one of the variables of ov_object
OV_RESULT is_standard_variable(OV_STRING identifier);

/*
 * converts f.e. /acplt/a/b to a/b
 */
OV_STRING className_from_path(const OV_STRING factory) {
	OV_UINT len = 0;
	OV_STRING resstr = NULL;
	OV_STRING* neutral = NULL;
	ov_string_setvalue(&resstr, factory);
	neutral = ov_string_split(resstr, "/", &len);
	if (len < 2)
		return NULL;
	ov_string_print(&resstr, "%s/%s", neutral[len - 2], neutral[len - 1]);
	ov_string_freelist(neutral);
	return resstr;
}

/*
 * converts path from /a/b to ~/b if pre=/a & path=/a/b
 */
OV_STRING pathAbsToRel(const OV_STRING pre, const OV_STRING path) {
	OV_STRING resstr = NULL;
	if (path == NULL)
		return resstr;
	ov_string_setvalue(&resstr, path);
	if (pre == NULL)
		return resstr;

	OV_UINT prelen = ov_string_getlength(pre);
	OV_UINT pathlen = ov_string_getlength(path);

	if (prelen > pathlen) {
		return resstr;
	}

	*(resstr + prelen) = '\0';
	if (ov_string_compare(pre, resstr) == OV_STRCMP_EQUAL) {
		ov_string_print(&resstr, "~%s", path + prelen);
		return resstr;
	}
	ov_string_setvalue(&resstr, path);
	return resstr;
}

OV_RESULT Download_log(OV_INSTPTR_CTree_Download pinst, OV_MSG_TYPE msg_type,
		OV_RESULT result, const OV_STRING format, ...) {
	va_list args;

	char msg[1024];
	pinst->v_result = result;
	va_start(args, format);

#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);

	ov_logfile_print(msg_type, msg);
	if (msg_type == OV_MT_ERROR) {
		pinst->v_result = result;
		ov_string_print(&pinst->v_ErrorMsg, "%s ; %s ;",
				ov_result_getresulttext(result), msg);
	}
	return result;
}

OV_RESULT CTree_Download_execute(OV_INSTPTR_CTree_Download pinst) {
	OV_RESULT res = OV_ERR_OK;
	/*
	 * Initial
	 */
	pinst->v_cache.jsbase = cJSON_CreateObject();
	pinst->v_cache.jslibs = cJSON_AddArrayToObject(pinst->v_cache.jsbase,
			"Libraries");
	pinst->v_cache.jstree = cJSON_AddObjectToObject(pinst->v_cache.jsbase,
	TREENAME);

	pinst->v_cache.proot = Ov_StaticPtrCast(ov_domain,
			ov_path_getobjectpointer(pinst->v_path, VERSION_FOR_CTREE));
	cJSON_AddStringToObject(pinst->v_cache.jsbase, PATHNAME, pinst->v_path);

	//!!!check if root is accessable
	if (pinst->v_cache.proot == NULL) {
		Download_log(pinst, OV_MT_ERROR, OV_ERR_BADPARAM,
				"%s could not be found", pinst->v_path);
		return res;
	}

	/*
	 crawling
	 */
	cJSON* jsroot = cJSON_AddObjectToObject(pinst->v_cache.jstree,
			pinst->v_cache.proot->v_identifier);
	res = crawl_tree(pinst, pinst->v_cache.proot, jsroot);
	if (Ov_Fail(res)) {
		Download_log(pinst, OV_MT_ERROR, res, "Error at crawling tree.");
		return res;
	}

	/*
	 //TODO:checking libs and tree
	 */
	/*
	 Links
	 */
	/*
	 OV_ELEMENT elroot = {0};
	 elroot.elemtype = OV_ET_OBJECT;
	 elroot.pobj = Ov_StaticPtrCast(ov_object, pinst->v_cache.proot);
	 */
	/*
	 res = crawl_links(pinst, &elroot);
	 if (Ov_Fail(res)) {
	 Download_log(pinst, OV_MT_ERROR, res, "Couldnt crawl links.");
	 return res;
	 }
	 */

	char* tmpString = cJSON_Print(pinst->v_cache.jsbase);
	res = ov_string_setvalue(&pinst->v_tree, tmpString);
	free(tmpString);
	if (Ov_Fail(res)) {
		Download_log(pinst, OV_MT_ERROR, res, "Couldnt set json to v_tree.");
		return res;
	}

	char* tmpString2 = cJSON_Print(pinst->v_cache.jslibs);
	res = ov_string_setvalue(&pinst->v_libs, tmpString2);
	free(tmpString2);
	if (Ov_Fail(res)) {
		Download_log(pinst, OV_MT_ERROR, res, "Couldnt set json to v_libs.");
		return res;
	}
	return res;
}

OV_DLLFNCEXPORT void CTree_Download_typemethod(OV_INSTPTR_fb_functionblock pfb,
		OV_TIME *pltc) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_CTree_Download pinst = Ov_StaticPtrCast(CTree_Download, pfb);

	OV_RESULT res = CTree_Download_execute(pinst);
	switch (res) {
	case OV_ERR_OK:
		pinst->v_result = res;
		ov_logfile_info("Download done.");
		break;
	case OV_ERR_BADPARAM:
		pinst->v_result = res;
		ov_logfile_error("Download failed.");
		break;
	default:
		pinst->v_result = OV_ERR_GENERIC;
		ov_logfile_error("Download failed.");
	}
	cJSON_free(pinst->v_cache.jsbase);
	return;
}

OV_RESULT is_standard_variable(OV_STRING identifier) {
	return !ov_string_compare(identifier, "idL")
			|| !ov_string_compare(identifier, "idH")
			|| !ov_string_compare(identifier, "pouterobject")
			|| !ov_string_compare(identifier, "identifier")
			|| !ov_string_compare(identifier, "creationtime")
			|| !ov_string_compare(identifier, "objectstate")
			|| !ov_string_compare(identifier, "linktable");
}

OV_RESULT ovvalueToJSON(cJSON** pjsvalue, const OV_VAR_VALUE * value) {
	OV_RESULT result = OV_ERR_OK;
	OV_STRING valuestr = NULL;
	cJSON* jstmp = NULL;

	switch (value->vartype) {
	case OV_VT_VOID:
		result = OV_ERR_BADTYPE;
		break;
	case OV_VT_BYTE:
		*pjsvalue = cJSON_CreateNumber(value->valueunion.val_byte);
		break;

	case OV_VT_BOOL:
		*pjsvalue = cJSON_CreateBool(value->valueunion.val_bool);
		break;
	case OV_VT_INT:
		*pjsvalue = cJSON_CreateNumber(value->valueunion.val_int);
		break;
	case OV_VT_UINT:
		*pjsvalue = cJSON_CreateNumber(value->valueunion.val_uint);
		break;
	case OV_VT_SINGLE:
		*pjsvalue = cJSON_CreateNumber(value->valueunion.val_single);
		break;
	case OV_VT_DOUBLE:
		*pjsvalue = cJSON_CreateNumber(value->valueunion.val_double);
		break;
	case OV_VT_STRING:
		if (value->valueunion.val_string == NULL){
			*pjsvalue = cJSON_CreateNull();
			break;
		}
		*pjsvalue = cJSON_CreateString(value->valueunion.val_string);
		break;
	case OV_VT_TIME:
		*pjsvalue = cJSON_CreateArray();
		cJSON_AddItemToArray(*pjsvalue,
				cJSON_CreateNumber(value->valueunion.val_time.secs));
		cJSON_AddItemToArray(*pjsvalue,
				cJSON_CreateNumber(value->valueunion.val_time.usecs));
		break;
	case OV_VT_TIME_SPAN:
		*pjsvalue = cJSON_CreateArray();
		cJSON_AddItemToArray(*pjsvalue,
				cJSON_CreateNumber(value->valueunion.val_time_span.secs));
		cJSON_AddItemToArray(*pjsvalue,
				cJSON_CreateNumber(value->valueunion.val_time_span.usecs));
		break;
	case OV_VT_STATE:
		*pjsvalue = cJSON_CreateString(NOTSUPPORTEDTYPE);
		break;
	case OV_VT_STRUCT:
		*pjsvalue = cJSON_CreateString(NOTSUPPORTEDTYPE);
		break;

	case OV_VT_BYTE_VEC:
		result = OV_ERR_NOTIMPLEMENTED;
		break;
	case OV_VT_BOOL_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_bool_vec.veclen; i++) {
			cJSON_AddItemToArray(*pjsvalue,
					cJSON_CreateBool(
							(int) value->valueunion.val_bool_vec.value[i]));
		}
		break;
	case OV_VT_INT_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_int_vec.veclen; i++) {
			cJSON_AddItemToArray(*pjsvalue,
					cJSON_CreateNumber(
							(int) value->valueunion.val_int_vec.value[i]));
		}
		break;
	case OV_VT_UINT_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_uint_vec.veclen; i++) {
			cJSON_AddItemToArray(*pjsvalue,
					cJSON_CreateNumber(
							value->valueunion.val_uint_vec.value[i]));
		}
		break;
	case OV_VT_SINGLE_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_single_vec.veclen; i++) {
			cJSON_AddItemToArray(*pjsvalue,
					cJSON_CreateNumber(
							value->valueunion.val_single_vec.value[i]));
		}
		break;
	case OV_VT_DOUBLE_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_double_vec.veclen; i++) {
			cJSON_AddItemToArray(*pjsvalue,
					cJSON_CreateNumber(
							value->valueunion.val_double_vec.value[i]));
		}
		break;
	case OV_VT_STRING_VEC:

		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_double_vec.veclen; i++) {
			if (value->valueunion.val_string_vec.value[i])
				cJSON_AddItemToArray(*pjsvalue,
						cJSON_CreateString(
								value->valueunion.val_string_vec.value[i]));
			else
				cJSON_AddItemToArray(*pjsvalue, cJSON_CreateNull());
		}
		break;
	case OV_VT_TIME_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_time_vec.veclen; i++) {
			jstmp = cJSON_CreateArray();
			cJSON_AddItemToArray(jstmp,
					cJSON_CreateNumber(
							value->valueunion.val_time_vec.value[i].secs));
			cJSON_AddItemToArray(jstmp,
					cJSON_CreateNumber(
							value->valueunion.val_time_vec.value[i].usecs));
			cJSON_AddItemToArray(*pjsvalue, jstmp);
		}
		break;
	case OV_VT_TIME_SPAN_VEC:
		*pjsvalue = cJSON_CreateArray();
		for (OV_UINT i = 0; i < value->valueunion.val_time_span_vec.veclen;
				i++) {
			jstmp = cJSON_CreateArray();
			cJSON_AddItemToArray(jstmp,
					cJSON_CreateNumber(
							value->valueunion.val_time_span_vec.value[i].secs));
			cJSON_AddItemToArray(jstmp,
					cJSON_CreateNumber(
							value->valueunion.val_time_span_vec.value[i].usecs));
			cJSON_AddItemToArray(*pjsvalue, jstmp);
		}
		break;
	case OV_VT_TIME_SERIES:
	case OV_VT_STATE_VEC:

	case OV_VT_STRUCT_VEC:

	case OV_VT_BOOL_PV:
	case OV_VT_INT_PV:
	case OV_VT_UINT_PV:
	case OV_VT_SINGLE_PV:
	case OV_VT_DOUBLE_PV:
	case OV_VT_STRING_PV:
	case OV_VT_TIME_PV:
	case OV_VT_TIME_SPAN_PV:

	case OV_VT_BOOL_PV_VEC:
	case OV_VT_INT_PV_VEC:
	case OV_VT_UINT_PV_VEC:
	case OV_VT_SINGLE_PV_VEC:
	case OV_VT_DOUBLE_PV_VEC:
	case OV_VT_STRING_PV_VEC:
	case OV_VT_TIME_PV_VEC:
	case OV_VT_TIME_SPAN_PV_VEC:

	case OV_VT_ANY:
		result = OV_ERR_NOTIMPLEMENTED;
		return result;
	default:
		return OV_ERR_BADPARAM;
	}
	return result;
}

OV_RESULT get_ep(OV_INSTPTR_CTree_Download pinst, cJSON* jsobj,
		const OV_STRING object_path) {

	OV_GETEP_PAR params;
	OV_GETEP_RES result;
	OV_OBJ_ENGINEERED_PROPS *one_result;

	OV_TICKET* pticket = NULL;

	OV_RESULT res = OV_ERR_OK;

	cJSON* jschild = NULL;
	OV_STRING mask = NULL;
	OV_STRING var_path = NULL;

//initialize ov_string
	params.path = NULL;
	params.name_mask = NULL;

	cJSON* jsvars = cJSON_AddObjectToObject(jsobj, VARIABLESNAME);
	cJSON* jslinks = cJSON_AddObjectToObject(jsobj, LINKSNAME);

	OV_STRING factory = NULL;
	cJSON* jscurrent = NULL;
	OV_INSTPTR_ov_association passoc = NULL;
	cJSON* jsparent = NULL;
	cJSON* jslinksarray = NULL;

	OV_GETVAR_RES get_var_res = { 0, 0, NULL };
	OV_GETVAR_PAR get_var_par = { 0, NULL };

	/**
	 * Build Parameter for KS function
	 */
//TODO: check input
//path
	ov_string_setvalue(&params.path, object_path);

//mask
	if (mask) {
		ov_string_setvalue(&params.name_mask, mask);
	} else {
		ov_string_setvalue(&params.name_mask, "*");
	}
	ov_string_setvalue(&mask, NULL);
//flag
//	KS_EP_FLAGS flag;
//	if (flag) {
//		params.scope_flags = flag;
//	} else {
	params.scope_flags = KS_EPF_DEFAULT;
//	}

//	KS_OBJ_TYPE object_type = KS_OT_VARIABLE;
//	if (object_type) {
//		params.type_mask = object_type;
//	} else {
//		res = OV_ERR_BADPARAM;
////					": Requesttype not supported");
//		return res;
//	}
	params.type_mask = KS_OT_ANY;

//create NONE-ticket
	pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL,
	OV_TT_NONE);

	ov_memstack_lock(); //needed for ov_path_resolve and the class_identifier
	ov_ksserver_getep(2, pticket, &params, &result);
	ov_string_setvalue(&params.path, NULL);
	ov_string_setvalue(&params.name_mask, NULL);
	if (Ov_Fail(result.result)) {
		//getEP is only valid for one target, so this variable hold all possible errors, not only NOACCESS like the other services
		ov_memstack_unlock();
//		return Download_print_log_exit(pinst, result.result);
		return result.result;
	}

	one_result = result.pfirst;
	while (one_result != NULL) {
		//open Child item level
		if (result.pfirst != one_result) {
//		kshttp_response_parts_seperate(&temp, request.response_format);
		}
		//change target output
		switch (one_result->objtype) {
		case KS_OT_DOMAIN:
			/*
			 jschild = cJSON_AddObjectToObject(jschildren, one_result->);
			 res = crawl_tree(pinst, Ov_StaticPtrCast(ov_domain, elchild.pobj),
			 jschild);
			 if (Ov_Fail(res))
			 return res;
			 */
			break;
		case KS_OT_LINK:
//			OV_INSTPTR_ov_object pparent = NULL;
//			OV_INSTPTR_ov_object pchild = NULL;
			jscurrent = cJSON_AddObjectToObject(jslinks,
					one_result->identifier);

			factory =
					one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier;
			/*
			 check if it is useful link
			 */
			if (ov_string_match(factory, "*/ov/containment")
					|| ov_string_match(factory, "*/ov/instantiation")) {
				cJSON_DeleteItemFromObject(jslinks, one_result->identifier);
				break;
			}

			OV_STRING tmpString = className_from_path(factory);
			cJSON_AddStringToObject(jscurrent, ASSOCNAME,
					tmpString);
			ov_string_setvalue(&tmpString, NULL);

			ov_memstack_lock();
			passoc = Ov_StaticPtrCast(ov_association,
					ov_path_getobjectpointer(factory, VERSION_FOR_CTREE));
			ov_memstack_unlock();

			if (ov_string_compare(passoc->v_parentrolename,
					one_result->identifier)) {
				cJSON* jscurrent1 = cJSON_AddArrayToObject(jscurrent,
				LINKPARENTSNAME);
				cJSON_AddItemToArray(jscurrent1, cJSON_CreateString(LINKTHIS));
				jslinksarray = cJSON_AddArrayToObject(jscurrent,
				LINKCHILDRENNAME);
			} else {
				jslinksarray = cJSON_AddArrayToObject(jscurrent,
				LINKPARENTSNAME);
				cJSON* jscurrent1 = cJSON_AddArrayToObject(jscurrent,
				LINKCHILDRENNAME);
				cJSON_AddItemToArray(jscurrent1, cJSON_CreateString(LINKTHIS));
			}

			ov_string_print(&var_path, "%s.%s", object_path,
					one_result->identifier);
			get_var_par.identifiers_len = 1;
			get_var_par.identifiers_val = &var_path;
			ov_ksserver_getvar(VERSION_FOR_CTREE, pticket, &get_var_par,
					&get_var_res);

//			OV_STRING value = NULL;
			for (unsigned i = 0; i < get_var_res.items_len; i++) {
				OV_VAR_VALUE value =
						get_var_res.items_val[i].var_current_props.value;
				if (value.vartype == OV_VT_STRING){
					OV_STRING tmpString = pathAbsToRel(pinst->v_path,
							value.valueunion.val_string);
					cJSON_AddItemToArray(jslinksarray,
							cJSON_CreateString(tmpString
									));
					ov_string_setvalue(&tmpString, NULL);
				}
				if (get_var_res.items_val[i].var_current_props.value.vartype
						== OV_VT_STRING_VEC)
					for (unsigned j = 0;
							j
									< get_var_res.items_val[i].var_current_props.value.valueunion.val_string_vec.veclen;
							j++){
						OV_STRING tmpString = pathAbsToRel(pinst->v_path,
													value.valueunion.val_string);
						cJSON_AddItemToArray(jslinksarray,
								cJSON_CreateString(
										tmpString));
						ov_string_setvalue(&tmpString, NULL);
					}
			}

			/*
			 clearing empty links
			 */
			if (!cJSON_GetArraySize(jslinksarray))
				cJSON_DeleteItemFromObject(jslinks, one_result->identifier);
			break;
		case KS_OT_VARIABLE:
			if (is_standard_variable(one_result->identifier))
				break;

			/*
			 * 			Accessing information
			 CTree_helper_accessToStr(&accessstr, &one_result->access);
			 cJSON_AddItemToArray(jschild,
			 cJSON_CreateString(accessstr));
			 */

			OV_GETVAR_RES get_var_res = { 0, 0, NULL };

			ov_string_print(&var_path, "%s.%s", object_path,
					one_result->identifier);
			get_var_par.identifiers_len = 1;
			get_var_par.identifiers_val = &var_path;
			ov_ksserver_getvar(VERSION_FOR_CTREE, pticket, &get_var_par,
					&get_var_res);
			if (Ov_Fail(get_var_res.result)) {
				ov_logfile_error("%s: from ks : %s",
						ov_result_getresulttext(get_var_res.result), var_path);
				break;
			}
			if (Ov_Fail(get_var_res.items_val[0].result)) {
				ov_logfile_error("%s: from ks :%s",
						ov_result_getresulttext(
								get_var_res.items_val[0].result), var_path);
				break;
			}

			jschild = cJSON_AddArrayToObject(jsvars, one_result->identifier);
			//TODO: force to standard order
			/*
			 getting type as string
			 */
			OV_STRING typestr = NULL;
			//			ov_string_setvalue(&typestr, "1");
			res =
					ov_string_setvalue(&typestr,
							CTree_helper_ovtypeToStr(
									get_var_res.items_val[0].var_current_props.value.vartype));
			if (Ov_OK(res)) {
				cJSON_AddItemToArray(jschild, cJSON_CreateString(typestr));
			} else {
				//TODO: Erase it
				cJSON_AddItemToArray(jschild, cJSON_CreateString("error"));
				break;
			}
			ov_string_setvalue(&typestr, NULL);
			/*
			 getting value as string
			 */
			cJSON* jsvalue = NULL;
			res = ovvalueToJSON(&jsvalue,
					&get_var_res.items_val[0].var_current_props.value);
			if (Ov_OK(res))
				cJSON_AddItemToArray(jschild, jsvalue);
			else {
				ov_logfile_error("%s: %s", ov_result_getresulttext(res),
						var_path);
				res = OV_ERR_OK; //TODO erase it;
			}
			break;

			/*
			 res = CTree_helper_valueTojsStr(&jsvalue,
			 &get_var_res.items_val[0].var_current_props.value);
			 if (Ov_OK(res)) {
			 if (valuestr == NULL) {

			 does not save empty value

			 cJSON_DeleteItemFromObject(jsvars, one_result->identifier);
			 break;
			 };
			 cJSON_AddItemToArray(jschild, cJSON_CreateString(valuestr));
			 } else {
			 cJSON_AddItemToArray(jschild, cJSON_CreateString("error"));
			 }*/
			break;
		case OV_OT_HISTORY:
			break;
		case OV_OT_STRUCTURE:
			break;
		case OV_OT_ANY:
			break;
		default:
			break;
		}

		if (Ov_Fail(res)) {
//		//should not happen with an UINT
//		ov_string_append(&response->contentString, "internal memory problem");
//		res = OV_ERR_GENERIC;
//		kshttp_print_result_array(&message, request.response_format, &res, 1,
//				": internal memory problem");
			ov_string_setvalue(&var_path, NULL);
			return res; //503
		}

		one_result = one_result->pnext;
	}
	ov_memstack_unlock();
	/*	delete Ticket	*/
	pticket->vtbl->deleteticket(pticket);

	/*
	 deleting empty links
	 */
	if (!cJSON_GetArraySize(jslinks))
		cJSON_DeleteItemFromObject(jsobj, LINKSNAME);
	if (!cJSON_GetArraySize(jsvars))
		cJSON_DeleteItemFromObject(jsobj, VARIABLESNAME);
	ov_string_setvalue(&var_path, NULL);
	return res;
}

/*OV_RESULT get_variables(const OV_INSTPTR_ov_object pobj, cJSON* jsvars,
 OV_ELEM_TYPE mask) {
 OV_RESULT res = OV_ERR_OK;

 //	OV_INSTPTR_ov_object pchild;
 OV_STRING identifier = NULL;

 OV_ELEMENT elparent;
 elparent.elemtype = OV_ET_OBJECT;
 elparent.pobj = pobj;

 OV_VTBLPTR_ov_object pVtblObj = NULL;

 OV_ELEMENT elvar;
 elvar.elemtype = OV_ET_NONE;

 OV_ANY value;
 cJSON* jschild = NULL;

 Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
 while (TRUE) {
 //get variable element
 res = ov_element_getnextpart(&elparent, &elvar, mask);
 if (Ov_Fail(res)) {
 ov_logfile_error("%s", ov_result_getresulttext(res));
 return res;
 }
 if (elvar.elemtype == OV_ET_NONE)
 break;

 if (elvar.elemtype == mask)
 res = pVtblObj->m_getvar(elvar.pobj, &elvar, &value);

 switch (mask) {
 case OV_ET_NONE: invalid element
 case OV_ET_OBJECT:
 case OV_ET_MEMBER:  structure member (superseded by PART)
 case OV_ET_OPERATION:
 case OV_ET_ANY:
 ov_logfile_info("Cannot handle it for now");
 break;
 case OV_ET_VARIABLE:

 ov_string_setvalue(&identifier, ov_element_getidentifier(&elvar));
 if (is_standard_variable(identifier))
 break;

 jschild = cJSON_AddArrayToObject(jsvars, identifier);
 //			ov_variable_initialvalue_get();
 // access = pVtblObj->m_getaccess(pobj, pelem, pticket);
 cJSON_AddItemToArray(jschild, cJSON_CreateString("1"));
 OV_STRING valuestr = NULL;
 //			ov_string_setvalue(&valuestr, "1");
 res = CTree_helper_valueToStr(&valuestr, &value.value);
 if (Ov_OK(res)) {
 cJSON_AddItemToArray(jschild, cJSON_CreateString(valuestr));
 } else {
 cJSON_AddItemToArray(jschild, cJSON_CreateString("error"));
 }

 OV_STRING typestr;
 //			ov_string_setvalue(&typestr, "1");
 res = CTree_helper_ovtypeToStr(&typestr, &value.value.vartype);
 if (Ov_OK(res)) {
 cJSON_AddItemToArray(jschild, cJSON_CreateString(typestr));
 } else {
 cJSON_AddItemToArray(jschild, cJSON_CreateString("error"));
 }
 break;
 case OV_ET_PARENTLINK:
 jschild = cJSON_AddArrayToObject(jsvars,
 ov_element_getidentifier(&elvar));

 cJSON_AddItemToArray(jschild, cJSON_CreateString("parentlink"));

 cJSON_AddItemToArray(jschild,
 cJSON_CreateString(
 elvar.elemunion.passoc->v_parentrolename));
 cJSON_AddItemToArray(jschild,
 cJSON_CreateString(
 elvar.elemunion.passoc->v_childrolename));

 ov_memstack_lock();
 cJSON_AddItemToArray(jschild,
 cJSON_CreateString(
 ov_path_getcanonicalpath(
 Ov_PtrUpCast(ov_object,
 elvar.elemunion.passoc),
 VERSION_FOR_CTREE)));
 ov_memstack_unlock();
 break;
 case OV_ET_CHILDLINK:
 jschild = cJSON_AddArrayToObject(jsvars,
 ov_element_getidentifier(&elvar));

 cJSON_AddItemToArray(jschild, cJSON_CreateString("childlink"));

 cJSON_AddItemToArray(jschild,
 cJSON_CreateString(
 elvar.elemunion.passoc->v_parentrolename));
 cJSON_AddItemToArray(jschild,
 cJSON_CreateString(
 elvar.elemunion.passoc->v_childrolename));

 ov_memstack_lock();
 cJSON_AddItemToArray(jschild,
 cJSON_CreateString(
 ov_path_getcanonicalpath(
 Ov_PtrUpCast(ov_object,
 elvar.elemunion.passoc),
 VERSION_FOR_CTREE)));
 ov_memstack_unlock();
 }
 }

 return res;
 }*/

OV_RESULT crawl_tree(OV_INSTPTR_CTree_Download pinst,
		const OV_INSTPTR_ov_domain pobj, cJSON* jsobj) {
	cJSON* jslibs = pinst->v_cache.jslibs;
	OV_RESULT res = 0;

	cJSON* current = NULL;

//debug
	ov_memstack_lock();

	/*
	 variables
	 */
	if (pinst->v_getVar) {
//		current = cJSON_AddObjectToObject(jsobj, VARIABLESNAME);
//		get_variables(Ov_PtrUpCast(ov_object, pobj), current, OV_ET_VARIABLE);
		get_ep(pinst, jsobj,
				ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj),
				VERSION_FOR_CTREE));
	}
	ov_memstack_unlock();

//factory


//	libraries
	OV_STRING factory = CTree_helper_getfactory(pobj);
	OV_STRING tmpString = className_from_path(factory);
	cJSON_AddStringToObject(jsobj, FACTORYNAME, tmpString);
	ov_string_setvalue(&tmpString, NULL);
	OV_UINT len = 0;
	OV_STRING* seperated = ov_string_split(factory, "/", &len);
	ov_string_setvalue(&factory, NULL);
	if (seperated == NULL)
		return OV_ERR_BADVALUE;

	if (len < 2)
		return OV_ERR_BADVALUE;

//	checks if the library already found.
	current = cJSON_GetObjectItem(jslibs, seperated[len - 2]);
	if (current == NULL)
		cJSON_AddStringToObject(jslibs, seperated[len - 2], seperated[len - 2]);
	ov_string_freelist(seperated);
	/*
	 variables & children & links
	 */
//	get_ep(pinst, jsobj, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj), VERSION_FOR_CTREE));
	/*
	 children
	 */
	cJSON* jschildren = cJSON_AddObjectToObject(jsobj, CHILDRENNAME);
	cJSON* jsparts = cJSON_AddObjectToObject(jsobj, PARTSNAME);

	OV_ELEMENT objElement;
	objElement.elemtype = OV_ET_OBJECT;
	objElement.pobj = Ov_PtrUpCast(ov_object, pobj);

	OV_ELEMENT elchild;
	elchild.elemtype = OV_ET_NONE;
	ov_element_getnextchild(&objElement, &elchild);
	while (elchild.elemtype == OV_ET_OBJECT) {
//		ov_logfile_info("%s", ov_element_getidentifier(&elchild));

		/*
		 self skipping
		 */
		if (elchild.pobj == Ov_StaticPtrCast(ov_object, pinst)) {
			ov_element_getnextchild(&objElement, &elchild);
			continue;
		}

		cJSON* jschild = cJSON_AddObjectToObject(jschildren,
				ov_element_getidentifier(&elchild));
		res = crawl_tree(pinst, Ov_StaticPtrCast(ov_domain, elchild.pobj),
				jschild);
		if (Ov_Fail(res)) {
			return res;
		}
		ov_element_getnextchild(&objElement, &elchild);
	}
	/*
	 * parts
	 */
	OV_ELEM_TYPE filter = OV_ET_OBJECT | OV_ET_MEMBER;
	elchild.elemtype = OV_ET_NONE;
	ov_element_getnextpart(&objElement, &elchild, filter);
	while (elchild.elemtype == OV_ET_OBJECT) {
//		ov_logfile_info("%s", ov_element_getidentifier(&elchild));

		/*
		 self skipping
		 */
		if (elchild.pobj == Ov_StaticPtrCast(ov_object, pinst)) {
			ov_element_getnextpart(&objElement, &elchild, filter);
			continue;
		}

		cJSON* jspart = cJSON_AddObjectToObject(jsparts,
				ov_element_getidentifier(&elchild));
		res = crawl_tree(pinst, Ov_StaticPtrCast(ov_domain, elchild.pobj),
				jspart);
		if (Ov_Fail(res)) {
			return res;
		}
		ov_element_getnextpart(&objElement, &elchild, filter);
	}

	/*
	 deleting empty children and parts
	 */
	if (!cJSON_GetArraySize(jschildren))
		cJSON_DeleteItemFromObject(jsobj, CHILDRENNAME);
	if (!cJSON_GetArraySize(jsparts))
		cJSON_DeleteItemFromObject(jsobj, PARTSNAME);

	return res;
}
