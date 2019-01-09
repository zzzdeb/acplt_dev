/*
 * ovunity_helper.c
 *
 *  Created on: Oct 19, 2018
 *      Author: zzz
 */

#include "libov/ov_ov.h"
#include "cJSON.h"
#include "ovunity_helper.h"
#include "libov/ov_path.h"


/* build ovCase from init.json file */
OV_DLLFNCEXPORT void ovunity_ovCase_build1(OV_INSTPTR_ovunity_ovCase pcase) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ov_domain penv = NULL;
	OV_STRING buildJsonPath = NULL;
	ov_string_print(&buildJsonPath, "%s%s", pcase->v_sysPath, buildFilePath);
	ov_memstack_lock();
	ov_string_setvalue(&pcase->v_path, ov_path_getcanonicalpath(pcase, 2));

	cJSON* jsbuild = cJSON_ParseFromFile(buildJsonPath);
	if(!jsbuild) //todo: inform
		return;
	cJSON * jsobj = cJSON_GetObjectItem(jsbuild, JSONOBJNAME);
	OV_INSTPTR_ovunity_main pmain = Ov_GetParent(ovunity_case, pcase);
	if(jsobj) {
		result = ovunity_main_getObjFilePath(pmain, &pcase->v_objFilePath,
			jsobj->valuestring);
	} else {
		ov_logfile_warning("no obj found in build.json. getting default");
		return;
//		result = ovunity_main_getEnvFilePath(pmain, &pcase->v_objFilePath,
//			"default");
//		ov_string_setvalue(&pcase->v_envFilePath, NULL);
	}
	//todo result check

	ov_string_print(&pcase->v_objPath, "%s%s", pcase->v_path, objPath);
	//load obj
	ovunity_loadJsonAsTree(pcase->v_objFilePath, pcase->v_objPath);
	//todo check

	cJSON * jsenv = cJSON_GetObjectItem(jsbuild, JSONENVNAME);
	if(jsenv) {
		result = ovunity_main_getEnvFilePath(pmain, &pcase->v_envFilePath,
			jsenv->valuestring);
		ov_string_print(&pcase->v_envPath, "%s%s", pcase->v_path, envPath);
		//load env
		ovunity_loadJsonAsTree(pcase->v_envFilePath, pcase->v_envPath);
		//todo check
	} else {
		ov_logfile_warning("no env found in build.json. getting default");
//		result = ovunity_main_getEnvFilePath(pmain, &pcase->v_envFilePath,
//			"default");
		Ov_CreateObject(ov_domain, penv, pcase, "env");
		ov_string_print(&pcase->v_envPath, "%s%s", pcase->v_path, envPath);
	}
	//todo result check


	cJSON_Delete(jsbuild);
	ov_memstack_unlock();
//	ovunity_loadEnv();
}
