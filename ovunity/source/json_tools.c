/*
 * json_tools.c
 *
 *  Created on: Oct 19, 2018
 *      Author: zzz
 */

#include "cJSON.h"

#include "libov/ov_macros.h"
#include "libov/ov_ov.h"
#include "string.h"
#include "ovunity_helper.h"

OV_DLLFNCEXPORT cJSON* cJSON_ParseFromFile(OV_STRING path) {
  OV_STRING jsonStr = ovunity_helper_data2str(path);
  cJSON*    tmp = cJSON_Parse(jsonStr);
  Ov_HeapFree(jsonStr);
  return tmp;
}

OV_DLLFNCEXPORT int cJSON_isSame(cJSON* file1, cJSON* file2) {
  cJSON* child1 = NULL;
  cJSON* child2 = NULL;
  int    same = 0;
  int    i = 0;
  cJSON* jstmp = NULL;
  for(child1 = (file1 != ((void*)0)) ? (file1)->child : ((void*)0);
      child1 != ((void*)0);) {
    same = 0;
    if(child1->string)
      child2 = cJSON_GetObjectItem(file2, child1->string);
    else
      child2 = cJSON_GetArrayItem(file2, i);
    if(!child2 || child2->type != child1->type) {
      // todo: error
      child1 = child1->next;
      continue;
    }

    switch(child1->type) {
      case cJSON_Invalid:
        if(cJSON_IsInvalid(child2)) same = 1;
        break;
      case cJSON_False:
        if(cJSON_IsFalse(child2)) same = 1;
        break;
      case cJSON_True:
        if(cJSON_IsTrue(child2)) same = 1;
        break;
      case cJSON_NULL:
        if(cJSON_IsNull(child2)) same = 1;
        break;
      case cJSON_Number:
        if(child1->valuedouble == child2->valuedouble) same = 1;
        break;
      case cJSON_String:
        if(!strcmp(child1->valuestring, child2->valuestring)) same = 1;
        break;
      case cJSON_Array:
        same = cJSON_isSame(child1, child2);
        if(same == -1) { // todo info
        };
        break;
      case cJSON_Object:
        same = cJSON_isSame(child1, child2);
        if(same == -1) { // todo info
        };
        break;
      case cJSON_Raw:
        if(cJSON_IsRaw(child2)) same = 1;
        break;
      default:
        // todo: error
        return -1;
    }
    if(same) {
      if(child1->string) {
        jstmp = child1->prev;
        cJSON_DeleteItemFromObject(file1, child1->string);
        child1 = jstmp ? jstmp : file1->child;

        jstmp = child2->prev;
        cJSON_DeleteItemFromObject(file2, child2->string);
        child2 = jstmp ? jstmp : file2->child;
      } else {
        jstmp = child1->prev;
        cJSON_DeleteItemFromArray(file1, i);
        child1 = jstmp ? jstmp : file1->child;

        jstmp = child2->prev;
        cJSON_DeleteItemFromArray(file2, i);
        child2 = jstmp ? jstmp : file2->child;
      }
    } else {
      child1 = child1->next;
      i++;
    }
  }
  if(cJSON_GetArraySize(file1) || cJSON_GetArraySize(file2)) return 0;
  return 1;
}
