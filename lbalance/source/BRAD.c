
/******************************************************************************
 *
 *   FILE
 *   ----
 *   BRAD.c
 *
 *   History
 *   -------
 *   2018-12-28   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"
#include "lbalance_helper.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "object_helper.h"

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_constructor(OV_INSTPTR_ov_object pobj) {
  OV_RESULT                result = OV_ERR_OK;
  OV_INSTPTR_lbalance_BRAD pinst = Ov_StaticPtrCast(lbalance_BRAD, pobj);

  result = ov_object_constructor(pobj);

  OV_INSTPTR_fb_object pDiscoveryReg = Ov_DynamicPtrCast(
		  fb_object, ov_path_getobjectpointer("/data/mDNSDiscovery/reg", 2));
  OV_INSTPTR_fb_object pDiscoveryDisco = Ov_DynamicPtrCast(
		  fb_object, ov_path_getobjectpointer("/data/mDNSDiscovery/disco", 2));

  /***********************************************************/
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "syncTrigger",
                Ov_StaticPtrCast(fb_object, pinst), "init");
  /* ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "D", */
  /* Ov_StaticPtrCast(fb_object, &pinst->p), "D"); */

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_appMonitor);
  pinst->p_appMonitor.v_actimode = 1;
  pinst->p_appMonitor.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "appPaths",
                Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "appPaths");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_nbInformer);
  pinst->p_nbInformer.v_actimode = 1;
  pinst->p_nbInformer.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "sum",
                Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer), "sum");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "localCapacity",
                Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer), "cap");
  if (pDiscoveryReg)
	  ov_fb_connect(pDiscoveryDisco, "ovServers",
					Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer),
					"ovServers");
  else
	  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "ovServers",
					Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer),
					"ovServers");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "B",
                Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer), "B");
  if (pDiscoveryReg)
	  ov_fb_connect(pDiscoveryReg, "registerHostname",
					Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer), "myIP");
  else
	  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "selfHost",
					Ov_StaticPtrCast(fb_object, &pinst->p_nbInformer), "myIP");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_nbDB);
  pinst->p_nbDB.v_actimode = 1;
  pinst->p_nbDB.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "A",
                Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "A");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_outLSEOptimizer);
  pinst->p_outLSEOptimizer.v_actimode = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "B",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "enable");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "apps",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "localApps");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "appReq",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "localAppReq");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "loads",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "localAppLoads");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "IPs",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "neighbourIPs");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "loads",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "neighbourLoads");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "caps",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "neighbourCaps");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "sysInfos",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "neighbourSysInfos");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "sum",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "localLoad");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "localCapacity",
                Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "localCap");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_reqSender);
  pinst->p_reqSender.v_actimode = 1;
  pinst->p_reqSender.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "outApp", Ov_StaticPtrCast(fb_object, &pinst->p_reqSender),
                "outApp");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "outLoad", Ov_StaticPtrCast(fb_object, &pinst->p_reqSender),
                "outLoad");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer), "outIP",
                Ov_StaticPtrCast(fb_object, &pinst->p_reqSender),
                "destination");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "R",
                Ov_StaticPtrCast(fb_object, &pinst->p_reqSender), "R");
  if (pDiscoveryReg)
	  ov_fb_connect(pDiscoveryReg, "registerHostname",
					Ov_StaticPtrCast(fb_object, &pinst->p_reqSender), "selfHost");
  else
	  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "selfHost",
					Ov_StaticPtrCast(fb_object, &pinst->p_reqSender), "selfHost");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_reqReceiver);
  pinst->p_reqReceiver.v_actimode = 1;
  pinst->p_reqReceiver.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "D",
                Ov_StaticPtrCast(fb_object, &pinst->p_reqReceiver), "reset");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_inLSEOptimizer);
  pinst->p_inLSEOptimizer.v_actimode = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "R",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "enable");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_reqReceiver), "reqIPs",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "reqIPs");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_reqReceiver), "reqLoads",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "reqLoads");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "IPs",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "neighbourIPs");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "loads",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "neighbourLoads");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_nbDB), "caps",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "neighbourCaps");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_appMonitor), "sum",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "localLoad");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "localCapacity",
                Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "localCap");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_acceptNotifier);
  pinst->p_acceptNotifier.v_actimode = 1;
  pinst->p_acceptNotifier.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_reqReceiver), "reqIPs",
                Ov_StaticPtrCast(fb_object, &pinst->p_acceptNotifier),
                "reqIPs");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_inLSEOptimizer),
                "reqIndex",
                Ov_StaticPtrCast(fb_object, &pinst->p_acceptNotifier), "index");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "A",
                Ov_StaticPtrCast(fb_object, &pinst->p_acceptNotifier), "A");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_sendInitiator);
  pinst->p_sendInitiator.v_actimode = 1;
  pinst->p_sendInitiator.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "R",
                Ov_StaticPtrCast(fb_object, &pinst->p_sendInitiator), "clear");

  /***********************************************************/
  Ov_Link(fb_tasklist, pinst, &pinst->p_dsync);
  pinst->p_dsync.v_actimode = 1;
  pinst->p_dsync.v_iexreq = 1;
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer),
                "outApp", Ov_StaticPtrCast(fb_object, &pinst->p_dsync),
                "srcPath");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_sendInitiator),
                "accepted", Ov_StaticPtrCast(fb_object, &pinst->p_dsync), "EN");
  ov_fb_connect(Ov_StaticPtrCast(fb_object, pinst), "D",
                Ov_StaticPtrCast(fb_object, &pinst->p_dsync), "trigger");

  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_outLSEOptimizer), "outIP",
                Ov_StaticPtrCast(fb_object, &pinst->p_dsync), "destKS");
  if (pDiscoveryReg)
	  ov_fb_connect(pDiscoveryReg, "registerHostname",
					Ov_StaticPtrCast(fb_object, &pinst->p_dsync), "selfHost");
  else
	  ov_fb_connect(Ov_StaticPtrCast(fb_object, &pinst->p_mock), "selfHost",
					Ov_StaticPtrCast(fb_object, &pinst->p_dsync), "selfHost");

  return result;
}

OV_DLLFNCEXPORT OV_RESULT
                lbalance_BRAD_reset_set(OV_INSTPTR_lbalance_BRAD pinst, const OV_BOOL value) {
  pinst->v_reset = value;
  return 0;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_init_set(OV_INSTPTR_lbalance_BRAD pobj,
                                                 const OV_BOOL value) {
  if(value && !pobj->v_init) {
    Ov_WarnIf(!(pobj->v_status == LB_BRAD_D) || (pobj->v_status == LB_BRAD_I));
    pobj->v_status = LB_BRAD_B;
    pobj->v_B = 1;
    pobj->v_R = 0;
    pobj->v_A = 0;
    pobj->v_D = 0;
    ov_time_gettime(&pobj->v_timeLastEvent);
    ov_logfile_info("lbalance_BRAD: status inited by init");
  }
  pobj->v_init = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_B_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_B = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_R_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_R = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_A_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_A = value;
  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT lbalance_BRAD_D_set(OV_INSTPTR_lbalance_BRAD pobj,
                                              const OV_BOOL            value) {
  pobj->v_D = value;
  return OV_ERR_OK;
}

/**
 * Helper function to get the relevant time interval to check against in the
 * current BRAD phase
 */
static inline OV_SINGLE get_relevant_interval(OV_INSTPTR_lbalance_BRAD pinst) {
  switch(pinst->v_status) {
    case LB_BRAD_I:
      return pinst->v_T * 4;
    case LB_BRAD_B:
    case LB_BRAD_R:
    case LB_BRAD_A:
      return pinst->v_T;
    case LB_BRAD_D:
      return pinst->v_T;
    default:
      return 0;
  }
}

OV_DLLFNCEXPORT void lbalance_BRAD_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                              OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_BRAD pinst = Ov_StaticPtrCast(lbalance_BRAD, pfb);
  OV_SINGLE                interval = get_relevant_interval(pinst);
  OV_TIME                  now = {0};
  OV_TIME_SPAN             tstemp = {0};
  OV_BOOL                  changePhase = FALSE;

  ov_time_gettime(&now);
  ov_time_diff(&tstemp, &now, &(pinst->v_timeLastEvent));

  // Always restart in initializing state after a longer execution pause.
  if(tstemp.secs > (OV_UINT)2 * interval) {
    ov_logfile_info("lbalance_BRAD: (re)acitvated after long downtime. "
                    "Starting with Init phase.");
    pinst->v_status = LB_BRAD_I;
    pinst->v_B = 0;
    pinst->v_R = 0;
    pinst->v_A = 0;
    pinst->v_D = 0;
    ov_time_gettime(&pinst->v_timeLastEvent);
    return;
  }

  // Check if BRAD phase time has expired
  if((tstemp.secs > (OV_INT)interval) ||
     ((tstemp.secs == (OV_INT)interval) &&
      (tstemp.usecs >= (OV_UINT)(interval / 1e6)))) {
    changePhase = TRUE;
    ov_logfile_info("lbalance_BRAD: changing phase from %d", pinst->v_status);
  }

  // Do phase/state changes
  switch(pinst->v_status) {
    case LB_BRAD_I:
      if(changePhase) {
        pinst->v_B = 1;
        pinst->v_status = LB_BRAD_B;
        pinst->v_timeLastEvent = now;
      }
      return;
    case LB_BRAD_B:
      if(changePhase) {
        pinst->v_B = 0;
        pinst->v_R = 1;
        pinst->v_status = LB_BRAD_R;
        pinst->v_timeLastEvent = now;
      }
      return;
      break;
    case LB_BRAD_R:
      if(changePhase) {
        pinst->v_R = 0;
        pinst->v_A = 1;
        pinst->v_status = LB_BRAD_A;
        pinst->v_timeLastEvent = now;
      }
      break;
    case LB_BRAD_A:
      if(changePhase) {
        pinst->v_A = 0;
        pinst->v_D = 1;
        pinst->v_init = 0;
        pinst->v_status = LB_BRAD_D;
        pinst->v_timeLastEvent = now;
      }
      break;
    case LB_BRAD_D:
      if(changePhase) {
        pinst->v_D = 0;
        pinst->v_B = 1;
        pinst->v_status = LB_BRAD_B;
        pinst->v_timeLastEvent = now;
      }
      break;

      LB_CASEERROR
  }
  return;
}

