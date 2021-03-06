
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
#include "libov/ov_vendortree.h"

#define __USE_XOPEN_EXTENDED
#include <dns_sd.h>
#include <unistd.h>
#if OV_SYSTEM_NT
#define HOST_NAME_MAX 255
#else
#include <limits.h>
#include <arpa/inet.h>
#endif


static OV_RESULT registerWithBonjour(OV_INSTPTR_ressourcesMonitor_mDNSRegisterer regObj) {
	// Get local hostname and append OV instance name
	const unsigned int MAX_NAME_LEN = HOST_NAME_MAX + 1 + OV_NAME_MAXLEN + 1;
	char servicename[MAX_NAME_LEN];
	char hostname[HOST_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);
	OV_ANY serverName;
	ov_vendortree_getservername(&serverName, NULL);
	strncpy(servicename, serverName.value.valueunion.val_string, OV_NAME_MAXLEN);
	unsigned int serverNameLen = strlen(servicename);
	servicename[serverNameLen] = '.';
	strncat(servicename + serverNameLen + 1, hostname, HOST_NAME_MAX);

	// Get KS server port (guess 7509 if unspecified)
	OV_INT port = ov_vendortree_getport();
	if (port < 1) {
		port = 7509;
	}

	// Register at Bonjour/Avahi
	DNSServiceErrorType res = DNSServiceRegister(
			&regObj->v_sdRef,
			0,			/* flags */
			0,			/* interfaceIndex */
			servicename,	/* name */
			"_ov._tcp",	/* regtype */
			".local",	/* domain */
			NULL,		/* host */
			htons((uint16_t)port),/* port */
			0,			/* txtLen */
			NULL,		/* txtRecord */
			NULL,		/* callBack */ // TODO: Use callback to get asynchronous errors
			NULL		/* context */
	);

	if (res != kDNSServiceErr_NoError) {
		ov_logfile_error("Could not register OV server at Bonjour/Avahi: Errorcode %i", res);
		return OV_ERR_GENERIC;
	} else {
		regObj->v_isRegistered = TRUE;
		return OV_ERR_OK;
	}
}


OV_DLLFNCEXPORT void ressourcesMonitor_mDNSRegisterer_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_ressourcesMonitor_mDNSRegisterer pinst = Ov_StaticPtrCast(ressourcesMonitor_mDNSRegisterer, pobj);

    /* do what the base class does first */
    fb_functionblock_startup(pobj);

	char hostname[HOST_NAME_MAX+1];
	gethostname(hostname, HOST_NAME_MAX);
	ov_string_setvalue(&pinst->v_registerHostname, hostname);
	ov_string_append(&pinst->v_registerHostname, ".local");


    return;
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

