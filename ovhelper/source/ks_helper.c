/*
 * ks_helper.c
 *
 *  Created on: Aug 3, 2018
 *      Author: zzz
 */

#include "ks_helper.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT parse_kspath(const OV_STRING kspath,
		OV_STRING *serverHost,
		OV_STRING *port, OV_STRING *serverName, OV_STRING *path) {
	OV_RESULT result = OV_ERR_OK;

	if(kspath == NULL) return OV_ERR_BADPARAM;

	OV_STRING serverNameStr = NULL;
	OV_STRING pathStr = NULL;

	OV_STRING kspathcopy = NULL;
	ov_string_setvalue(&kspathcopy, kspath);
	OV_STRING portStr = strchr(kspathcopy, ':');
	if(portStr) {
		*portStr = 0;
		portStr++;
		serverNameStr = strchr(portStr, '/');
	} else {
		serverNameStr = strchr(kspathcopy, '/');
	}
	if(serverNameStr) {
		*serverNameStr = 0;
		serverNameStr++;
	}

	if(serverNameStr) {
		pathStr = strchr(serverNameStr, '/');
		if(pathStr) {
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
	if(serverHost && !result) {
		result = ov_string_setvalue(serverHost, kspathcopy);
		if(Ov_Fail(result)) {
			ov_string_setvalue(&kspathcopy, NULL);
			result = OV_ERR_GENERIC;
		}
	}

	if(port && !result) {
		result = ov_string_setvalue(port, portStr);
		;
		if(Ov_Fail(result)) {
			result = OV_ERR_GENERIC;
		}
	}

	if(serverName && !result) {
		result = ov_string_setvalue(serverName, serverNameStr);
		if(Ov_Fail(result)) {
			result = OV_ERR_GENERIC;
		}
	}

	if(path && !result) {
		result = ov_string_setvalue(path, pathStr);
		if(Ov_Fail(result)) {
			result = OV_ERR_GENERIC;
		}
	}
	ov_string_setvalue(&kspathcopy, NULL);
	ov_string_setvalue(&portStr, NULL);
	ov_string_setvalue(&serverNameStr, NULL);
	ov_string_setvalue(&pathStr, NULL);
	return result;
}
