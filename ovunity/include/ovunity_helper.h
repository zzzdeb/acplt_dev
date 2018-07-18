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
OV_STRING ReadFile1(OV_STRING filename);

/* loads json file as tree */
void load_test_data(OV_STRING projname, OV_STRING name);



#endif /* HELPER_H_ */
