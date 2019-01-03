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
#include "libov/ov_result.h"

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

OV_DLLFNCEXPORT OV_INSTPTR_fb_connection
                ov_fb_connect(OV_INSTPTR_fb_object fromObj, OV_STRING fromName,
                              OV_INSTPTR_fb_object toObj, OV_STRING toName) {
  OV_RESULT                result = OV_ERR_OK;
  OV_INSTPTR_fb_connection pconn = NULL;
  OV_INSTPTR_ov_domain     pconnDom =
      Ov_StaticPtrCast(ov_domain, ov_path_getobjectpointer("/Cons", 2));
  if(!pconnDom) {
    ov_logfile_error("ovhelper_object_helper: could not get /Cons ");
    return NULL;
  }

  Ov_CreateIDedObject(fb_connection, pconn, pconnDom, "fb_connection");
  if(!pconn) {
    ov_logfile_error("ovhelper_object_helper: could not create fb_connection");
    return NULL;
  }
  result |= fb_connection_sourceport_set(pconn, fromName);
  result |= fb_connection_targetport_set(pconn, fromName);
  result |= Ov_Link(fb_inputconnections, toObj, pconn);
  result |= Ov_Link(fb_outputconnections, fromObj, pconn);
  result |= fb_connection_on_set(pconn, 1);
  if(Ov_Fail(result)) {
    ov_logfile_error("ovhelper_object_helper: %u: %s: could not set ports",
                     result, ov_result_getresulttext(result));
    return NULL;
  }
  return pconn;
}
