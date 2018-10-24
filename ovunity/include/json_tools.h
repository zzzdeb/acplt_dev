#ifndef _JSON_TOOLS_
#define _JSON_TOOLS_


#include "cJSON.h"
#include "libov/ov_ov.h"
#include "string.h"

cJSON* cJSON_ParseFromFile(OV_STRING path);

int cJSON_isSame(cJSON* file1, cJSON* file2);

#endif
