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


/* reads file and gives it as OV_STRING */
OV_STRING ovunity_helper_data2str(OV_STRING filename);

/* loads json file as tree */
/*
 * gives path under the object to simulate environment
 */
void ovunity_loadEnv(const OV_INSTPTR_ovunity_main pinst, const OV_STRING what, const OV_STRING where);

/* getting case path */
/*
 * gives path under the object to simulate environment
 */
OV_STRING ovunity_getCasePath(const OV_INSTPTR_ovunity_main pinst,
		const OV_STRING case_name);


#endif /* HELPER_H_ */
