/******************************************************************************
 *
 *   FILE
 *   ----
 *   sendInitiator.c
 *
 *   History
 *   -------
 *   Do 03 Jan 2019 00:34:31 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"
#include "lbalance_helper.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include <stdio.h>

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT lbalance_sendInitiator_reset_set(
    OV_INSTPTR_lbalance_sendInitiator pinst, const OV_BOOL value) {
  pinst->v_reset = value;
  return 0;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_sendInitiator_R_set(
    OV_INSTPTR_lbalance_sendInitiator pinst, const OV_BOOL value) {
  OV_RESULT                  result = OV_ERR_OK;
  OV_INSTPTR_PostSys_Message pMsg = NULL;
  if(value && !pinst->v_R) {
    pinst->v_status = LB_SENDINIT_INIT;
    /* deleting all msgs in inbox */
    Ov_ForEachChildEx(ov_containment, &pinst->p_inbox, pMsg, PostSys_Message) {
      Ov_DeleteObject(pMsg);
    }
    Ov_SetDynamicVectorLength(&pinst->v_appPaths, 0, STRING);
  }
  pinst->v_R = value;
  return result;
}

OV_DLLFNCEXPORT void
lbalance_sendInitiator_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                  OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_sendInitiator pinst =
      Ov_StaticPtrCast(lbalance_sendInitiator, pfb);

  OV_INSTPTR_PostSys_Message pMsg = NULL;
  OV_STRING                  appPath = NULL;
  OV_RESULT                  result = OV_ERR_OK;

  OV_UINT waitingMsgsLen = 0;
  fprintf(stderr, "%s", "test");
  Ov_WarnIf(1);

  switch(pinst->v_status) {
    case LB_SENDINIT_INIT:
      if(pinst->v_R) {
        Ov_ForEachChildEx(ov_containment, &pinst->p_inbox, pMsg,
                          PostSys_Message) {
          waitingMsgsLen++;
        }
        // TODO: zzz: verify the context of msg :2019 Jan 06 11:59
        if(!waitingMsgsLen) {
          ov_logfile_warning("lbalance_sendInitiator: not accepted this time");
          pinst->v_status = LB_SENDINIT_SENT;
          return;
        }
        ov_logfile_info("lbalance_sendInitiator: accepted");

        for(OV_UINT i = 0; i < pinst->v_appPaths.veclen; ++i) {
          OV_INSTPTR_ov_domain pdom = NULL;
          OV_INSTPTR_ov_object pobj = NULL;
          pdom = Ov_StaticPtrCast(
              ov_domain,
              ov_path_getobjectpointer(pinst->v_appPaths.value[i], 2));
          Ov_ForEachChild(ov_containment, pdom, pobj) {
            if(ov_string_compare(pinst->v_outApp, pobj->v_identifier) ==
               OV_STRCMP_EQUAL) {
              ov_string_print(&appPath, "%s/%s", pinst->v_appPaths.value[i],
                              pinst->v_outApp);
              result |= ov_string_setvalue(&pinst->v_outApp, appPath);

              if(Ov_Fail(result)) {
                ov_logfile_error("%u: %s: ", result,
                                 ov_result_getresulttext(result));
                pinst->v_status = LB_INTERNALERROR;
                pinst->v_result = result;
                return;
              }
            }
          }
        }

        break;
      }
    case LB_SENDINIT_SENT:
      break;

      LB_CASEERROR
  }

  return;
}

