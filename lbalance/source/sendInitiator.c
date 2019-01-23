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

OV_DLLFNCEXPORT OV_RESULT lbalance_sendInitiator_clear_set(
    OV_INSTPTR_lbalance_sendInitiator pinst, const OV_BOOL value) {
  OV_RESULT                  result = OV_ERR_OK;
  OV_INSTPTR_PostSys_Message pMsg = NULL;
  if(value && !pinst->v_clear) {
    pinst->v_status = LB_SENDINIT_INIT;
    /* deleting all msgs in inbox */
    Ov_ForEachChildEx(ov_containment, &pinst->p_inbox, pMsg, PostSys_Message) {
      Ov_DeleteObject(pMsg);
    }
    pinst->v_accepted = FALSE;
  }
  pinst->v_clear = value;
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

  OV_UINT waitingMsgsLen = 0;

  switch(pinst->v_status) {
    case LB_SENDINIT_INIT:
        Ov_ForEachChildEx(ov_containment, &pinst->p_inbox, pMsg,
                          PostSys_Message) {
          waitingMsgsLen++;
        }
        // TODO: zzz: verify the context of msg :2019 Jan 06 11:59
        pinst->v_accepted = waitingMsgsLen > 0;
        break;

    LB_CASEERROR
  }

  return;
}

