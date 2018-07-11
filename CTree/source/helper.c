/******************************************************************************
 *
 *   FILE
 *   ----
 *   helper.c
 *
 *   History
 *   -------
 *   2018-03-02   File created by Zolboo Erdenebayar
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
#include "ksbase.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include <stdio.h>
#include <string.h>

#define VERSION_FOR_CTREE 2
/*
 * String
 */
OV_DLLFNCEXPORT OV_STRING
CTree_helper_strlistcat(const OV_STRING_VEC *const vector) {
  OV_STRING res = NULL;
  OV_UINT len = 0;
  //	for (int i = 0; i < vector->veclen; i++)
  //		len += strlen(vector->value[i]);
  //	res = (OV_STRING) ov_memstack_alloc(len + 1);

  for (int i = 0; i < vector->veclen; i++) {
    len += ov_string_getlength(vector->value[i]);
    ov_string_append(&res, vector->value[i]);
  }
  return res;
}
/*
 * Functions
 */
OV_DLLFNCEXPORT OV_STRING CTree_helper_getfactory(OV_INSTPTR_ov_domain pobj) {
  if (pobj == NULL)
    return NULL;

  OV_STRING factory = NULL;
  ov_memstack_lock();
  OV_INSTPTR_ov_object pparobj =
      Ov_PtrUpCast(ov_object, Ov_GetParent(ov_instantiation, pobj));
  OV_STRING tmp = ov_path_getcanonicalpath(pparobj, VERSION_FOR_CTREE);
  ov_memstack_unlock();
  ov_string_setvalue(&factory, tmp);
  return factory;
}

OV_RESULT parse_kspath(const OV_STRING kspath, OV_STRING *serverHost,
                       OV_STRING *port, OV_STRING *serverName,
                       OV_STRING *path) {
  OV_RESULT result = OV_ERR_OK;

  if (kspath == NULL)
    return OV_ERR_BADPARAM;

  OV_STRING serverNameStr = NULL;
  OV_STRING pathStr = NULL;

  OV_STRING kspathcopy = NULL;
  ov_string_setvalue(&kspathcopy, kspath);
  OV_STRING portStr = strchr(kspathcopy, ':');
  if (portStr) {
    *portStr = 0;
    portStr++;
    serverNameStr = strchr(portStr, '/');
  } else {
    serverNameStr = strchr(kspathcopy, '/');
  }
  if (serverNameStr) {
    *serverNameStr = 0;
    serverNameStr++;
  }

  if (serverNameStr) {
    pathStr = strchr(serverNameStr, '/');
    if (pathStr) {
      *pathStr = 0;
      pathStr++;
    }
  } else {
    pathStr = NULL;
  }

  //	if (len==){
  //		ov_string_freelist(splited);
  //		return OV_ERR_BADPARAM;
  //	}
  //
  if (serverHost && !result) {
    result = ov_string_setvalue(serverHost, kspathcopy);
    if (Ov_Fail(result)) {
      ov_string_setvalue(&kspathcopy, NULL);
      result = OV_ERR_GENERIC;
    }
  }

  if (port && !result) {
    result = ov_string_setvalue(port, portStr);
    ;
    if (Ov_Fail(result)) {
      result = OV_ERR_GENERIC;
    }
  }

  if (serverName && !result) {
    result = ov_string_setvalue(serverName, serverNameStr);
    if (Ov_Fail(result)) {
      result = OV_ERR_GENERIC;
    }
  }

  if (path && !result) {
    result = ov_string_setvalue(path, pathStr);
    if (Ov_Fail(result)) {
      result = OV_ERR_GENERIC;
    }
  }
  ov_string_setvalue(&kspathcopy, NULL);
  ov_string_setvalue(&portStr, NULL);
  ov_string_setvalue(&serverNameStr, NULL);
  ov_string_setvalue(&pathStr, NULL);
  return result;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_setKSParam(
    const OV_INSTPTR_ksbase_ClientBase pclient, const OV_STRING param) {
  OV_RESULT result = OV_ERR_OK;
  OV_STRING serverHost = NULL;
  OV_STRING serverPort = NULL;
  OV_STRING serverName = NULL;
  OV_STRING serverPath = NULL;
  result =
      parse_kspath(param, &serverHost, &serverPort, &serverName, &serverPath);

  if (!serverHost) {
    return OV_ERR_BADPARAM;
  }

  result = ksbase_ClientBase_serverHost_set(
      (OV_INSTPTR_ksbase_ClientBase)pclient, serverHost);
  if (Ov_Fail(result)) {
    return result;
  }
  if (serverName) {
    result = ksbase_ClientBase_serverName_set(
        (OV_INSTPTR_ksbase_ClientBase)pclient, serverName);
    if (Ov_Fail(result)) {
      return result;
    }
  }

  return OV_ERR_OK;
}

/*
 *  Value
 */

typedef struct {
  OV_UINT veclen;
  OV_VAR_TYPE value[];
} OV_VARTYPE_VEC;

const OV_VARTYPE_VEC OV_VAR_TYPE_LIST = {
    .veclen = 30,
    .value = {OV_VT_VOID,        OV_VT_BYTE,          OV_VT_BOOL,
              OV_VT_INT,         OV_VT_UINT,          OV_VT_SINGLE,
              OV_VT_DOUBLE,      OV_VT_STRING,        OV_VT_TIME,
              OV_VT_TIME_SPAN,   OV_VT_STATE,         OV_VT_STRUCT,

              OV_VT_CTYPE,       OV_VT_POINTER,       OV_VT_ISVECTOR,

              OV_VT_BYTE_VEC,    OV_VT_BOOL_VEC,      OV_VT_INT_VEC,
              OV_VT_UINT_VEC,    OV_VT_SINGLE_VEC,    OV_VT_DOUBLE_VEC,
              OV_VT_STRING_VEC,  OV_VT_TIME_VEC,      OV_VT_TIME_SPAN_VEC,
              OV_VT_TIME_SERIES, OV_VT_STATE_VEC,

              OV_VT_STRUCT_VEC,

              OV_VT_HAS_STATE,   OV_VT_HAS_TIMESTAMP, OV_VT_KSMASK}};

typedef struct {
  OV_UINT veclen;
  KS_VAR_TYPE value[];
} KS_VARTYPE_VEC;

const KS_VARTYPE_VEC KS_VAR_TYPE_LIST = {.veclen = 39,
                                         .value = {KS_VT_VOID,

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

                                                   KS_VT_ANY}};


/*	----------------------------------------------------------------------
 */
/*
 *	Return value type of getAccessor
 */
OV_DLLFNCEXPORT OV_STRING CTree_helper_ovtypeToStr(OV_VAR_TYPE vartype) {
  switch (vartype) {
  case OV_VT_VOID:
    return "VOID";
  case OV_VT_BYTE:
    return "BYTE";
  case OV_VT_BOOL:
    return "BOOL";
  case OV_VT_INT:
    return "INT";
  case OV_VT_UINT:
    return "UINT";
  case OV_VT_SINGLE:
    return "SINGLE";
  case OV_VT_DOUBLE:
    return "DOUBLE";
  case OV_VT_STRING:
    return "STRING";
  case OV_VT_TIME:
    return "TIME";
  case OV_VT_TIME_SPAN:
    return "TIME_SPAN";
  case OV_VT_STATE:
    return "OV_STATE";
  case OV_VT_STRUCT:
    return "OV_STRUCT";

  case OV_VT_BYTE_VEC:
    return "BYTE_VEC";
  case OV_VT_BOOL_VEC:
    return "BOOL_VEC";
  case OV_VT_INT_VEC:
    return "INT_VEC";
  case OV_VT_UINT_VEC:
    return "UINT_VEC";
  case OV_VT_SINGLE_VEC:
    return "SINGLE_VEC";
  case OV_VT_DOUBLE_VEC:
    return "DOUBLE_VEC";
  case OV_VT_STRING_VEC:
    return "STRING_VEC";
  case OV_VT_TIME_VEC:
    return "TIME_VEC";
  case OV_VT_TIME_SPAN_VEC:
    return "TIME_SPAN_VEC";
  case OV_VT_TIME_SERIES:
    return "TIME_SERIES";
  case OV_VT_STATE_VEC:
    return "STATE_VEC";

  case OV_VT_STRUCT_VEC:
    return "STRUCT_VEC";

  case OV_VT_BOOL_PV:
    return "BOOL_PV";
  case OV_VT_INT_PV:
    return "INT_PV";
  case OV_VT_UINT_PV:
    return "UINT_PV*";
  case OV_VT_SINGLE_PV:
    return "SINGLE_PV";
  case OV_VT_DOUBLE_PV:
    return "DOUBLE_PV";
  case OV_VT_STRING_PV:
    return "STRING_PV";
  case OV_VT_TIME_PV:
    return "TIME_PV";
  case OV_VT_TIME_SPAN_PV:
    return "TIME_SPAN_PV";

  case OV_VT_BOOL_PV_VEC:
    return "BOOL_PV_VEC";
  case OV_VT_INT_PV_VEC:
    return "INT_PV_VEC";
  case OV_VT_UINT_PV_VEC:
    return "UINT_PV_VEC";
  case OV_VT_SINGLE_PV_VEC:
    return "SINGLE_PV_VEC";
  case OV_VT_DOUBLE_PV_VEC:
    return "DOUBLE_PV_VEC";
  case OV_VT_STRING_PV_VEC:
    return "STRING_PV_VEC";
  case OV_VT_TIME_PV_VEC:
    return "TIME_PV_VEC";
  case OV_VT_TIME_SPAN_PV_VEC:
    return "TIME_SPAN_PV_VEC";

  case OV_VT_ANY:
    return "ANY";
  default:
    ov_logfile_error("Unknown value type. %u\n", vartype);
    return NULL;
    exit(EXIT_FAILURE);
  }
  return NULL;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_strToOVType(OV_VAR_TYPE *vartype,
                                                   const OV_STRING typestr) {
  if (!typestr || !vartype) {
    return OV_ERR_BADPARAM;
  }
  if (ov_string_compare(typestr, "VOID") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_VOID;
  else if (ov_string_compare(typestr, "BYTE") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_BYTE;
  else if (ov_string_compare(typestr, "BOOL") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_BOOL;
  else if (ov_string_compare(typestr, "INT") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_INT;
  else if (ov_string_compare(typestr, "UINT") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_UINT;
  else if (ov_string_compare(typestr, "SINGLE") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_SINGLE;
  else if (ov_string_compare(typestr, "DOUBLE") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_DOUBLE;
  else if (ov_string_compare(typestr, "STRING") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STRING;
  else if (ov_string_compare(typestr, "TIME") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME;
  else if (ov_string_compare(typestr, "TIME_SPAN") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_SPAN;
  else if (ov_string_compare(typestr, "OV_STATE") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STATE;
  else if (ov_string_compare(typestr, "OV_STRUCT") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STRUCT;

  else if (ov_string_compare(typestr, "BYTE_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_BYTE_VEC;
  else if (ov_string_compare(typestr, "BOOL_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_BOOL_VEC;
  else if (ov_string_compare(typestr, "INT_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_INT_VEC;
  else if (ov_string_compare(typestr, "UINT_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_UINT_VEC;
  else if (ov_string_compare(typestr, "SINGLE_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_SINGLE_VEC;
  else if (ov_string_compare(typestr, "DOUBLE_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_DOUBLE_VEC;
  else if (ov_string_compare(typestr, "STRING_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STRING_VEC;
  else if (ov_string_compare(typestr, "TIME_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_VEC;
  else if (ov_string_compare(typestr, "TIME_SPAN_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_SPAN_VEC;
  else if (ov_string_compare(typestr, "TIME_SERIES") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_SERIES;
  else if (ov_string_compare(typestr, "STATE_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STATE_VEC;

  else if (ov_string_compare(typestr, "STRUCT_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STRUCT_VEC;

  else if (ov_string_compare(typestr, "BOOL_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_BOOL_PV;
  else if (ov_string_compare(typestr, "INT_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_INT_PV;
  else if (ov_string_compare(typestr, "UINT_PV*") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_UINT_PV;
  else if (ov_string_compare(typestr, "SINGLE_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_SINGLE_PV;
  else if (ov_string_compare(typestr, "DOUBLE_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_DOUBLE_PV;
  else if (ov_string_compare(typestr, "STRING_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STRING_PV;
  else if (ov_string_compare(typestr, "TIME_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_PV;
  else if (ov_string_compare(typestr, "TIME_SPAN_PV") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_SPAN_PV;

  else if (ov_string_compare(typestr, "BOOL_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_BOOL_PV_VEC;
  else if (ov_string_compare(typestr, "INT_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_INT_PV_VEC;
  else if (ov_string_compare(typestr, "UINT_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_UINT_PV_VEC;
  else if (ov_string_compare(typestr, "SINGLE_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_SINGLE_PV_VEC;
  else if (ov_string_compare(typestr, "DOUBLE_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_DOUBLE_PV_VEC;
  else if (ov_string_compare(typestr, "STRING_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_STRING_PV_VEC;
  else if (ov_string_compare(typestr, "TIME_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_PV_VEC;
  else if (ov_string_compare(typestr, "TIME_SPAN_PV_VEC") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_TIME_SPAN_PV_VEC;

  else if (ov_string_compare(typestr, "ANY") == OV_STRCMP_EQUAL)
    *vartype = OV_VT_ANY;
  else {
    vartype = OV_VT_VOID;
    return OV_ERR_BADPARAM;
  }
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT CTree_helper_accessToStr(OV_STRING *accessstr,
                                                   const OV_ACCESS *access) {
  ov_string_setvalue(accessstr, "");
  if (*access & KS_AC_NONE)
    ov_string_append(accessstr, "NONE ");
  else if (*access & KS_AC_READ)
    ov_string_append(accessstr, "READ ");
  if (*access & KS_AC_WRITE)
    ov_string_append(accessstr, "WRITE ");
  if (*access & KS_AC_DELETEABLE)
    ov_string_append(accessstr, "DELETEABLE ");
  if (*access & KS_AC_RENAMEABLE)
    ov_string_append(accessstr, "RENAMEABLE ");
  if (*access & KS_AC_LINKABLE)
    ov_string_append(accessstr, "LINKABLE ");
  if (*access & KS_AC_UNLINKABLE)
    ov_string_append(accessstr, "UNLINKABLE ");
  if (*access & KS_AC_INSTANTIABLE)
    ov_string_append(accessstr, "INSTANTIABLE ");
  if (*access & KS_AC_PART)
    ov_string_append(accessstr, "PART ");

  return OV_ERR_OK;
}
