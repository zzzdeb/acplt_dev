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
#include "list.h"
#include "CException.h"

#include <stdlib.h>

enum RollerAction {
	UP, RIGHT, DOWN, LEFT, HEAT, TURN, MAXCHILDNUM
};
const OV_STRING const actionToStr[MAXCHILDNUM] =
		{ [UP] = "up", [RIGHT] = "right", [DOWN] = "down", [LEFT] = "left", [HEAT
				]="heat", [TURN]="turn" };

enum Datatype {
	ROLLER, SCHIEBER, OFEN, DREHTISCH
};
enum Level {
	LEVEL0, LEVEL1, NEUTRAL
};

/* data type for list */
typedef struct Data_s {
	OV_INSTPTR_TGraph_Node self;

	struct Data_s *pparent;
	OV_SINGLE dist;
} Data_t;

void initData(Data_t *d, OV_INSTPTR_TGraph_Node obj) {
	Ov_AbortIf(!d);
	d->self = obj;
	d->pparent = NULL;
	d->dist = -1;
	return;
}

Data_t* constructData(OV_INSTPTR_TGraph_Node obj) {
	Data_t* data = ov_memstack_alloc(sizeof(Data_t));
	initData(data, obj);
	return data;
}

/* compares 2 data as topology */
OV_BOOL is_samedata(Data_t *r1, Data_t *r2) {
	if(r1->self == r2->self) return 1;
	return 0;
}

/* how to print data in listPrint */
//void printData(struct listNode *node) {
//	ov_logfile_info("(%d, %s, %d, %s)", ((Data_t *) node->data)->type,
//		actionToStr[((Data_t *) node->data)->actionOfParent],
//		((Data_t *) node->data)->self->v_identifier);
//}
/* get children of parent for bfs and adds it to children*/
//list_t* get_children(Data_t *parent) {
//	list_t *children = constructList(sizeof(Data_t));
//	children->is_same = &is_samedata;
//
//	OV_INSTPTR_TGraph_Node pparent = NULL;
//
//	pparent = Ov_StaticPtrCast(TGraph_Node, parent->self);
//// check validity
//// TODO:
//	OV_INSTPTR_TGraph_Edge out = NULL;
//	Ov_ForEachChildEx(TGraph_Start, pparent, out, TGraph_Edge)
//	{
//		OV_INSTPTR_TGraph_Node child = Ov_GetParent(TGraph_End, out);
//		if(child == nodes[(i + 1) % 2]) return TRUE;
//	}
//
//// adding child if ok
//	if(childNode->self) {
//		insertFirst(children, childNode);
//	}
//	return children;
//}
OV_BOOL is_same_int(){
	return 0;
}

int int_relation(void* a, void* b) {
	OV_INT a1 = *(OV_INT*)a;
	OV_INT b1 = *(OV_INT*)b;
	if(a1 > b1) return 1;
	if(a1 == b1) return 0;
	return -1;
}

OV_INT getMinChild(list_t* Nodes, OV_SINGLE* dist){
	return -1;
}

//dijkstra
list_t* dijkstra_get_path(OV_INSTPTR_TGraph_graph Graph,
		OV_INSTPTR_TGraph_Node n1, OV_INSTPTR_TGraph_Node n2) {
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
	OV_INT** adj = ov_memstack_alloc(n * sizeof(OV_INT*));
	for (OV_UINT i = 0; i < n; ++i) {
		adj[i] = ov_memstack_alloc(n * sizeof(OV_INT));
		for (OV_UINT j = 0; j < n; ++j) {
			OV_INSTPTR_TGraph_Edge edge = TGraph_graph_areNodesLinked(V[i], V[j]);
			if(edge) {
				adj[i][j] = edge->v_Length;
			} else
				adj[i][j] = -1;
		}
	}

	//begin
	list_t* Unexplored = constructList(sizeof(OV_INT));
	Unexplored->is_same = &is_same_int;
	Unexplored->compare = &int_relation;
	dist[source] = 0;

	while (listLength(Unexplored)) {
		OV_INT current = getMinChild(Unexplored, dist);

		if(current == target) {
			break;
		}
		// Node with the least distance
		// will be selected first
//		remove u from Q
		listRemove(Unexplored, &current);

//		for v in adj[u]:
		for (OV_UINT v = 0; v < n; ++v) {
			if(adj[current][v] < 0) continue;

			OV_SINGLE alt = dist[current] + adj[current][v];
			if(alt < dist[current]) {            // A shorter path to v has been found
				dist[v] = alt;
				prev[v] = current;
			}
		}
	}
}

#endif /* DIJKSTRA_H_ */
