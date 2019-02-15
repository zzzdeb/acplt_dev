/******************************************************************************
 *
 *   FILE
 *   ----
 *   syncDownload.c
 *
 *   History
 *   -------
 *   Mi 05 Dez 2018 18:22:51 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_sync
#define OV_COMPILE_LIBRARY_sync
#endif

#include "sync.h"

#include "libov/ov_macros.h"
#include "libov/ov_ov.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include "libov/ov_string.h"

#include "object_helper.h"
#include "ovhelper.h"

#include "CTree.h"

#include "sync_helper.h"

#include "ksapi.h"

OV_DLLFNCEXPORT void
sync_syncDownload_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_syncDownload pinst = Ov_StaticPtrCast(sync_syncDownload, pfb);

  switch(pinst->v_state) {
    case SYNC_DOWNLOAD_DONE:
    case SYNC_DOWNLOAD_INIT:
      if(!pinst->v_path) {
        ov_logfile_error("sync_syncDownload: pinst->path is NULL");
        return;
      }

      CTree_Download_typemethod(Ov_StaticPtrCast(fb_functionblock, pinst),
                                pltc);
      pinst->v_state = SYNC_DOWNLOAD_DONE;
      OV_RESULT result = pinst->v_result;
      switch(result) {
        case OV_ERR_OK:
          ov_logfile_info("syncDownload done.");
          break;
        default:
          pinst->v_state = SYNC_INTERNALERROR;
          ov_logfile_error("syncDownload failed. : %s",
                           ov_result_getresulttext(result));
      }
      break;
    default:
      ov_logfile_error("sync_syncDownload: Unknown state %d", pinst->v_state);
      break;
  }
  return;
}
