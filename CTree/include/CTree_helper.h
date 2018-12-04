#ifdef _CTREE_HELPER_
#define _CTREE_HELPER_

#include "cJSON.h"
#include "libov/ov_macros.h"

OV_RESULT valueToJSON(cJSON** pjsvalue, const OV_VAR_TYPE vartype,
                      const OV_BYTE* var, const OV_UINT size);
#endif
