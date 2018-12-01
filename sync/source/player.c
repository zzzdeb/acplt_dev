/******************************************************************************
 *
 *   FILE
 *   ----
 *   player.c
 *
 *   History
 *   -------
 *   Do 29 Nov 2018 15:37:07 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_sync
#define OV_COMPILE_LIBRARY_sync
#endif

#include "sync.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT sync_player_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT              result = OV_ERR_OK;
  OV_INSTPTR_sync_player pinst = Ov_StaticPtrCast(sync_player, pobj);
  OV_INSTPTR_ov_domain   pNode = NULL;
  OV_INSTPTR_ov_object   pInbox = NULL;

  result = ov_object_constructor(pobj);
  result |= Ov_CreateObject(ov_domain, pNode, pinst, "srcNode");
  result |= Ov_CreateObject(sync_sscBuffer, pInbox, pNode, "inbox");
  result |= Ov_CreateObject(ov_domain, pNode, pinst, "locNode");
  result |= Ov_CreateObject(sync_dstNode, pInbox, pNode, "inbox");
  // TODO: zzz: not ready  Do 29 Nov 2018 16:01:39 CET
  return result;
}

OV_DLLFNCEXPORT void sync_player_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                            OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_player pinst = Ov_StaticPtrCast(sync_player, pfb);

  return;
}

