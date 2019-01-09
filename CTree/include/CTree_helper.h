#ifndef _CTREE_HELPER_
#define _CTREE_HELPER_

#include "cJSON.h"
#include "libov/ov_macros.h"

#define CTREE_COMMON_INTERNALERROR 128
#define CTREE_COMMON_EXTERNALERROR 64

OV_RESULT valueToJSON(cJSON** pjsvalue, const OV_VAR_TYPE vartype,
                      const OV_BYTE* var, const OV_UINT size);

OV_RESULT jsonToVarvalue(OV_VAR_VALUE* pvalue, const cJSON* jsvalue);

OV_STRING CTree_helper_ovtypeToStr(OV_VAR_TYPE vartype);

OV_RESULT CTree_helper_strToOVType(OV_VAR_TYPE*    vartype,
                                   const OV_STRING typestr);
#endif
