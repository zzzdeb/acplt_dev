/**
 * A library containing the algorithmic part of the distributed BRAD loadbalancing algorithm.
 *
 * * `lse_optimization_chose_app()` takes the gathered load and capacity information of all known hosts and the list of
 *   local applications and calculates, which application should be moved to improve the total squared error.
 * * `lse_optimization_chose_app()` takes the list of received movement requests and calculates which of them will
 *   reduce the total square error the most.
 *
 * @date Jan 2019
 * @author Michael Thies <m.thies@acplt.rwth-aachen.de>
 */

#include "libov/ov_ov.h"

/**
 * Return type struct for `lse_optimization_chose_app()` including app to move and node to move it to.
 */
typedef struct {
	OV_BOOL valid;		// If not TRUE, no possible movement has been found and the given ids are invalid
	OV_UINT appId;		// Index of the application to move
	OV_UINT nodeId;		// Index of the node to move the application to
} AppMovementSpec;


/**
 * Chose the best possible movement of one local application to an other node, according to the maximum gain in total
 * squared error.
 *
 * @param appLoads A vector of the loads of all apps, which are running on our node and can be moved.
 * @param nodeLoads A vector of current loads of all known neighbour nodes. Must include our own node.
 * @param nodeCapacities A vector of load capacities of all known neighbour nodes (indices must match those of
 *                       `nodeLoads`)
 * @param ownNodeId The index of our own node in `nodeLoads`, `nodeCapacities`
 * @return A AppMovementSpec struct, containing the proposed app's index to move and node's index to move it to, if any
 */
AppMovementSpec lse_optimization_chose_app(
		OV_UINT_VEC appLoads, OV_UINT_VEC nodeLoads, OV_UINT_VEC nodeCapacities, OV_UINT ownNodeId);


/**
 * Chose the best possible movement from the received movement requests
 *
 * @param reqLoads A vector of the load that should be transferred to our node, according to each single request
 * @param reqSourceIds A vector of the indices of the requesting node in the `nodeLoads` and `nodeCapacities` vectors
 *        for each request
 * @param nodeLoads A vector of current loads of all known neighbour nodes. Must include our own node.
 * @param nodeCapacities A vector of load capacities of all known neighbour nodes (indices must match those of
 * 	                     `nodeLoads`)
 * @param ownNodeId The index of our own node in `nodeLoads`, `nodeCapacities`
 * @return The index of the request to accept or MAX_UINT, if no request should be accepted
 */
OV_UINT lse_optimization_chose_request(
		OV_UINT_VEC reqLoads, OV_UINT_VEC reqSourceIds, OV_UINT_VEC nodeLoads, OV_UINT_VEC nodeCapacities,
		OV_UINT ownNodeId);


/**
 * Transform a vector of IP addresses to a vector of node Indexes according to a known list of node IP addresses.
 *
 * @param requestIPs The vector of IP addresses to transform
 * @param nodeIPs The vector of IP addresses of all known nodes
 * @return The vector of the indices of the nodes belonging to the given IP addresses
 *         Returns MAX_UINT (highest UINT number) if IP address is unknown
 */
OV_UINT_VEC lse_optimization_lookup_requesting_nodes(
		OV_STRING_VEC requestIPs, OV_STRING_VEC nodeIPs);
