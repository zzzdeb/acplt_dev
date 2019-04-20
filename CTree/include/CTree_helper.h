#ifndef _CTREE_HELPER_
#define _CTREE_HELPER_

#include "cJSON.h"
#include "libov/ov_macros.h"

#define CTREE_COMMON_INTERNALERROR 64
#define CTREE_COMMON_EXTERNALERROR 128

#define CTREE_SF_INIT 0
#define CTREE_SF_SENT_WAITING 1
#define CTREE_SF_DONE 2

#define CTREE_LL_INIT 0
#define CTREE_LL_INFOREQSTED 1
#define CTREE_LL_INFO_RECEIVED 2
#define CTREE_LL_DATA_SENT 3
#define CTREE_LL_DONE 4

#define CTREE_TR_INIT 0
#define CTREE_TR_LIBSSENT_WAITING 1
#define CTREE_TR_TREESENT_WAITING 2
#define CTREE_TR_DONE 3

/**
 * @brief converts ov_Value to json
 *
 * @param pjsvalue
 * @param vartype type of value
 * @param var pointer to value
 * @param size not used so far. It is for value, where byte lenght can not be
 * extracted from type
 *
 * @return
 */
OV_RESULT valueToJSON(cJSON** pjsvalue, const OV_VAR_TYPE vartype,
                      const OV_BYTE* var, const OV_UINT size);

/**
 * @brief writes ov_value from json
 *
 * @param pvalue
 * @param jsvalue
 *
 * @return
 */
OV_RESULT jsonToVarvalue(OV_VAR_VALUE* pvalue, const cJSON* jsvalue);

OV_STRING CTree_helper_ovtypeToStr(OV_VAR_TYPE vartype);

OV_RESULT CTree_helper_strToOVType(OV_VAR_TYPE*    vartype,
                                   const OV_STRING typestr);
#endif
