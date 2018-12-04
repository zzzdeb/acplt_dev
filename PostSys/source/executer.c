/******************************************************************************
 *
 *   FILE
 *   ----
 *   executer.c
 *
 *   History
 *   -------
 *   Sa 01 Dez 2018 11:29:06 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_PostSys
#define OV_COMPILE_LIBRARY_PostSys
#endif

#include "PostSys.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT
                PostSys_executer_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT result = OV_ERR_OK;
  OV_INSTPTR_PostSys_executer this = Ov_StaticPtrCast(PostSys_executer, pobj);
  OV_INSTPTR_ov_domain pinbox = NULL;

  result = ov_object_constructor(pobj);

  result |= Ov_CreateObject(PostSys_inbox, pinbox, this, "inbox");

  return result;
}

OV_DLLFNCEXPORT void
PostSys_executer_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  OV_INSTPTR_PostSys_executer pinst = Ov_StaticPtrCast(PostSys_executer, this);

  OV_INSTPTR_PostSys_Message pMsg = NULL;
  OV_INSTPTR_ov_domain       pinbox =
      Ov_SearchChildEx(ov_containment, pinst, "inbox", ov_domain);
  if(!pinbox) {
    ov_logfile_error("no inbox for executer");
    return;
  }

  Ov_ForEachChildEx(ov_containment, pinbox, pMsg, PostSys_Message) { break; }
  if(!pMsg) return;
  ov_logfile_info("postExec: %s", pMsg->v_msgBody);

  // TODO: zzz: work on it Sa 01 Dez 2018 18:30:29 CET
  Ov_DeleteObject(pMsg);
  return;
}

