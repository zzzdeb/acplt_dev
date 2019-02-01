/*
 * object_helper.h
 *
 *  Created on: Aug 3, 2018
 *      Author: zzz
 */

#ifndef OBJECT_HELPER_H_
#define OBJECT_HELPER_H_

#include "libov/ov_macros.h"
#include "libov/ov_ov.h"
#include "ovhelper.h"

/**
 * set a variable with a pointer to the object and the name of the variable
 * (correct handling for chart variables)
 * @param pTargetObj
 * @param targetVarname
 * @param value
 * @return
 */
OV_RESULT object_setNamedVariable(const OV_INSTPTR_ov_object pTargetObj,
                                  const OV_STRING targetVarname, OV_ANY* value);

/**
 * checks if pchild descendant of pparent in ov_containment;
 * @param pparent
 * @param pchild
 * @return
 */
OV_BOOL object_isDescendant(const OV_INSTPTR_ov_object pparent,
                            const OV_INSTPTR_ov_object pchild);

/**
 * sets str1 = str2[beg:end]
 * @return
 */
OV_DLLFNCEXPORT OV_RESULT ov_string_get(OV_STRING* str1, OV_STRING str2,
                                        OV_INT beg, OV_INT end);

/**
 * @brief creates connection between 2 fb_objects
 *
 * @param inObj input object
 * @param inName input object var name
 * @param outObj analog
 * @param outName analog
 *
 * @return fb_connection object if null, it was unsuccessful
 */
OV_DLLFNCEXPORT OV_INSTPTR_fb_connection
                ov_fb_connect(OV_INSTPTR_fb_object inObj, OV_STRING inName,
                              OV_INSTPTR_fb_object outObj, OV_STRING outName);

/**
 * @brief multiplies time by factor. Cant handle negative factor.
 *
 * @param res
 * @param src
 * @param factor
 *
 * @return
 */
OV_DLLFNCEXPORT void ov_time_multiply(OV_TIME* res, const OV_TIME src,
                                      const OV_SINGLE factor);
OV_DLLFNCEXPORT void ov_timespan_multiply(OV_TIME_SPAN*      res,
                                          const OV_TIME_SPAN src,
                                          const OV_SINGLE    factor);
#endif /* OBJECT_HELPER_H_ */
