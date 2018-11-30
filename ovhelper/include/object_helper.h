/*
 * object_helper.h
 *
 *  Created on: Aug 3, 2018
 *      Author: zzz
 */

#ifndef OBJECT_HELPER_H_
#define OBJECT_HELPER_H_

#include "libov/ov_macros.h"

/**
 * set a variable with a pointer to the object and the name of the variable
 * (correct handling for chart variables)
 * @param pTargetObj
 * @param targetVarname
 * @param value
 * @return
 */
OV_RESULT object_setNamedVariable(const OV_INSTPTR_ov_object pTargetObj,
                                  const OV_STRING targetVarname, OV_ANY *value);

/**
 * checks if pchild descendant of pparent in ov_containment;
 * @param pparent
 * @param pchild
 * @return
 */
OV_BOOL object_isDescendant(const OV_INSTPTR_ov_object pparent,
                            const OV_INSTPTR_ov_object pchild);

#endif /* OBJECT_HELPER_H_ */
