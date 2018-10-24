/*
 * helper.h
 *
 *  Created on: Jul 13, 2018
 *      Author: zzz
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <stdarg.h>
#include "libov/ov_ov.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "ksbase.h"
#include "NoneTicketAuthenticator.h"
#include "CException.h"
#include "cJSON.h"
#include "json_tools.h"
#include "ovunity.h"

#define objPath "/obj"
#define envPath "/env"
#define JSONOBJNAME "object"
#define JSONENVNAME "env"

#define buildFilePath "/build.json"

/* compares ist with soll.json*/
OV_RESULT ovunity_compareIstSoll(const OV_INSTPTR_ovunity_ovCase pinst);

/* getting obj path */
OV_INSTPTR_ov_object ovunity_ovCase_getObjPath(OV_INSTPTR_ovunity_ovCase pcase);

/* build ovCase from init.json file */
void ovunity_ovCase_build1(OV_INSTPTR_ovunity_ovCase pcase);

OV_RESULT ovunity_main_getObjFilePath(OV_INSTPTR_ovunity_main pinst,
		OV_STRING* path, const OV_STRING name);

OV_RESULT ovunity_main_getEnvFilePath(OV_INSTPTR_ovunity_main pinst,
		OV_STRING* path, const OV_STRING name);


/* creates case with case_name under pinst and links */
OV_INSTPTR_ovunity_ovCase ovunity_createCase(OV_INSTPTR_ovunity_main pinst,
		const OV_STRING case_name);

/* reads file and gives it as OV_STRING */
OV_STRING ovunity_helper_data2str(OV_STRING filename);

/* reads file and gives it as OV_STRING */
OV_RESULT ovunity_helper_str2data(const OV_STRING text,
		const OV_STRING filename);

/* loads json file as tree */
/*
 * gives path under the object to simulate environment
 */
void ovunity_loadEnv(const OV_INSTPTR_ovunity_main pinst, const OV_STRING what,
		const OV_STRING where);

/* loads json file as tree */
void ovunity_loadJsonAsTree(const OV_STRING what,
		const OV_STRING where);

/* getting case path */
/*
 * gives path under the object to simulate environment
 */
OV_STRING ovunity_getCasePath(const OV_INSTPTR_ovunity_main pinst,
		const OV_STRING case_name);

/* compares 2 json file and deletes same part, returns bool */
int cJSON_isSame(cJSON* file1, cJSON* file2);

#endif /* HELPER_H_ */
