#ifndef OV_COMPILE_LIBRARY_ressourcesMonitor
#define OV_COMPILE_LIBRARY_ressourcesMonitor
#endif

#include "ressourcesMonitor.h"
#ifdef ov_library_open_ressourcesMonitor
#undef ov_library_open_ressourcesMonitor
#endif

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"

OV_RESULT ov_library_setglobalvars_ressourcesMonitor_new(void) {
	OV_RESULT result = OV_ERR_OK;

	result = ov_library_setglobalvars_ressourcesMonitor();
	if (Ov_Fail(result)) {
		return result;
	}

	OV_INSTPTR_fb_task pUrtask = Ov_DynamicPtrCast(fb_task,
			ov_path_getobjectpointer("/Tasks/UrTask", 2));

	OV_INSTPTR_ov_domain pData = Ov_StaticPtrCast(ov_domain,
			Ov_SearchChild(ov_containment, &pdb->root, "data"));
	if (!pData) {
		return result;
	}
	OV_INSTPTR_ov_domain pdiscovery = Ov_StaticPtrCast(ov_domain,
			Ov_SearchChild(ov_containment, pData, "mDNSDiscovery"));
	if (!pdiscovery) {
		result = Ov_CreateObject(ov_domain, pdiscovery, pData, "mDNSDiscovery");
		if (Ov_Fail(result)) {
			ov_logfile_error("Error: %s: couldnt create mDNSDiscovery in /data;",
					ov_result_getresulttext(result));
			return OV_ERR_OK;
		}
	}

	OV_INSTPTR_ressourcesMonitor_mDNSRegisterer preg = Ov_StaticPtrCast(
			ressourcesMonitor_mDNSRegisterer,
			Ov_SearchChild(ov_containment, pdiscovery, "reg"));
	if (!preg) {
		result = Ov_CreateObject(ressourcesMonitor_mDNSRegisterer, preg,
				pdiscovery, "reg");
		if (Ov_Fail(result)) {
			ov_logfile_error("Error: %s: couldnt create mDNSRegisterer;",
					ov_result_getresulttext(result));
		} else {
			preg->v_actimode = 1;
			preg->v_iexreq = TRUE;
			if (pUrtask) {
				Ov_Link(fb_tasklist, pUrtask, preg);
			} else {
				ov_logfile_warning(
						"lbalance_library_open: mDNSRegisterer not listed in UrTask");
			}
		}
	}

	OV_INSTPTR_ressourcesMonitor_ovDiscoverer pdisco = Ov_StaticPtrCast(
			ressourcesMonitor_ovDiscoverer,
			Ov_SearchChild(ov_containment, pdiscovery, "disco"));
	if (!pdisco) {
		result = Ov_CreateObject(ressourcesMonitor_ovDiscoverer, pdisco,
				pdiscovery, "disco");
		if (Ov_Fail(result)) {
			ov_logfile_error("Error: %s: couldnt create ovDiscoverer;",
					ov_result_getresulttext(result));
		} else {
			pdisco->v_actimode = 1;
			pdisco->v_iexreq = TRUE;
			if (pUrtask) {
				Ov_Link(fb_tasklist, pUrtask, pdisco);
			} else {
				ov_logfile_warning(
						"lbalance_library_open: ovDiscoverer not listed in UrTask");
			}
		}
	}

	return result;
}

OV_DLLFNCEXPORT OV_LIBRARY_DEF* ov_library_open_ressourcesMonitor(void) {
	/* local variables */
	static OV_LIBRARY_DEF* OV_LIBRARY_DEF_ressourcesMonitor_new;
	/*
	 *       replace the 'setglobalvars' function created by the code
	 * generator with a new one.
	 */
	OV_LIBRARY_DEF_ressourcesMonitor_new = ov_library_open_ressourcesMonitor_old();
	OV_LIBRARY_DEF_ressourcesMonitor_new->setglobalvarsfnc =
			ov_library_setglobalvars_ressourcesMonitor_new;
	return OV_LIBRARY_DEF_ressourcesMonitor_new;
}

