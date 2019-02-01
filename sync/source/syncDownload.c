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

OV_BOOL startsWith(const char* pre, const char* str) {
  size_t lenpre = strlen(pre), lenstr = strlen(str);
  return lenstr < lenpre ? FALSE : strncmp(pre, str, lenpre) == 0;
}

OV_RESULT syncDownloadConfigureSetGet(OV_INSTPTR_sync_syncDownload pinst,
                                      OV_INSTPTR_ov_object         pobj) {
  OV_INSTPTR_fb_task pUrtask = NULL;
  pUrtask =
      Ov_DynamicPtrCast(fb_task, ov_path_getobjectpointer("/Tasks/UrTask", 2));

  if(Ov_CanCastTo(ksapi_setVar, pobj) || Ov_CanCastTo(ksapi_getVar, pobj)) {
    OV_INSTPTR_ksapi_KSApiCommon pobjCasted =
        Ov_StaticPtrCast(ksapi_KSApiCommon, pobj);

    OV_INSTPTR_ksmsg_msgClient pMsgClnt = Ov_SearchChildEx(
        ov_containment, pobjCasted, "msgClient", ksmsg_msgClient);
    if(!pMsgClnt) {
      ov_logfile_error("sync_syncDownload: %s has no msgClient",
                       pobjCasted->v_identifier);
      return OV_ERR_GENERIC;
    }
    ov_string_setvalue(&pMsgClnt->v_pathInstance.value[1],
                       PLAYER_DSTNODE_PATH_DEST);
    ov_string_setvalue(&pMsgClnt->v_pathHost.value[0],
                       pMsgClnt->v_pathHost.value[1]);
    ov_string_setvalue(&pMsgClnt->v_pathName.value[0],
                       pMsgClnt->v_pathName.value[1]);
    ov_string_setvalue(
        &pMsgClnt->v_pathInstance.value[0],
        ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, pMsgClnt), 2));
    OV_INSTPTR_fb_functionblock pFbobj =
        Ov_DynamicPtrCast(fb_functionblock, pobj->v_pouterobject);
    if(pFbobj) {
      if(pFbobj->v_cyctime.secs || pFbobj->v_cyctime.usecs)
        ov_timespan_multiply(&pFbobj->v_cyctime, pFbobj->v_cyctime, 0.5);
      else
        ov_timespan_multiply(&pFbobj->v_cyctime, pUrtask->v_cyctime, 0.5);
    }
  } else {
    ov_logfile_error("%s is not from right class", pobj->v_identifier);
    return OV_ERR_BADPARAM;
  }
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT void
sync_syncDownload_typemethod(OV_INSTPTR_fb_functionblock pfb, OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_sync_syncDownload pinst = Ov_StaticPtrCast(sync_syncDownload, pfb);
  OV_STRING                    path = NULL;
  OV_INSTPTR_ov_domain         proot = NULL;

  switch(pinst->v_state) {
    case SYNC_DOWNLOAD_INIT:
      ov_string_setvalue(&path, pinst->v_path);
      if(!path) {
        ov_logfile_error("sync_syncDownload: pinst->path is NULL");
        return;
      }

      CTree_Download_typemethod(Ov_StaticPtrCast(fb_functionblock, pinst),
                                pltc);
      pinst->v_state = SYNC_DOWNLOAD_DONE;
      OV_RESULT result = pinst->v_result;
      ov_string_setvalue(&pinst->v_path, path);
      switch(result) {
        case OV_ERR_OK:
          proot =
              Ov_StaticPtrCast(ov_domain, ov_path_getobjectpointer(path, 2));
          if(!proot) {
            ov_logfile_error("sync_syncDownload: pdstImage couldnt found");
            pinst->v_result = OV_ERR_GENERIC;
            return;
          }
          // setvar
          const OV_STRING classesToConfiugure[] = {[KSAPIGET] = "ksapi/getVar",
                                                   [KSAPISET] = "ksapi/setVar"};

          for(OV_UINT i = 0; i < SYNC_CONFIGURE_LEN; ++i) {
            OV_INSTPTR_ov_class pssc = ov_class_search(classesToConfiugure[i]);
            if(pssc) {
              OV_INSTPTR_ov_object pobj = NULL;
              Ov_ForEachChild(ov_instantiation, pssc, pobj) {
                /* checking if right one */
                if(object_isDescendant(Ov_StaticPtrCast(ov_object, proot),
                                       Ov_StaticPtrCast(ov_object, pobj))) {
                  /* create Msg alternative */
                  result = syncDownloadConfigureSetGet(pinst, pobj);
                  if(Ov_Fail(result)) {
                    ov_logfile_error(
                        "%u: %s: configuring SetGet on dst site didnt work",
                        result, ov_result_getresulttext(result));
                    pinst->v_result = result;
                    return;
                  }
                }
              }
            } else {
              ov_logfile_info("%s not used", classesToConfiugure[i]);
            }
          }
          ov_logfile_info("setGetConfig on dst done.");
          pinst->v_result = result;
          ov_logfile_info("syncDownload done.");
          break;
        default:
          pinst->v_result = result;
          ov_logfile_error("syncDownload failed. : %s",
                           ov_result_getresulttext(result));
      }
      break;
    case SYNC_DOWNLOAD_DONE:
      break;
    default:
      ov_logfile_error("sync_syncDownload: Unknown state %d", pinst->v_state);
      break;
  }
  return;
}
