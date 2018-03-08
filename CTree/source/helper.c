/******************************************************************************
 *
 *   FILE
 *   ----
 *   helper.c
 *
 *   History
 *   -------
 *   2018-03-02   File created
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
#include "libov/ov_path.h"

#define VERSION_FOR_CTREE	2

OV_DLLFNCEXPORT void CTree_helper_typemethod(OV_INSTPTR_fb_functionblock pfb,
		OV_TIME *pltc) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_CTree_helper pinst = Ov_StaticPtrCast(CTree_helper, pfb);

	return;
}

OV_DLLFNCEXPORT OV_STRING CTree_helper_getfactory(OV_INSTPTR_ov_domain pobj) {
	if (pobj == NULL)
		return NULL;

	ov_memstack_lock();
	OV_INSTPTR_ov_object pparent = Ov_GetParent(ov_instantiation, pobj);
	OV_INSTPTR_ov_object pparobj = Ov_PtrUpCast(ov_object, pparent);
	OV_STRING factory = ov_path_getcanonicalpath(pparobj, VERSION_FOR_CTREE);
	ov_memstack_unlock();
	return factory;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_valueToStr(OV_STRING* valuestr,
		const OV_VAR_VALUE* value) {
	OV_RESULT result = OV_ERR_OK;
	if (value->valueunion.val_string != NULL) {
		switch (value->vartype) {
		case OV_VT_BOOL:
			result = ov_string_print(valuestr, "%s",
					(value->valueunion.val_bool ? "TRUE" : "FALSE"));
			break;
		case OV_VT_BYTE:
			result = ov_string_print(valuestr, "%s",
					value->valueunion.val_byte);
			break;
		case OV_VT_INT:
			result = ov_string_print(valuestr, "%i", value->valueunion.val_int);
			break;
		case OV_VT_UINT:
			result = ov_string_print(valuestr, "%u",
					value->valueunion.val_uint); /* if vartype == OV_VT_UINT */
			break;
		case OV_VT_SINGLE:
			result = ov_string_print(valuestr, "%f",
					value->valueunion.val_single); /* if vartype == OV_VT_SINGLE */
			break;
		case OV_VT_DOUBLE:
			result = ov_string_print(valuestr, "%d",
					value->valueunion.val_double);
			break;
		case OV_VT_STRING:
			result = ov_string_print(valuestr, "%s",
					value->valueunion.val_string); /* if vartype == OV_VT_STRING */
			break;
		case OV_VT_TIME:
//		OV_TIME val_time; /* if vartype == OV_VT_TIME */
//		break;
		case OV_VT_TIME_SPAN:
//		ov_string_print(valuestr, "", value->valueunion.val_time_span);
//	case OV_VT_TIME_SPAN:
//		ov_string_print(valuestr, "", value->valueunion.val_state); /* if vartype == OV_VT_TIME_SPAN */
//	case OV_VT_STRUCT:
//		OV_STRUCT val_struct; /* if vartype == OV_VT_STRUCT */
			/* vector datatypes */
		default:
			result = ov_string_setvalue(valuestr, "-");
//		OV_BYTE_VEC val_byte_vec; /* if vartype == OV_VT_BYTE_VEC or OV_VT_CTYPE*/
//		OV_BOOL_VEC val_bool_vec; /* if vartype == OV_VT_BOOL_VEC */
//		OV_INT_VEC val_int_vec; /* if vartype == OV_VT_INT_VEC */
//		OV_UINT_VEC val_uint_vec; /* if vartype == OV_VT_UINT_VEC */
//		OV_SINGLE_VEC val_single_vec; /* if vartype == OV_VT_SINGLE_VEC */
//		OV_DOUBLE_VEC val_double_vec; /* if vartype == OV_VT_DOUBLE_VEC */
//		OV_STRING_VEC val_string_vec; /* if vartype == OV_VT_STRING_VEC */
//		OV_TIME_VEC val_time_vec; /* if vartype == OV_VT_TIME_VEC */
//		OV_TIME_SPAN_VEC val_time_span_vec; /* if vartype == OV_VT_TIME_SPAN_VEC */
//		OV_STATE_VEC val_state_vec; /* if vartype == OV_VT_TIME_SPAN */
//		OV_GENERIC_VEC val_generic_vec; /* for internal use only */
//		OV_STRUCT_VEC val_struct_vec; /* if vartype == OV_VT_STRUCT_VEC */
		}
	} else {
		result = ov_string_setvalue(valuestr, "-");
	}
	return result;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_typeToStr(OV_STRING* typestr,
		const OV_VAR_TYPE * type) {
	OV_RESULT result = OV_ERR_OK;
	switch (*type) {
	case OV_VT_BOOL:
		result = ov_string_setvalue(typestr, "BOOL");
		break;
	case OV_VT_BYTE:
		result = ov_string_setvalue(typestr, "BYTE");
		break;
	case OV_VT_CTYPE:
		result = ov_string_setvalue(typestr, "CTYPE");
		break;
	case OV_VT_DOUBLE:
		result = ov_string_setvalue(typestr, "DOUBLE");
		break;
	case OV_VT_INT:
		result = ov_string_setvalue(typestr, "INT");
		break;
	case OV_VT_POINTER:
		result = ov_string_setvalue(typestr, "POINTER");
		break;
	case OV_VT_SINGLE:
		result = ov_string_setvalue(typestr, "SINGLE");
		break;
	case OV_VT_STATE:
		result = ov_string_setvalue(typestr, "STATE");
		break;
	case OV_VT_STRING:
		result = ov_string_setvalue(typestr, "STRING");
		break;
	case OV_VT_STRUCT:
		result = ov_string_setvalue(typestr, "STRUCT");
		break;
	case OV_VT_TIME:
		result = ov_string_setvalue(typestr, "TIME");
		break;
	case OV_VT_TIME_SPAN:
		result = ov_string_setvalue(typestr, "TIME_SPAN");
		break;
	case OV_VT_UINT:
		result = ov_string_setvalue(typestr, "UINT");
		break;
	case OV_VT_VOID:
		result = ov_string_setvalue(typestr, "VOID");
		break;
	default:
		result = ov_string_print(typestr, "-");
	}
	return result;
}
