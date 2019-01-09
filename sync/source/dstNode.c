/******************************************************************************
 *
 *   FILE
 *   ----
 *   dstNode.c
 *
 *   History
 *   -------
 *   Do 29 Nov 2018 16:12:01 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_sync
#define OV_COMPILE_LIBRARY_sync
#endif

#include "sync.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT void sync_dstNode_typemethod(OV_INSTPTR_ksbase_ComTask this) {
  /*
   *   local variables
   */
  /*OV_INSTPTR_sync_dstNode pinst = Ov_StaticPtrCast(sync_dstNode, this);*/

  PostSys_node_typemethod(this);

  return;
}

