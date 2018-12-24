/*
 * object_helper.c
 *
 *  Created on: Aug 3, 2018
 *      Author: zzz
 */

#include "object_helper.h"
#include "fb.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"

OV_DLLFNCEXPORT OV_RESULT
                object_setNamedVariable(const OV_INSTPTR_ov_object pTargetObj,
                                        const OV_STRING targetVarname, OV_ANY* value) {
  OV_RESULT            result = OV_ERR_OK;
  OV_ELEMENT           element;
  OV_ELEMENT           varElement;
  OV_VTBLPTR_ov_object pVtblObj = NULL;

  if(pTargetObj == NULL) {
    return OV_ERR_BADPARAM;
  } else if(Ov_CanCastTo(fb_functionchart, pTargetObj)) {
    // set variable in a functionchart
    result = fb_functionchart_setport(
        Ov_StaticPtrCast(fb_functionchart, pTargetObj), targetVarname, value);
  } else {
    // set variable in a object
    varElement.elemtype = OV_ET_NONE;
    element.elemtype = OV_ET_OBJECT;
    element.pobj = pTargetObj;

    // search the variable for the set operation
    ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, targetVarname);
    if(varElement.elemtype == OV_ET_VARIABLE) {
      // port found, use the setter to write the value
      Ov_GetVTablePtr(ov_object, pVtblObj, pTargetObj);
      result = pVtblObj->m_setvar(varElement.pobj, &varElement, value);
    }
  }

  return result;
}

OV_DLLFNCEXPORT OV_BOOL object_isDescendant(const OV_INSTPTR_ov_object pparent,
                                            const OV_INSTPTR_ov_object pchild) {
  OV_PATH path = {0};
  ov_memstack_lock();
  OV_STRING pathname =
      ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pchild), 2);

  ov_path_resolve(&path, NULL, pathname, 2);
  if(!path.elements) {
    ov_logfile_error("ovhelper_object_helper: %s no path",
                     pchild->v_identifier);
    ov_memstack_unlock();
    return -1;
  }
  for(OV_UINT i = 0; i < path.size; ++i) {
    if(path.elements[i].pobj == pparent) {
      ov_memstack_unlock();
      return 1;
    }
  }
  ov_memstack_unlock();
  return 0;
}
