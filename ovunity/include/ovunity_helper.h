/*
 * helper.h
 *
 *  Created on: Jul 13, 2018
 *      Author: zzz
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <stdarg.h>

#include "libov/ov_macros.h"
#include "libov/ov_ov.h"

#include "ovunity.h"

#include "ksbase.h"

#include "libov/ov_result.h"

#include "CException.h"
#include "cJSON.h"
#include "json_tools.h"

#include "NoneTicketAuthenticator.h"

#define objPath "/obj"
#define envPath "/env"
#define JSONOBJNAME "object"
#define JSONENVNAME "env"

#define buildFilePath "/build.json"

/**
 * @brief compares ist.json with soll.json
 *
 * @param pinst case
 *
 * @return
 */
OV_RESULT ovunity_compareIstSoll(const OV_INSTPTR_ovunity_ovCase pinst);

/**
 * @brief gets obj to test for this case
 *
 * @param pcase case
 *
 * @return obj to test
 */
OV_INSTPTR_ov_object ovunity_ovCase_getObjPath(OV_INSTPTR_ovunity_ovCase pcase);

/**
 * @brief build ovCase from init.json file
 *
 * @param pcase
 */
void ovunity_ovCase_build1(OV_INSTPTR_ovunity_ovCase pcase);

/**
 * @brief
 *
 * @param pinst
 * @param path
 * @param name
 *
 * @return
 */
OV_RESULT ovunity_main_getObjFilePath(OV_INSTPTR_ovunity_main pinst,
                                      OV_STRING* path, const OV_STRING name);

/**
 * @brief
 *
 * @param pinst
 * @param path
 * @param name
 *
 * @return
 */
OV_RESULT ovunity_main_getEnvFilePath(OV_INSTPTR_ovunity_main pinst,
                                      OV_STRING* path, const OV_STRING name);

/**
 * @brief creates case with case_name under pinst and links
 *
 * @param pinst
 * @param case_name
 *
 * @return
 */
OV_INSTPTR_ovunity_ovCase ovunity_createCase(OV_INSTPTR_ovunity_main pinst,
                                             const OV_STRING         case_name);

/**
 * @brief reads file and gives it as OV_STRING
 *
 * @param filename
 *
 * @return
 */
OV_STRING ovunity_helper_data2str(OV_STRING filename);

/* reads file and gives it as OV_STRING */
OV_RESULT ovunity_helper_str2data(const OV_STRING text,
                                  const OV_STRING filename);

/**
 * @brief loads json file as tree
 *
 * @param pinst
 * @param what name of json file in
 * $ACPLT_HOME/dev/projname/classname/*what*
 * @param where place to load in ov tree
 */
void ovunity_loadEnv(const OV_INSTPTR_ovunity_main pinst, const OV_STRING what,
                     const OV_STRING where);

/**
 * @brief loads json file as tree
 *
 * @param what full file json path
 * @param where root path in ov
 */
void ovunity_loadJsonAsTree(const OV_STRING what, const OV_STRING where);

/**
 * @brief gives path under the object to simulate environment
 *
 * @param pinst
 * @param case_name
 *
 * @return
 */
OV_STRING ovunity_getCasePath(const OV_INSTPTR_ovunity_main pinst,
                              const OV_STRING               case_name);

/**
 * @brief compares 2 json file and deletes same part, returns bool
 *
 * @param file1
 * @param file2
 *
 * @return
 */
int cJSON_isSame(cJSON* file1, cJSON* file2);

#endif /* HELPER_H_ */
