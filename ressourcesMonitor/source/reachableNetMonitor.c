
/******************************************************************************
*
*   FILE
*   ----
*   reachableNetMonitor.c
*
*   History
*   -------
*   2018-08-11   File created
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
#include "libov/ov_database.h"
#include "libov/ov_path.h"
#include "ksapi_commonFuncs.h"

#include "ksapiHelper.h"

#if OV_SYSTEM_NT
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#define STR_IMPL_(x) #x      //stringify argument
#define STR(x) STR_IMPL_(x)  //indirection to expand argument macros

/**
 * Allocate a new networkEntry and prepend it to the given networkList.
 *
 * @param list		Pointer to the networkList to prepend the new entry to
 * @param networkId	The network identifer string
 * @param hops		The number of hops to reach the network
 * @param nextHop	The address of the next hop to reach the network
 * @param routable	If traffic to the network can be routed through this node
 * @return			The operations result. May be OV_ERR_DBOUTOFMEMORY.
 */
static OV_RESULT addNetworkToList(networkList *list, OV_STRING networkId, OV_UINT hops, OV_STRING nextHop, OV_BOOL routable) {
	networkEntry *newEntry = ov_database_malloc(sizeof(networkEntry));
	if (!newEntry) {
		return OV_ERR_DBOUTOFMEMORY;
	}
	ov_string_setvalue(&newEntry->networkId, networkId);
	newEntry->hops = hops;
	ov_string_setvalue(&newEntry->nextHop, nextHop);
	newEntry->routable = routable;
	newEntry->next = list->first;

	list->first = newEntry;
	++list->num;
	return OV_ERR_OK;
}

/**
 * Clear a given networkList, while taking care of all the allocated memory. (Frees referenced OV_STRINGs and the
 * networkEntrys).
 *
 * @param list		Pointer to the networkList to be cleared
 */
static void clearNetworkList(networkList *list) {
	networkEntry *pEntry = list->first;
	while (pEntry) {
		networkEntry *next = pEntry->next;
		ov_string_setvalue(&pEntry->networkId, NULL);
		ov_string_setvalue(&pEntry->nextHop, NULL);
		ov_database_free(pEntry);
		pEntry = next;
	}
	list->first = NULL;
	list->num = 0;
}

/**
 * Add a networkEntry to the given networkList, if the network is unknown, or update the existing entry for this network
 * identifier, if a new route with less itermediate hops has been found.
 *
 * @param list		Pointer to the networkList to prepend the new entry to
 * @param networkId	The network identifer string
 * @param hops		The number of hops to reach the network
 * @param nextHop	The address of the next hop to reach the network
 * @param routable	If traffic to the network can be routed through this node
 * @return			The operations result. May be OV_ERR_DBOUTOFMEMORY.
 */
static OV_RESULT addNetworkIfBetter(networkList *list, OV_STRING networkId, OV_UINT hops, OV_STRING nextHop, OV_BOOL routable) {
	networkEntry *pEntry = list->first;
	while (pEntry) {
		if (ov_string_compare(pEntry->networkId, networkId) == OV_STRCMP_EQUAL) {
			if (hops < pEntry->hops) {
				pEntry->hops = hops;
				ov_string_setvalue(&pEntry->nextHop, nextHop);
				pEntry->routable = routable;
			}
			return OV_ERR_OK;
		}
		pEntry = pEntry->next;
	}
	return addNetworkToList(list, networkId, hops, nextHop, routable);
}


/**
 * Send a getVar query to the next server in the internal list of servers.
 *
 * @param pinst The reachableNetMonitor instance to work upon
 * @param pltc  Current time of execution
 * @return The result of the operation
 */
static OV_RESULT sendQueryToServer(OV_INSTPTR_ressourcesMonitor_reachableNetMonitor pinst, OV_TIME *pltc) {
	// Reset apiGet object
	ksapi_KSApiCommon_Reset_set(Ov_PtrUpCast(ksapi_KSApiCommon, &pinst->p_apiGet), FALSE);
	ksapi_KSApiCommon_Reset_set(Ov_PtrUpCast(ksapi_KSApiCommon, &pinst->p_apiGet), TRUE);

	// Parse active server from the list
	char host[INET6_ADDRSTRLEN+1];
	OV_UINT port;
	char server[OV_NAME_MAXLEN+1];
	sscanf(pinst->v_ovServersInt.value[pinst->v_nextQueryServer], "%*[^\t]\t%*[^\t]\t%" STR(INET6_ADDRSTRLEN) "[^\t]\t%u\t%" STR(OV_NAME_MAXLEN) "[^\t]",
			host, &port, server);

	// Gather data and send request
	ov_string_setvalue(&pinst->p_apiGet.v_serverHost, host);
	ov_string_setvalue(&pinst->p_apiGet.v_serverName, server);
	ov_memstack_lock();
	ov_string_setvalue(&pinst->p_apiGet.v_path, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pinst), 2));
	ov_memstack_unlock();
	ov_string_append(&pinst->p_apiGet.v_path, ".networks");
	pinst->v_queryTime = *pltc;
	ksapi_KSApiCommon_Submit_set(Ov_PtrUpCast(ksapi_KSApiCommon, &pinst->p_apiGet), FALSE);
	ksapi_KSApiCommon_Submit_set(Ov_PtrUpCast(ksapi_KSApiCommon, &pinst->p_apiGet), TRUE);

	return OV_ERR_OK;
}

/**
 * Uninlined function to finish the network list update cycle. It should be called, after all known servers have been
 * contacted and their connected networks have been added to the internal network list.
 *
 * This function will generate the output network list and clear the internal state.
 *
 * @param pinst The reachableNetMonitor instance to work upon
 * @param pltc  The current execution timestamp
 */
static void finishCommunicationLoop(OV_INSTPTR_ressourcesMonitor_reachableNetMonitor pinst, OV_TIME* pltc) {
	// Generate output string list from internal linked list
	Ov_SetDynamicVectorLength(&pinst->v_networks, pinst->v_networksInt.num, STRING);
	networkEntry* net = pinst->v_networksInt.first;
	OV_UINT i = 0;
	while (net) {
		ov_string_print(&pinst->v_networks.value[i], "%s\t%u\t%s\t%i", net->networkId, net->hops,
				net->nextHop ? net->nextHop : "", net->routable);
		net = net->next;
		++i;
	}
	// Clear linked list
	clearNetworkList(&pinst->v_networksInt);
	// Update flags
	pinst->v_lastUpdate = *pltc;
	pinst->v_querySent = FALSE;
}


void ressourcesMonitor_reachableNetMonitor_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_ressourcesMonitor_reachableNetMonitor pinst = Ov_StaticPtrCast(ressourcesMonitor_reachableNetMonitor, pfb);

    // Check if we are currently in a communication loop and waiting for a response
    if (!pinst->v_querySent) {
    	// If not, check if time has come to start the next communication loop
    	OV_TIME start_time;
    	ov_time_add(&start_time, &pinst->v_lastUpdate, &pinst->v_updateInterval);
    	if (ov_time_compare(pltc, &start_time) == OV_TIMECMP_BEFORE)
    		return;

    	// Okay, it's time to start a new update.
		// Step 1: Add local networks to networkList
    	for (OV_UINT i = 0; i < pinst->v_localNetworks.veclen; ++i) {
    		addNetworkToList(&pinst->v_networksInt, pinst->v_localNetworks.value[i], 0, "", TRUE); // TODO: more intelligent routable flag
    	}
    	// Step 2: Make an internal copy of the list of known OV servers
    	// TODO: Prevent modification with a set_accessor to avoid double copying?
    	Ov_SetDynamicVectorValue(&pinst->v_ovServersInt, pinst->v_ovServers.value, pinst->v_ovServers.veclen, STRING);

    	// Step 3: If there is any known ov Servers, send query to first server, else finish loop (after 0 iterations)
		pinst->v_nextQueryServer = 0;
		pinst->v_querySent = TRUE;
		if (pinst->v_nextQueryServer < pinst->v_ovServersInt.veclen) {
			sendQueryToServer(pinst, pltc);
		} else {
			finishCommunicationLoop(pinst, pltc);
		}


    } else {
        // If we are currently in a communication loop, check if ksapi call failed (due to internal error, external
    	// error or timeout) or has finished successfully. If not, continue waiting
    	if (pinst->p_apiGet.v_status != KSAPI_COMMON_REQUESTCOMPLETED
    			&& !ksapiHelper_checkForKSApiError(Ov_PtrUpCast(ksapi_KSApiCommon, &pinst->p_apiGet), &pinst->v_queryTime, pltc)) {
    		return;
    	}

		// If the ksapi request has been completed successfully, process the result
    	if (pinst->p_apiGet.v_status == KSAPI_COMMON_REQUESTCOMPLETED) {
			// Check if variable Operation was successful
			if (Ov_Fail(pinst->p_apiGet.v_varRes)) {
				ov_logfile_warning("ksapi request failed with server-side error: %s",
						ov_result_getresulttext(pinst->p_apiGet.v_varRes));
			} else if (pinst->p_apiGet.v_varValue.value.vartype != OV_VT_STRING_VEC) {
				ov_logfile_warning("ksapi request did not return a STRING_VEC: %s",
						ov_result_getresulttext(pinst->p_apiGet.v_varRes));
			} else {
				// Parse the resulting string vector and add networks to our internal list if appropriate
				OV_STRING_VEC *result = &pinst->p_apiGet.v_varValue.value.valueunion.val_string_vec;
		    	for (OV_UINT i = 0; i < result->veclen; ++i) {
		    		char network_name[1024];
		    		unsigned int hops;
		    		int routable;
		    		sscanf(result->value[i], "%1024[^\t]\t%u\t%*[^\t]\t%i", network_name, &hops, &routable);
		    		if (routable)
		    			addNetworkIfBetter(&pinst->v_networksInt, network_name, hops+1, pinst->p_apiGet.v_serverHost, TRUE);
		    	}
			}
    	}

		// Now, query the next server or finish the communication loop
		pinst->v_nextQueryServer += 1;
		if (pinst->v_nextQueryServer < pinst->v_ovServersInt.veclen) {
			sendQueryToServer(pinst, pltc);
		} else {
			finishCommunicationLoop(pinst, pltc);
		}

    }
}
