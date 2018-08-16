
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


OV_DLLFNCEXPORT void ressourcesMonitor_reachableNetMonitor_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ressourcesMonitor_reachableNetMonitor pinst = Ov_StaticPtrCast(ressourcesMonitor_reachableNetMonitor, pfb);

	// TODO if not querySent
    	// TODO  if time since last update > ~ 30 sec)
			// TODO add local networks to networkList
			// TODO copy ov server list to internal variable
			// TODO set active server index to 0
			// TODO send getValue request to active server
    		// TODO set querySent = TRUE

    // TODO if querySent
		// TODO check if value was returned (or timeout?)
			// TODO if value is okay
				// TODO for each network in list
				// TODO parse entry
				// TODO use addNetworkIfBetter
			// TODO set active server to next server
			// TODO if active server >= server list size
				// TODO generate output string list from internal linked list
				// TODO clear linked list
				// TODO update last update timestamp set querySent = FALSE
			// TODO else
				// TODO send getValue request to active server

    return;
}
