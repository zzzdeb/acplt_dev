/*
 * ks_helper.h
 *
 *  Created on: Aug 3, 2018
 *      Author: zzz
 */

#ifndef KS_HELPER_H_
#define KS_HELPER_H_

#include "libov/ov_macros.h"

OV_RESULT parse_kspath(const OV_STRING kspath,
		OV_STRING *serverHost,
		OV_STRING *port, OV_STRING *serverName, OV_STRING *path);

#endif /* KS_HELPER_H_ */
