/******************************************************************************
 *
 *   file
 *   ----
 *   library_open.c
 *
 *   history
 *   -------
 *   So 06 Jan 2019 23:54:29 CET zzz file created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"
#ifdef ov_library_open_lbalance
#undef ov_library_open_lbalance
#endif

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"

OV_RESULT ov_library_setglobalvars_lbalance_new(void) {
  OV_RESULT result = OV_ERR_OK;

  result = ov_library_setglobalvars_lbalance();
  if(Ov_Fail(result)) {
    return result;
  }

  OV_INSTPTR_ov_domain     plbalance = NULL;
  OV_INSTPTR_lbalance_BRAD pbrad = NULL;

  OV_INSTPTR_ov_domain pData = NULL;
  OV_INSTPTR_fb_task   pUrtask = NULL;
  pUrtask =
      Ov_DynamicPtrCast(fb_task, ov_path_getobjectpointer("/Tasks/UrTask", 2));

  pData = Ov_StaticPtrCast(ov_domain,
                           Ov_SearchChild(ov_containment, &pdb->root, "data"));
  if(!pData) {
    return result;
  }
  plbalance = Ov_StaticPtrCast(
      ov_domain, Ov_SearchChild(ov_containment, pData, "lbalance"));
  if(!plbalance) {
    result = Ov_CreateObject(ov_domain, plbalance, pData, "lbalance");
    if(Ov_Fail(result)) {
      ov_logfile_error("Error: %s: couldnt create lbalance in /data;",
                       ov_result_getresulttext(result));
      return OV_ERR_OK;
    }
  }
  pbrad = Ov_StaticPtrCast(lbalance_BRAD,
                           Ov_SearchChild(ov_containment, plbalance, "BRAD"));
  if(!pbrad) {
    result = Ov_CreateObject(lbalance_BRAD, pbrad, plbalance, "BRAD");
    if(Ov_Fail(result)) {
      ov_logfile_error("Error: %s: couldnt create BRAD;",
                       ov_result_getresulttext(result));
    }
    if(pUrtask) {
      Ov_Link(fb_tasklist, pUrtask, pbrad);
    } else {
      ov_logfile_warning("lbalance_library_open: brad not listed in UrTask");
    }
  }
  return result;
}

OV_DLLFNCEXPORT OV_LIBRARY_DEF* ov_library_open_lbalance(void) {
  /* local variables */
  static OV_LIBRARY_DEF* OV_LIBRARY_DEF_lbalance_new;
  /*
   *       replace the 'setglobalvars' function created by the code
   * generator with a new one.
   */
  OV_LIBRARY_DEF_lbalance_new = ov_library_open_lbalance_old();
  OV_LIBRARY_DEF_lbalance_new->setglobalvarsfnc =
      ov_library_setglobalvars_lbalance_new;
  return OV_LIBRARY_DEF_lbalance_new;
}

