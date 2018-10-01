/*
 * dijkstra.h
 *
 *  Created on: Jul 13, 2018
 *      Author: zzz
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "TGraph.h"

#include <search.h>
#include <string.h>

#include "libov/ov_debug.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "list.h"
#include "CException.h"

#include <stdlib.h>

/* print Node */
void printNode(struct listNode* node) {
	ov_logfile_info("(%s)", ((OV_INSTPTR_TGraph_Node) node->data)->v_identifier);
}
/* print Edge */
void printEdge(struct listNode* node) {
	ov_logfile_info("(%s)", ((OV_INSTPTR_TGraph_Edge) node->data)->v_identifier);
}

OV_BOOL is_same_int(OV_INT* i1, OV_INT* i2) {
	return *i1 == *i2;
}

int int_relation(void* a, void* b) {
	OV_INT a1 = *(OV_INT*) a;
	OV_INT b1 = *(OV_INT*) b;
	if(a1 > b1) return 1;
	if(a1 == b1) return 0;
	return -1;
}

OV_INT getMinChild(list_t* Nodes, OV_SINGLE* dist) {
	if(!listLength(Nodes)) return -1;

	listNode_t* child = NULL;
	OV_INT minChild = -1;
	OV_SINGLE minDist = -1;
	listIterate(Nodes, child)
	{
		OV_INT current = *(OV_INT*) child->data;
		if(dist[current] == -1) continue;
		if(dist[current] < minDist || minDist == -1) {
			minChild = current;
			minDist = dist[current];
		}
	}
	return minChild;
}

//dijkstra
list_p dijkstra_get_path(OV_INSTPTR_TGraph_Node n1, OV_INSTPTR_TGraph_Node n2) {
	/*param check*/
	if(!n1 || !n2) Throw(OV_ERR_BADPARAM);
	/* if target == source */
	if(n1 == n2) {
		return constructList(sizeof(OV_INSTPTR_TGraph_Edge));
	}

	OV_STRING path = ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object, n1), 2);
	OV_PATH resolved;
	ov_path_resolve(&resolved, NULL, path, 2);
	OV_INSTPTR_TGraph_graph Graph = Ov_StaticPtrCast(TGraph_graph,
		resolved.elements[resolved.size - 3].pobj);

	OV_INSTPTR_ov_domain Nodes = &(Graph->p_Nodes);
	OV_INT source = -1;
	OV_INT target = -1;
	OV_INSTPTR_TGraph_Node node = NULL;
//n = getKnonenZahl;
	OV_UINT n = 0;
	Ov_ForEachChildEx(ov_containment, Nodes, node, TGraph_Node)
	{
		n++;
	}

//OV_Node vert[] = allocate(n*p);
	OV_INSTPTR_TGraph_Node* V = ov_memstack_alloc(
		n * sizeof(OV_INSTPTR_TGraph_Node));
	OV_INT i = 0;
	Ov_ForEachChildEx(ov_containment, Nodes, node, TGraph_Node)
	{
		if(node == n1) source = i;
		if(node == n2) target = i;
		V[i++] = node;
	}

//OV_INT prev[] = allocate(n);
	OV_INT* prev = ov_memstack_alloc(n * sizeof(OV_INT));
	for (OV_UINT i = 0; i < n; ++i) {
		prev[i] = -1;
	}

//	OV_SINGLE dist[] = allocate(n);
	OV_SINGLE* dist = ov_memstack_alloc(n * sizeof(OV_SINGLE));
	for (OV_UINT i = 0; i < n; ++i) {
		dist[i] = -1;
	}
//	OV_INT adj[][] = allocate(n * n);
	OV_SINGLE** adj = ov_memstack_alloc(n * sizeof(OV_SINGLE*));
	for (OV_UINT i = 0; i < n; ++i) {
		adj[i] = ov_memstack_alloc(n * sizeof(OV_SINGLE));
		for (OV_UINT j = 0; j < n; ++j) {
			OV_INSTPTR_TGraph_Edge edge = TGraph_graph_areNodesLinked(V[i], V[j]);
			if(edge) {
				OV_UINT tmp1;
				OV_SINGLE* costs = TGraph_Edge_totalCost_get(edge, &tmp1);
				adj[i][j] = costs[0] > costs[1] ? costs[0] : costs[1];
			} else
				adj[i][j] = -1;
		}
	}

	//begin
	list_t* Unexplored = constructList(sizeof(OV_INT));
	Unexplored->compare = &int_relation;
	Unexplored->is_same = &is_same_int;

	for (OV_UINT i = 0; i < n; ++i) {
		OV_INT* tmp = ov_memstack_alloc(sizeof(OV_INT));
		*tmp = i;
		insertFirst(Unexplored, tmp);
	}
	dist[source] = 0;

	while (listLength(Unexplored)) {
		OV_INT current = getMinChild(Unexplored, dist);
		if(current == -1) break;

		if(current == target) {
			list_t* pathList = constructList(sizeof(OV_INSTPTR_TGraph_Edge));
			pathList->printNode = &printEdge;
			do {
				insertFirst(pathList,
					TGraph_graph_areNodesLinked(V[prev[current]], V[current]));
				current = prev[current];
			} while (current != source);
			listPrint(pathList);
			return pathList;
		}
		// Node with the least distance
		// will be selected first
//		remove u from Q
		listRemove(Unexplored, &current);

//		for v in adj[u]:
		for (OV_UINT v = 0; v < n; ++v) {
			if(adj[current][v] < 0) continue;

			OV_SINGLE alt = dist[current] + adj[current][v];
			if(alt < dist[v] || dist[v] == -1) { // A shorter path to v has been found
				dist[v] = alt;
				prev[v] = current;
			}
		}
	}
	return NULL;
}

/* execute dijksta */
//OV_RESULT dijkstra(list_t* graph, list_t *path, Data_t *proot, Data_t *ptarget) {
//
//	Ov_AbortIf(proot->prev);
//
//	list_t *explored = constructList(sizeof(Data_t));
//	explored->is_same = &is_samedata;
//	explored->printNode = &printData;
//
//	insertFirst(explored, proot);
//
//	listNode_t *frontier = explored->head;
//
//	while (frontier) {
//		// explored
//		if(is_samedata(frontier->data, ptarget)) {
//			//    	ov_logfile_info("found path");
//
//			Data_t *currentNodeData = frontier->data;
//			do{
//				insertFirst(path, currentNodeData);
//				currentNodeData = currentNodeData->prev;
//			} while (currentNodeData!=proot);
//			//			Data_t* currentNodeData =
//			//copyData(frontier->data); 			while(currentNodeData){ 				insertFirst(path,
//			//currentNodeData); 				currentNodeData = copyData(currentNodeData->prev);
//			//			}
//			// free
//			destructList(explored);
//			return 0;
//		}
//		// add
//		listNode_t * = NULL;
//		listIterate(((Data_t*)frontier->data)->neighbours, child)
//		{
//			if(!listFind(explored, child->data)) {
//				insertLast(explored, child->data);
//			}
//			//			listPrint(explored);
//		}
//
//		// next
//		frontier = frontier->next;
//	}
//
//// free
//	destructList(explored);
//	return 1;
//}
//
//#define SEPERATOR " "
//OV_RESULT initDataFromStr(Data_t* data, OV_STRING path) {
//	if(!data || !path) return OV_ERR_BADPARAM;
//
//	OV_UINT len = 0;
//	OV_STRING* splited = ov_string_split(path, SEPERATOR, &len);
//
//	OV_INSTPTR_ov_object pobj = ov_path_getobjectpointer(splited[0], 2);
//// param check
//	if(!pobj) {
//		ov_string_freelist(splited);
//		return OV_ERR_BADPARAM;
//	}
//	initData(data, pobj);
//	if(len == 1) {
//		ov_string_freelist(splited);
//		return 0;
//	}
////schieber
//	if(data->type == SCHIEBER) {
//		if(ov_string_compare(splited[1], "1") == OV_STRCMP_EQUAL) {
//			data->position = LEVEL1;
//			return 0;
//		} else if(ov_string_compare(splited[1], "0") == OV_STRCMP_EQUAL) {
//			data->position = LEVEL0;
//			return 0;
//			//todo: free mem
//		} else
//			ov_logfile_error("bad action %s on object %s", splited[1],
//				data->self->v_identifier);
//		return OV_ERR_BADPARAM;
//	}
//
////ofen || drehtisch
//	data->actions = constructList(sizeof(Data_t));
//	Data_t* actionNode = constructData();
//	if(ov_string_compare(splited[1], "Heat") == OV_STRCMP_EQUAL) {
//		if(isDataOfen(data)) {
//			data->type = OFEN;
//			*actionNode = *data;
//			actionNode->actionOfParent = HEAT;
//			insertLast(data->actions, actionNode);
//		} else {
//			ov_logfile_error("bad action %s on object %s", splited[1],
//				data->self->v_identifier);
//			ov_string_freelist(splited);
//			return OV_ERR_BADPARAM;
//		}
//	} else if(ov_string_compare(splited[1], "Turn") == OV_STRCMP_EQUAL) {
//		if(isDataDrehtisch(data)) {
//			data->type = DREHTISCH;
//			*actionNode = *data;
//			actionNode->actionOfParent = TURN;
//			insertLast(data->actions, actionNode);
//		} else {
//			ov_logfile_error("bad action %s on object %s", splited[1],
//				data->self->v_identifier);
//			ov_string_freelist(splited);
//			return OV_ERR_BADPARAM;
//		}
//	} else {
//		ov_logfile_error("bad action %s on object %s", splited[1],
//			data->self->v_identifier);
//		ov_string_freelist(splited);
//		return OV_ERR_BADPARAM;
//	}
//	ov_string_freelist(splited);
//	return 0;
//}
//
//OV_STRING get_param(Data_t* data) {
//	if(!data) return NULL;
//	switch (data->type) {
//		case ROLLER:
//			return NULL;
//			break;
//		case SCHIEBER:
//			return NULL;
//			break;
//		case OFEN:
//			return NULL;
//			break;
//		case DREHTISCH:
//			return NULL;
//			break;
//		default:
//			return NULL;
//	}
//}
#endif /* DIJKSTRA_H_ */
