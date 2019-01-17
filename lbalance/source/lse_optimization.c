/*
 * @author Michael Thies <m.thies@plt.rwth-aachen.de>
 */

#include "lse_optimization.h"

#include "libov/ov_ov.h"
#include "libov/ov_macros.h"
#include "limits.h"


static const OV_UINT MAX_UINT = (~(OV_UINT)0);


/**
 * Calculate the target load of every known neighbour node.
 *
 * @param loads A vector of current loads of all known neighbour nodes. Must include our own node.
 * @param capacities A vector of load capacities of all known neighbour nodes (indices must match those of `loads`)
 * @return A vector of the target load of each known neighbour node (indices match those of the input parameters).
 */
static OV_UINT_VEC lse_optimization_calculate_target_load(OV_UINT_VEC loads, OV_UINT_VEC capacities) {
	OV_UINT numNodes = (loads.veclen < capacities.veclen) ? loads.veclen : capacities.veclen;

	// Calculate total load and total capacity
	OV_UINT totalLoad = 0, totalCapacity = 0;
	for (OV_UINT i = 0; i < numNodes; ++i) {
		totalLoad += loads.value[i];
		totalCapacity += capacities.value[i];
	}

	// Calculate individual node's target load
	OV_UINT_VEC res;
	Ov_SetDynamicVectorLength(&res, numNodes, UINT);
	for (OV_UINT i = 0; i < numNodes; ++i) {
		res.value[i] = totalLoad * capacities.value[i] / totalCapacity;
	}
	return res;
}


/**
 * Calculate the reduction of total squared error when moving an app from one node to the other.
 *
 * @param sourceNodeId Index of the source node of the planned movement in the `loads` and `targetLoads` lists.
 * @param destinationNodeId Index of the destination node of the planned movement in the `loads` and `targetLoads`
 *        lists.
 * @param appLoad The load of the application to be moved
 * @param loads The vector of all nodes' loads.
 * @param targetLoads The vector of all nodes' load targets
 * @return The reduction of the total squared error (postive = good)
 */
static OV_SINGLE lse_optimization_calculate_se_gain(
		OV_UINT sourceNodeId, OV_UINT destinationNodeId, OV_UINT appLoad, OV_UINT_VEC loads, OV_UINT_VEC targetLoads)
{
	OV_UINT sourceLoad = loads.value[sourceNodeId],
			destLoad = loads.value[destinationNodeId],
			sourceTarget = targetLoads.value[sourceNodeId],
			destTarget = targetLoads.value[destinationNodeId];

	OV_SINGLE square(OV_UINT a) { return (OV_SINGLE)a * (OV_SINGLE)a; }

	return square(sourceLoad - sourceTarget) - square(sourceLoad - appLoad - sourceTarget)
			+ square(destLoad - destTarget) - square(destLoad + appLoad - destTarget);
}



AppMovementSpec lse_optimization_chose_app(
		OV_UINT_VEC appLoads, OV_UINT_VEC nodeLoads, OV_UINT_VEC nodeCapacities, OV_UINT ownNodeId)
{
	OV_UINT_VEC targetLoads = lse_optimization_calculate_target_load(nodeLoads, nodeCapacities);

	AppMovementSpec res = {FALSE, 0, 0};
	OV_SINGLE maximumGain = 0.0f;
	for (OV_UINT appId = 0; appId < appLoads.veclen; ++appId) {
		for (OV_UINT nodeId = 0; nodeId < targetLoads.veclen; ++nodeId) {
			if (nodeId == ownNodeId)
				continue;

			OV_SINGLE seGain = lse_optimization_calculate_se_gain(
					ownNodeId, nodeId, appLoads.value[appId], nodeLoads, targetLoads);
			if (seGain > maximumGain) {
				res.appId = appId;
				res.nodeId = nodeId;
				maximumGain = seGain;
			}
		}
	}

	Ov_SetDynamicVectorLength(&targetLoads, 0, UINT);
	return res;
}


OV_UINT lse_optimization_chose_request(
		OV_UINT_VEC reqLoads, OV_UINT_VEC reqSourceIds, OV_UINT_VEC nodeLoads, OV_UINT_VEC nodeCapacities,
		OV_UINT ownNodeId)
{
	OV_UINT_VEC targetLoads = lse_optimization_calculate_target_load(nodeLoads, nodeCapacities);

	OV_UINT res = MAX_UINT;
	OV_SINGLE maximumGain = 0.0f;
	for (OV_UINT i = 0; i < reqLoads.veclen && i < reqSourceIds.veclen; ++i) {
		OV_UINT nodeId = reqSourceIds.value[i];
		if (nodeId >= targetLoads.veclen)
			continue; // This also filters out the MAX_UINT value if IP is unknown

		OV_SINGLE seGain = lse_optimization_calculate_se_gain(
				nodeId, ownNodeId, reqLoads.value[i], nodeLoads, targetLoads);
		if (seGain > maximumGain) {
			res = i;
			maximumGain = seGain;
		}
	}

	Ov_SetDynamicVectorLength(&targetLoads, 0, UINT);
	return res;
}


OV_UINT_VEC lse_optimization_lookup_requesting_nodes(OV_STRING_VEC requestIPs, OV_STRING_VEC nodeIPs) {
	OV_UINT_VEC res;
	Ov_SetDynamicVectorLength(&res, requestIPs.veclen, UINT);


	for (OV_UINT i = 0; i < requestIPs.veclen; ++i) {
		OV_UINT idx = MAX_UINT;
		for (OV_UINT j = 0; i < nodeIPs.veclen; ++i) {
			if (ov_string_compare(requestIPs.value[i], nodeIPs.value[j]) == OV_STRCMP_EQUAL) {
				idx = j;
				break;
			}
		}

		res.value[i] = idx;
	}

	return res;
}
