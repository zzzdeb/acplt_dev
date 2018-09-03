
#ifndef __KSAPI_HELPER_H_
#define __KSAPI_HELPER_H_

#include "ksapi_commonFuncs.h"
#include "libov/ov_logfile.h"
#include "libov/ov_result.h"

static const unsigned int TIMEOUT = 10;  // Timeout of ksapi actions in seconds

/**
 * Uninlined function to check if a ksapi object finished its operation with an error. If so, log the error.
 *
 * @param ksapi	The ksapi block to check its state.
 * @param start	The timestamp, when the operation was started (to check for timeouts)
 * @param pltc	The current fb clock (to check for timeouts)
 * @return		TRUE if an error occured in the ksapi operation, FALSE otherwise (operation still ongoing or success)
 */
static inline OV_BOOL ksapiHelper_checkForKSApiError(OV_INSTPTR_ksapi_KSApiCommon ksapi, OV_TIME *start, OV_TIME *pltc) {
	if(ksapi->v_status == KSAPI_COMMON_INTERNALERROR) {
		ov_logfile_error("Internal error of ksapi: %s", ov_result_getresulttext(ksapi->v_result));
		return TRUE;
	} else if(ksapi->v_status == KSAPI_COMMON_EXTERNALERROR) {
		ov_logfile_error("Request error of ksapi: %s", ov_result_getresulttext(ksapi->v_result));
		return TRUE;
	} else if(ksapi->v_status == KSAPI_COMMON_WAITINGFORANSWER) {
		if(pltc->secs > start->secs + TIMEOUT) {
			ov_logfile_error("Timeout while waiting for ksapi request");
			return TRUE;
		}
	}
	return FALSE;
}

#endif
