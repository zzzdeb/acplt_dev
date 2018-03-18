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
#include <stdio.h>

#define VERSION_FOR_CTREE	2

//string
OV_DLLFNCEXPORT OV_STRING CTree_helper_strlistcat(const OV_STRING_VEC* const vector) {
	OV_STRING res = NULL;
	OV_UINT len = 0;
	//	for (int i = 0; i < vector->veclen; i++)
	//		len += strlen(vector->value[i]);
	//	res = (OV_STRING) ov_memstack_alloc(len + 1);
	for (int i = 0; i < vector->veclen; i++) {
		len += strlen(vector->value[i]);
		ov_string_append(&res, vector->value[i]);
	}
	return res;
}

//value

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

//	check validity of input
	if (value == NULL || valuestr == NULL) {
		result = OV_ERR_BADPARAM;
		return result;
	}

//	check
	switch (value->vartype) {
	case OV_VT_BOOL:
		result = ov_string_print(valuestr, "%s",
				(value->valueunion.val_bool ? "TRUE" : "FALSE"));
		break;
	case OV_VT_BYTE:
		result = ov_string_print(valuestr, "%s", value->valueunion.val_byte);
		break;
	case OV_VT_INT:
		result = ov_string_print(valuestr, "%i", value->valueunion.val_int);
		break;
	case OV_VT_UINT:
		result = ov_string_print(valuestr, "%u", value->valueunion.val_uint); /* if vartype == OV_VT_UINT */
		break;
	case OV_VT_SINGLE:
		result = ov_string_print(valuestr, "%f", value->valueunion.val_single); /* if vartype == OV_VT_SINGLE */
		break;
	case OV_VT_DOUBLE:
		result = ov_string_print(valuestr, "%d", value->valueunion.val_double);
		break;
	case OV_VT_STRING:
		if (value->valueunion.val_string == NULL)
			result = ov_string_setvalue(valuestr, "-");
		else
			result = ov_string_print(valuestr, "%s",
					value->valueunion.val_string); /* if vartype == OV_VT_STRING */
		break;
	case OV_VT_TIME:
		result = ov_string_print(valuestr, "%u.%u",
				value->valueunion.val_time.secs,
				value->valueunion.val_time.usecs); /* if vartype == OV_VT_STRING */
		break;
	case OV_VT_TIME_SPAN:
		result = ov_string_print(valuestr, "%u.%u",
				value->valueunion.val_time_span.secs,
				value->valueunion.val_time_span.usecs); /* if vartype == OV_VT_STRING */
		break;
	case OV_VT_STATE:
	case OV_VT_STRUCT:
	case OV_VT_CTYPE:

//		vector
	case OV_VT_BYTE_VEC:
	case OV_VT_BOOL_VEC:
	case OV_VT_INT_VEC:
	case OV_VT_UINT_VEC:
	case OV_VT_SINGLE_VEC:
	case OV_VT_DOUBLE_VEC:
	case OV_VT_STRING_VEC:
	case OV_VT_TIME_VEC:
	case OV_VT_TIME_SPAN_VEC:

	case OV_VT_POINTER:
	case OV_VT_VOID:
		ov_string_setvalue(valuestr, "CANT");
		break;
	default:
		result = ov_string_setvalue(valuestr, "-");
	}
	return result;
}
typedef struct {
	OV_UINT veclen;
	OV_VAR_TYPE value[];
} OV_VARTYPE_VEC;

const OV_VARTYPE_VEC OV_VAR_TYPE_LIST = { .veclen = 30, .value = {
OV_VT_VOID,
OV_VT_BYTE,
OV_VT_BOOL,
OV_VT_INT,
OV_VT_UINT,
OV_VT_SINGLE,
OV_VT_DOUBLE,
OV_VT_STRING,
OV_VT_TIME,
OV_VT_TIME_SPAN,
OV_VT_STATE,
OV_VT_STRUCT,

OV_VT_CTYPE,
OV_VT_POINTER,
OV_VT_ISVECTOR,

OV_VT_BYTE_VEC,
OV_VT_BOOL_VEC,
OV_VT_INT_VEC,
OV_VT_UINT_VEC,
OV_VT_SINGLE_VEC,
OV_VT_DOUBLE_VEC,
OV_VT_STRING_VEC,
OV_VT_TIME_VEC,
OV_VT_TIME_SPAN_VEC,
OV_VT_TIME_SERIES,
OV_VT_STATE_VEC,

OV_VT_STRUCT_VEC,

OV_VT_HAS_STATE,
OV_VT_HAS_TIMESTAMP,
OV_VT_KSMASK } };

typedef struct {
	OV_UINT veclen;
	KS_VAR_TYPE value[];
} KS_VARTYPE_VEC;

const KS_VARTYPE_VEC KS_VAR_TYPE_LIST = { .veclen = 39, .value = {
KS_VT_VOID,

KS_VT_BOOL,
KS_VT_INT,
KS_VT_UINT,
KS_VT_SINGLE,
KS_VT_DOUBLE,
KS_VT_STRING,
KS_VT_TIME,
KS_VT_TIME_SPAN,
KS_VT_STATE,
KS_VT_STRUCT,

KS_VT_BYTE_VEC,
KS_VT_BOOL_VEC,
KS_VT_INT_VEC,
KS_VT_UINT_VEC,
KS_VT_SINGLE_VEC,
KS_VT_DOUBLE_VEC,
KS_VT_STRING_VEC,
KS_VT_TIME_VEC,
KS_VT_TIME_SPAN_VEC,
KS_VT_TIME_SERIES,
KS_VT_STATE_VEC,

KS_VT_BOOL_PV,
KS_VT_INT_PV,
KS_VT_UINT_PV,
KS_VT_SINGLE_PV,
KS_VT_DOUBLE_PV,
KS_VT_STRING_PV,
KS_VT_TIME_PV,
KS_VT_TIME_SPAN_PV,

KS_VT_BOOL_PV_VEC,
KS_VT_INT_PV_VEC,
KS_VT_UINT_PV_VEC,
KS_VT_SINGLE_PV_VEC,
KS_VT_DOUBLE_PV_VEC,
KS_VT_STRING_PV_VEC,
KS_VT_TIME_PV_VEC,
KS_VT_TIME_SPAN_PV_VEC,

KS_VT_ANY } };

const OV_STRING const KS_TYPE_TO_STR[] = { [KS_VT_VOID] = "VOID", [KS_VT_BOOL
		] = "BOOL", [KS_VT_INT] = "INT", [KS_VT_UINT] = "UINT", [KS_VT_SINGLE
		] = "SINGLE", [KS_VT_DOUBLE] = "DOUBLE", [KS_VT_STRING] = "STRING",
		[KS_VT_TIME] = "TIME", [KS_VT_TIME_SPAN] = "TIME_SPAN", [KS_VT_STATE
				] = "STATE", [KS_VT_STRUCT] = "STRUCT",

		[KS_VT_BYTE_VEC] = "BYTE_VEC", [KS_VT_BOOL_VEC] = "BOOL_VEC",
		[KS_VT_INT_VEC] = "INT_VEC", [KS_VT_UINT_VEC] = "UINT_VEC",
		[KS_VT_SINGLE_VEC] = "SINGLE_VEC", [KS_VT_DOUBLE_VEC] = "DOUBLE_VEC",
		[KS_VT_STRING_VEC] = "STRING_VEC", [KS_VT_TIME_VEC] = "TIME_VEC",
		[KS_VT_TIME_SPAN_VEC] = "TIME_SPAN_VEC", [KS_VT_TIME_SERIES
				] = "TIME_SERIES", [KS_VT_STATE_VEC] = "STATE_VEC",

		[KS_VT_BOOL_PV] = "BOOL_PV", [KS_VT_INT_PV] = "INT_PV", [KS_VT_UINT_PV
				] = "UINT_PV", [KS_VT_SINGLE_PV] = "SINGLE_PV", [KS_VT_DOUBLE_PV
				] = "DOUBLE_PV", [KS_VT_STRING_PV] = "STRING_PV", [KS_VT_TIME_PV
				] = "TIME_PV", [KS_VT_TIME_SPAN_PV] = "TIME_SPAN_PV",

		[KS_VT_BOOL_PV_VEC] = "BOOL_PV_VEC", [KS_VT_INT_PV_VEC] = "INT_PV_VEC",
		[KS_VT_UINT_PV_VEC] = "UINT_PV_VEC", [KS_VT_SINGLE_PV_VEC
				] = "SINGLE_PV_VEC", [KS_VT_DOUBLE_PV_VEC] = "DOUBLE_PV_VEC",
		[KS_VT_STRING_PV_VEC] = "STRING_PV_VEC", [KS_VT_TIME_PV_VEC
				] = "TIME_PV_VEC",
		[KS_VT_TIME_SPAN_PV_VEC] = "TIME_SPAN_PV_VEC",

		[KS_VT_ANY] = "ANY", };

const OV_STRING const OV_TYPE_TO_STR[] = { [OV_VT_VOID] = "VOID", [OV_VT_BYTE
		] = "BYTE", [OV_VT_BOOL] = "BOOL", [OV_VT_INT] = "INT", [OV_VT_UINT
		]= "UINT", [OV_VT_SINGLE] = "SINGLE", [OV_VT_DOUBLE] = "DOUBLE",
		[OV_VT_STRING ] = "STRING", [OV_VT_TIME] = "TIME", [OV_VT_TIME_SPAN
				] = "TIME_SPAN", [OV_VT_STATE] = "STATE", [OV_VT_STRUCT
				] = "STRUCT",

		[OV_VT_CTYPE] = "CTYPE", [OV_VT_POINTER] = "POINTER", [OV_VT_ISVECTOR
				] = "ISVECTOR",

		[OV_VT_BYTE_VEC] = "BYTE_VEC", [OV_VT_BOOL_VEC] = "BOOL_VEC",
		[OV_VT_INT_VEC] = "INT_VEC", [OV_VT_UINT_VEC]= "UINT_VEC",
		[OV_VT_SINGLE_VEC] = "SINGLE_VEC", [OV_VT_DOUBLE_VEC] = "DOUBLE_VEC",
		[OV_VT_STRING_VEC] = "STRING_VEC", [OV_VT_TIME_VEC] = "TIME_VEC",
		[OV_VT_TIME_SPAN_VEC] = "TIME_SPAN_VEC",
		[OV_VT_STATE_VEC] = "STATE_VEC", [OV_VT_STRUCT_VEC] = "STRUCT_VEC", };

enum VARTYPES {
	OV, KS
};
typedef enum VARTYPES VARTYPES;

OV_DLLFNCEXPORT OV_RESULT CTree_helper_ovtypeToStr(OV_STRING* typestr,
		const OV_VAR_TYPE * type) {
	if (OV_TYPE_TO_STR[*type] == NULL)
		return OV_ERR_BADPARAM;
	OV_RESULT res = ov_string_setvalue(typestr, OV_TYPE_TO_STR[*type]);
	return res;
}

//#define ForEachType(type)	for(int i=0; i<OV_VARTYPE_LIST.veclen; i++, type=OV_VARTYPE_LIST.value[i])

OV_DLLFNCEXPORT OV_RESULT CTree_helper_strToOVType(OV_VAR_TYPE * type,
		const OV_STRING typestr) {
	for (size_t i = 0; i < OV_VAR_TYPE_LIST.veclen; i++) {
		if (!ov_string_compare(typestr,
				OV_TYPE_TO_STR[OV_VAR_TYPE_LIST.value[i]])) {
			*type = OV_VAR_TYPE_LIST.value[i];
			return OV_ERR_OK;
		}
	}
	return OV_ERR_BADPARAM;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_strToKSType(KS_VAR_TYPE * type,
		const OV_STRING typestr) {
	for (size_t i = 0; i < KS_VAR_TYPE_LIST.veclen; i++) {
		if (!ov_string_compare(typestr,
				KS_TYPE_TO_STR[KS_VAR_TYPE_LIST.value[i]])) {
			*type = KS_VAR_TYPE_LIST.value[i];
			return OV_ERR_OK;
		}
	}
	return OV_ERR_BADPARAM;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_kstypeToStr(OV_STRING* typestr,
		const OV_VAR_TYPE * type) {

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_strToValue(OV_VAR_VALUE* value,
		const OV_STRING valuestr) {
	OV_RESULT result = OV_ERR_OK;
	//	check validity of input
	if (value == NULL || valuestr == NULL) {
		result = OV_ERR_BADPARAM;
		return result;
	}

	OV_UINT len;
	OV_STRING* tmp = NULL;
	//	check
	switch (value->vartype) {
	case OV_VT_BOOL:
		value->valueunion.val_bool = !ov_string_compare("TRUE", valuestr);
		break;
	case OV_VT_BYTE:
//			result = ov_string_print(valuestr, "%s", value->valueunion.val_byte);
		break;
	case OV_VT_INT:
		value->valueunion.val_int = atoi(valuestr);
		break;
	case OV_VT_UINT:
		value->valueunion.val_uint = atoi(valuestr);
		break;
	case OV_VT_SINGLE:
		value->valueunion.val_uint = atof(valuestr);
		break;
	case OV_VT_DOUBLE:
		value->valueunion.val_uint = atof(valuestr);
		break;
	case OV_VT_STRING:
		result = ov_string_setvalue(&value->valueunion.val_string, valuestr);
		break;
	case OV_VT_TIME:
	case OV_VT_TIME_SPAN:
		tmp = ov_string_split(valuestr, ".", &len);
		value->valueunion.val_time.secs = atoi(tmp[0]);
		value->valueunion.val_time.usecs = atoi(tmp[1]);
		break;
	case OV_VT_STATE:
	case OV_VT_STRUCT:
	case OV_VT_CTYPE:

		//		vector
	case OV_VT_BYTE_VEC:
	case OV_VT_BOOL_VEC:
	case OV_VT_INT_VEC:
	case OV_VT_UINT_VEC:
	case OV_VT_SINGLE_VEC:
	case OV_VT_DOUBLE_VEC:
	case OV_VT_STRING_VEC:
	case OV_VT_TIME_VEC:
	case OV_VT_TIME_SPAN_VEC:

	case OV_VT_POINTER:
	case OV_VT_VOID:
	default:
		return result; //TODO: revise it
	}
	return result;
}

//http
//		switch (addrp->var_current_props.value.vartype & OV_VT_KSMASK){
//			case OV_VT_BOOL:
//				if (CHECK_BOOLTRUE(newvaluematch.value[i])){
//					addrp->var_current_props.value.valueunion.val_bool = TRUE;
//				}else if (CHECK_BOOLFALSE(newvaluematch.value[i])){
//					addrp->var_current_props.value.valueunion.val_bool = FALSE;
//				}else{
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not detected as bool");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//				break;
//
//			case OV_VT_INT:
//				tempLong = strtol(newvaluematch.value[i],&endPtr,10);
//				if (endPtr == newvaluematch.value[i]) {
//					//not a number
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a number");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}else if (ERANGE == errno || tempLong > OV_VL_MAXINT || tempLong < OV_VL_MININT) {
//					//not in range
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in INT range");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//				addrp->var_current_props.value.valueunion.val_int = (OV_INT) tempLong;
//				break;
//			case OV_VT_UINT:
//				tempUlong = strtoul(newvaluematch.value[i],&endPtr,10);
//				if (endPtr == newvaluematch.value[i]) {
//					//not a number
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a number");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}else if (ERANGE == errno || tempUlong > OV_VL_MAXUINT) {
//					//not in range
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in UINT range");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//				addrp->var_current_props.value.valueunion.val_uint = (OV_UINT) tempUlong;
//				break;
//
//			case OV_VT_SINGLE:
//			case OV_VT_DOUBLE:
//				tempDouble = strtod(newvaluematch.value[i], &endPtr);
//				if (endPtr == newvaluematch.value[i]) {
//					//not a number
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a number");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}else if (ERANGE == errno) {
//					//not in range
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in range");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//				switch (addrp->var_current_props.value.vartype & OV_VT_KSMASK){
//					case OV_VT_SINGLE:
//						addrp->var_current_props.value.valueunion.val_single = (OV_SINGLE) tempDouble;
//						break;
//					default:
//						addrp->var_current_props.value.valueunion.val_double = (OV_DOUBLE) tempDouble;
//						break;
//				}
//			break;
//
//			case OV_VT_STRING:
//				//setting the content of the pointer to null,
//				//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
//				//we have a new object, so no memory is allocated and the setting to NULL is save
//				addrp->var_current_props.value.valueunion.val_string = NULL;
//				fr = ov_string_setvalue(&addrp->var_current_props.value.valueunion.val_string, newvaluematch.value[i]);
//				if (Ov_Fail(fr)){
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Setting string value failed");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				};
//				break;
//
//			case OV_VT_VOID:
//				if(ov_string_getlength(newvaluematch.value[i]) > 0){
//					fr = OV_ERR_BADTYPE;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Variable is/should be void, but a newvalue is given");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//				break;
//
//			case OV_VT_TIME:
//				fr = kshttp_asciitotime(&addrp->var_current_props.value.valueunion.val_time, newvaluematch.value[i]);
//				if (Ov_Fail(fr)){
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Setting time value failed");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				};
//				break;
//
//			case OV_VT_TIME_SPAN:
//				//can be 42.1241 or P42.123456S or -P23.42S
//				ov_string_setvalue(&Temp, newvaluematch.value[i]);
//				if(Temp[0] == 'P'){
//					stringOffset = 1;
//					isNegative = FALSE;
//				}else if(Temp[0] == '-' && Temp[1] == 'P'){
//					stringOffset = 2;
//					isNegative = TRUE;
//				}
//				tempDouble = strtod(Temp+stringOffset, &endPtr);
//				if (endPtr == Temp+stringOffset) {
//					//not a number
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a valid date");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}else if (ERANGE == errno) {
//					//not in range
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in range");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//				if(isNegative == TRUE){
//					tempDouble = -tempDouble;
//				}
//				Ov_DoubleToTimeSpan((OV_DOUBLE) tempDouble, addrp->var_current_props.value.valueunion.val_time_span);
//				break;
//
//			case OV_VT_STRUCT:
//				//deprecated as KS2.0r
//				fr = OV_ERR_NOTIMPLEMENTED;
//				kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": STRUCT is deprecated with KS2.0r");
//				ov_memstack_unlock();
//				EXEC_SETVAR_RETURN fr;
//			break;
//
//			//****************** VEC: *******************
//			/* request could be "{1}%20{10}"
//			 * split at "%20", discard the "{" via pointer arithmetic (+1)
//			 * the strtol command ignore the last "}"
//			 * with STRING_VEC we have to do some more
//			 */
//
//	/*		todo implement base64encoded set
//			case OV_VT_BYTE_VEC:
//			case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
//	*/
//
//			case OV_VT_BOOL_VEC:
//				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
//				addrp->var_current_props.value.valueunion.val_bool_vec.veclen = 0;
//				addrp->var_current_props.value.valueunion.val_bool_vec.value = NULL;
//				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_bool_vec, len, BOOL);
//				for(i = 0; i < len; i++){
//					//killing the first character
//					ov_string_setvalue(&Temp, pArgumentList[i]+1);
//					//kill the last character, now we have two null bytes at the end
//					Temp[ov_string_getlength(Temp)-1] = '\0';
//
//					if (CHECK_BOOLTRUE(Temp)){
//						addrp->var_current_props.value.valueunion.val_bool_vec.value[i] = TRUE;
//					}else if (CHECK_BOOLFALSE(Temp)){
//						addrp->var_current_props.value.valueunion.val_bool_vec.value[i] = FALSE;
//					}else{
//						//default
//						addrp->var_current_props.value.valueunion.val_bool_vec.value[i] = FALSE;
//					}
//				}
//				ov_string_freelist(pArgumentList);
//				break;
//
//			case OV_VT_INT_VEC:
//				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
//				addrp->var_current_props.value.valueunion.val_int_vec.veclen = 0;
//				addrp->var_current_props.value.valueunion.val_int_vec.value = NULL;
//				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_int_vec, len, INT);
//				for(i = 0; i < len; i++){
//					tempLong = strtol(pArgumentList[i]+1,&endPtr,10);
//					if (endPtr == pArgumentList[i]+1) {
//						//not a number
//						fr = OV_ERR_BADPARAM;
//						kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a number");
//						ov_string_freelist(pArgumentList);
//						ov_memstack_unlock();
//						EXEC_SETVAR_RETURN fr;
//					}else if (ERANGE == errno || tempLong > OV_VL_MAXINT || tempLong < OV_VL_MININT) {
//						//not in range
//						fr = OV_ERR_BADPARAM;
//						kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in INT range");
//						ov_string_freelist(pArgumentList);
//						ov_memstack_unlock();
//						EXEC_SETVAR_RETURN fr;
//					}
//					addrp->var_current_props.value.valueunion.val_int_vec.value[i] = (OV_INT) tempLong;
//				}
//				ov_string_freelist(pArgumentList);
//				break;
//
//			case OV_VT_UINT_VEC:
//				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
//				addrp->var_current_props.value.valueunion.val_uint_vec.veclen = 0;
//				addrp->var_current_props.value.valueunion.val_uint_vec.value = NULL;
//				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_uint_vec, len, UINT);
//				for(i = 0; i < len; i++){
//					tempUlong = strtoul(pArgumentList[i]+1,&endPtr,10);
//					if (endPtr == pArgumentList[i]+1) {
//						//not a number
//						fr = OV_ERR_BADPARAM;
//						kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a number");
//						ov_memstack_unlock();
//						EXEC_SETVAR_RETURN fr;
//					}else if (ERANGE == errno || tempUlong > OV_VL_MAXUINT) {
//						//not in range
//						fr = OV_ERR_BADPARAM;
//						kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in UINT range");
//						ov_memstack_unlock();
//						EXEC_SETVAR_RETURN fr;
//					}
//					addrp->var_current_props.value.valueunion.val_uint_vec.value[i] = (OV_UINT) tempUlong;
//				}
//				ov_string_freelist(pArgumentList);
//				break;
//
//			case OV_VT_SINGLE_VEC:
//			case OV_VT_DOUBLE_VEC:
//				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
//				for(i = 0; i < len; i++){
//					tempDouble = strtod(pArgumentList[i]+1, &endPtr);
//					if (endPtr == pArgumentList[i]+1) {
//						//not a number
//						fr = OV_ERR_BADPARAM;
//						kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not a number");
//						ov_string_freelist(pArgumentList);
//						ov_memstack_unlock();
//						EXEC_SETVAR_RETURN fr;
//					}else if (ERANGE == errno) {
//						//not in range
//						fr = OV_ERR_BADPARAM;
//						kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Input not in range");
//						ov_string_freelist(pArgumentList);
//						ov_memstack_unlock();
//						EXEC_SETVAR_RETURN fr;
//					}
//					switch (addrp->var_current_props.value.vartype & OV_VT_KSMASK){
//						case OV_VT_SINGLE:
//							if(i==0){
//								addrp->var_current_props.value.valueunion.val_single_vec.veclen = 0;
//								addrp->var_current_props.value.valueunion.val_single_vec.value = NULL;
//								Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_single_vec, len, SINGLE);
//							}
//							addrp->var_current_props.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) tempDouble;
//							break;
//						default:
//							if(i==0){
//								addrp->var_current_props.value.valueunion.val_double_vec.veclen = 0;
//								addrp->var_current_props.value.valueunion.val_double_vec.value = NULL;
//								Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_double_vec, len, DOUBLE);
//							}
//							addrp->var_current_props.value.valueunion.val_double_vec.value[i] = (OV_DOUBLE) tempDouble;
//							break;
//					}
//				}
//				ov_string_freelist(pArgumentList);
//				break;
//
//			case OV_VT_STRING_VEC:
//				//request could be "{hello}%20{world}"
//				pArgumentList = ov_string_split(newvaluematch.value[i], "%20", &len);
//				addrp->var_current_props.value.valueunion.val_string_vec.veclen = 0;
//				addrp->var_current_props.value.valueunion.val_string_vec.value = NULL;
//				Ov_SetDynamicVectorLength(&addrp->var_current_props.value.valueunion.val_string_vec, len, STRING);
//
//				if(*pArgumentList[i] != '{' && len > 2){
//					fr = OV_ERR_BADPARAM;
//					kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": VEC entries should be urlencoded, separated with a space and wrapped with curly brackets");
//					ov_memstack_unlock();
//					EXEC_SETVAR_RETURN fr;
//				}
//
//				for(i = 0; i < len; i++){
//					//setting the content of the pointers to null
//					//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
//					addrp->var_current_props.value.valueunion.val_string_vec.value[i] = NULL;
//					ov_memstack_lock();
//					if(*pArgumentList[i] != '{'){
//						Temp2 = pArgumentList[i];
//					}else{
//						//killing the first character aka {
//						ov_string_setvalue(&Temp, pArgumentList[i]+1);
//						//kill the last character aka }, now we have two null bytes at the end
//						Temp[ov_string_getlength(Temp)-1] = '\0';
//						//finally decode URLencoding
//						Temp2 = url_decode(Temp);
//						//Temp2 = Temp;
//					}
//					fr = ov_string_setvalue(&addrp->var_current_props.value.valueunion.val_string_vec.value[i], Temp2);
//					ov_memstack_unlock();
//				}
//				Temp2 = NULL; //had a memstack pointer only
//				ov_string_freelist(pArgumentList);
//				break;
//
//			case OV_VT_STRUCT_VEC:
//				//deprecated as KS2.0r
//				fr = OV_ERR_NOTIMPLEMENTED;
//				kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": STRUCT is deprecated with KS2.0r");
//				ov_memstack_unlock();
//				EXEC_SETVAR_RETURN fr;
//
//	/*	TODO Time* VEC
//			case OV_VT_TIME_VEC:
//
//			case OV_VT_TIME_SPAN_VEC:
//
//			case OV_VT_STATE_VEC:
//	*/
//			default:
//	/*				ov_logfile_error("%s:%d - GestureReaction - target: %s, Userinput (%s), DataType %" OV_PRINT_UINT " not implemented.", __FILE__, __LINE__,
//					*varname,
//					newvaluematch.value[i],
//					addrp->var_current_props.value.vartype);
//	*/
//				fr = OV_ERR_NOTIMPLEMENTED;
//				kshttp_print_result_array(&response->contentString, request.response_format, &fr, 1, ": Vartype not supported");
//				ov_memstack_unlock();
//				EXEC_SETVAR_RETURN fr;
//		}

