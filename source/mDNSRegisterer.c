
/******************************************************************************
*
*   FILE
*   ----
*   mDNSRegisterer.c
*
*   History
*   -------
*   2018-07-20   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ressourcesMonitor
#define OV_COMPILE_LIBRARY_ressourcesMonitor
#endif


#include "ressourcesMonitor.h"
#include "libov/ov_macros.h"

#define __USE_XOPEN_EXTENDED
#include <dns_sd.h>
#include <unistd.h>
#include <limits.h>
#include <arpa/inet.h>


static OV_RESULT registerWithBonjour(OV_INSTPTR_ressourcesMonitor_mDNSRegisterer regObj) {
	// Get local hostname and append OV instance name
	const unsigned int MAX_NAME_LEN = HOST_NAME_MAX + 1 + OV_NAME_MAXLEN + 1;
	char servicename[MAX_NAME_LEN];
	char hostname[HOST_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);
	OV_ANY serverName;
	ov_vendortree_getservername(&serverName, NULL);
	strncpy(servicename, serverName.value.valueunion.val_string, OV_NAME_MAXLEN);
	ov_string_setvalue(&serverName.value.valueunion.val_string, NULL);
	unsigned int serverNameLen = strlen(servicename);
	servicename[serverNameLen] = '.';
	strncat(servicename + serverNameLen + 1, hostname, HOST_NAME_MAX);

	// Register at Bonjour/Avahi
	DNSServiceErrorType res = DNSServiceRegister(
			&regObj->v_sdRef,
			0,			/* flags */
			0,			/* interfaceIndex */
			servicename,	/* name */
			"_ov._tcp",	/* regtype */
			NULL,		/* domain */
			NULL,		/* host */
			htons(7509u),	/* port */ // FIXME: get actual server port
			0,			/* txtLen */
			NULL,		/* txtRecord */
			NULL,		/* callBack */ // TODO: Use callback to get asynchronous errors
			NULL		/* context */
	);

	if (res != kDNSServiceErr_NoError) {
		ov_logfile_error("Could not register OV server at Bonjour/Avahi: Errorcode %hi", res);
		return OV_ERR_GENERIC;
	} else {
		regObj->v_isRegistered = TRUE;
		return OV_ERR_OK;
	}
}


OV_DLLFNCEXPORT void ressourcesMonitor_mDNSRegisterer_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ressourcesMonitor_mDNSRegisterer pinst = Ov_StaticPtrCast(ressourcesMonitor_mDNSRegisterer, pfb);

    if (pinst->v_register && !pinst->v_isRegistered) {
    	registerWithBonjour(pinst);
    } else if (!pinst->v_register && pinst->v_isRegistered) {
    	DNSServiceRefDeallocate(pinst->v_sdRef);
		pinst->v_isRegistered = FALSE;
    }
}


OV_DLLFNCEXPORT void ressourcesMonitor_mDNSRegisterer_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ressourcesMonitor_mDNSRegisterer pinst = Ov_StaticPtrCast(ressourcesMonitor_mDNSRegisterer, pobj);

    if (pinst->v_isRegistered) {
		DNSServiceRefDeallocate(pinst->v_sdRef);
		pinst->v_isRegistered = FALSE;
	}

    /* set the object's state to "shut down" */
    fb_functionblock_shutdown(pobj);

    return;
}

