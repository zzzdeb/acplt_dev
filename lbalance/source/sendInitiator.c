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
    ov_string_setvalue(&pinst->v_appPath, NULL);
    Ov_ForEachChildEx(ov_containment, &pinst->p_inbox, pMsg, PostSys_Message) {
      Ov_DeleteObject(pMsg);
    }
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
  OV_RESULT                  result = OV_ERR_OK;

  OV_UINT waitingMsgsLen = 0;

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
        ov_logfile_info("lbalance_sendInitiator: accepted %s", pinst->v_outApp);
        result |= ov_string_setvalue(&pinst->v_appPath, pinst->v_outApp);
        if(Ov_Fail(result)) {
          ov_logfile_error("%u: %s: failed to set appPath", result,
                           ov_result_getresulttext(result));
          pinst->v_status = LB_INTERNALERROR;
          pinst->v_result = result;
          return;
        }
        ov_logfile_info("lbalance_sendInitiator: accepting %s",
                        pinst->v_appPath);
        break;
      }
    case LB_SENDINIT_SENT:
      break;

      LB_CASEERROR
  }

  return;
}

