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
	OV_INSTPTR_ov_object self;
	enum Datatype type;
	enum Level position;

	struct Data_s *pparent;
	enum RollerAction actionOfParent;

	OV_INT depth;
	list_t* actions;
} Data_t;

Data_t* constructData() {
	Data_t* data = ov_memstack_alloc(sizeof(Data_t));
	data->self = NULL;
	data->type = ROLLER;
	data->pparent = NULL;
	data->position = NEUTRAL;
	data->actionOfParent = UP;
	data->depth = 1;
	data->actions = NULL;
	return data;
}

void initData(Data_t *d, OV_INSTPTR_ov_object obj) {
	Ov_AbortIf(!d);
	d->self = obj;
	d->pparent = NULL;
	d->position = NEUTRAL;
	d->actionOfParent = UP;
	d->depth = 1;
	d->actions = NULL;
	if(obj) {
		if(Ov_CanCastTo(SMSTopologie_Rollgang, obj)) {
			d->type = ROLLER;
		} else if(Ov_CanCastTo(SMSTopologie_Schieber, obj)) {
			d->type = SCHIEBER;
//			d->position =
//			Ov_StaticPtrCast(SMSTopologie_Schieber, obj)->v_CurrentLocation;
		}
	}
	return;
}

// Data_t* copyData(Data_t* data){
//	if(!data)
//		return NULL;
//	Data_t* d = constructData();
//	*d = *data;
//	return d;
//}

/* compares 2 data as topology */
OV_BOOL is_samedata(Data_t *r1, Data_t *r2) {
	if(r1->self != r2->self) return 0;
	if(r1->type != r2->type) return 0;
	if(r1->type == SCHIEBER) {
		if(r1->position == NEUTRAL || r2->position == NEUTRAL)
			return 1;
		else if(r1->position != r2->position) return 0;
	}
	return 1;
}

OV_BOOL is_samedata_tmp(Data_t *r1, Data_t *r2) {
	if(r1->self != r2->self) return 0;
	if (r1->type != SCHIEBER && r2->type != SCHIEBER) {
		return 1;
	}
	if(r1->type == SCHIEBER) {
		if(r1->position == NEUTRAL || r2->position == NEUTRAL)
			return 1;
		else if(r1->position != r2->position) return 0;
	}
	return 1;
}

/* how to print data in listPrint */
void printData(struct listNode *node) {
	ov_logfile_info("(%d, %s, %d, %s)", ((Data_t *) node->data)->type,
		actionToStr[((Data_t *) node->data)->actionOfParent],
		((Data_t *) node->data)->position,
		((Data_t *) node->data)->self->v_identifier);
}

/* get children of parent for bfs and adds it to children*/
OV_RESULT get_children(list_t *children, Data_t *parent) {
	OV_INSTPTR_SMSTopologie_Rollgang pparentRG = NULL;
	OV_INSTPTR_SMSTopologie_Schieber pparentS = NULL;

	switch (parent->type) {
		case OFEN:
		case DREHTISCH:
		case ROLLER:
			pparentRG = Ov_StaticPtrCast(SMSTopologie_Rollgang, parent->self);
			break;
		case SCHIEBER:
			pparentS = Ov_StaticPtrCast(SMSTopologie_Schieber, parent->self);
			break;
		default:
			return 1;
	}
	// check validity
	// TODO:

	for (OV_UINT dir = 0; dir < MAXCHILDNUM; ++dir) {
		Data_t *childNode = constructData();
		childNode->depth = parent->depth + 1;
		childNode->actionOfParent = dir;
		childNode->pparent = parent;

		switch (parent->type) {
			case DREHTISCH:
			case OFEN:
			case ROLLER:
				pparentRG = Ov_StaticPtrCast(SMSTopologie_Rollgang, parent->self);
				switch (dir) {
					case RIGHT:
						// if roller
						childNode->type = ROLLER;
						childNode->self = Ov_StaticPtrCast(ov_object,
							Ov_GetParent(SMSTopologie_RG, pparentRG));
						if(childNode->self) break;

						// if schieber
						childNode->type = SCHIEBER;
						childNode->self = Ov_StaticPtrCast(ov_object,
							Ov_GetParent(SMSTopologie_SchieberUntenR, pparentRG));
						childNode->position = LEVEL0;
						if(childNode->self) break;

						childNode->self = Ov_StaticPtrCast(ov_object,
							Ov_GetParent(SMSTopologie_SchieberObenR, pparentRG));
						childNode->position = LEVEL1;
						if(childNode->self) break;
						break;
					case LEFT:
						childNode->type = ROLLER;
						childNode->self = Ov_StaticPtrCast(ov_object,
							Ov_GetChild(SMSTopologie_RG, pparentRG));
						if(childNode->self) break;

						// if schieber
						childNode->type = SCHIEBER;
						childNode->self = Ov_StaticPtrCast(ov_object,
							Ov_GetParent(SMSTopologie_SchieberUntenL, pparentRG));
						childNode->position = LEVEL0;
						if(childNode->self) break;

						childNode->self = Ov_StaticPtrCast(ov_object,
							Ov_GetParent(SMSTopologie_SchieberObenL, pparentRG));
						childNode->position = LEVEL1;
						if(childNode->self) break;
						break;
				}
				break;

			case SCHIEBER:
				pparentS = Ov_StaticPtrCast(SMSTopologie_Schieber, parent->self);
				childNode->type = ROLLER;
				switch (parent->position) {
					case NEUTRAL:
						// TODO: Find a way to add children reasonable
						switch (dir) {
							case UP:
								*childNode = *parent;
								childNode->position = LEVEL1;
								break;
							case RIGHT:
								break;
							case DOWN:
								*childNode = *parent;
								childNode->position = LEVEL0;
								break;
							case LEFT:
								break;
						}
						break;
					case LEVEL0:
						switch (dir) {
							case UP:
								childNode->self = parent->self;
								childNode->type = SCHIEBER;
								childNode->position = LEVEL1;
								break;
							case RIGHT:
								childNode->self = Ov_StaticPtrCast(ov_object,
									Ov_GetChild(SMSTopologie_SchieberUntenL, pparentS));
								break;
							case DOWN:
								break;
							case LEFT:
								childNode->self = Ov_StaticPtrCast(ov_object,
									Ov_GetChild(SMSTopologie_SchieberUntenR, pparentS));
								break;
						}
						break;
					case LEVEL1:
						switch (dir) {
							case UP:
								break;
							case RIGHT:
								childNode->self = Ov_StaticPtrCast(ov_object,
									Ov_GetChild(SMSTopologie_SchieberObenL, pparentS));
								break;
							case DOWN:
								childNode->self = parent->self;
								childNode->type = SCHIEBER;
								childNode->position = LEVEL0;
								break;
							case LEFT:
								childNode->self = Ov_StaticPtrCast(ov_object,
									Ov_GetChild(SMSTopologie_SchieberObenR, pparentS));
								break;
						}
						break;
				}
				break;
		}
		// adding child if ok
		if(childNode->self) {
			insertFirst(children, childNode);
		}
	}
	return 0;
}

/* execute BFS */
OV_RESULT BFS(list_t *path, Data_t *proot, Data_t *ptarget) {

	Ov_AbortIf(proot->pparent);
	Ov_AbortIf(proot->depth != 1);

	list_t *explored = constructList(sizeof(Data_t));
	explored->is_same = &is_samedata_tmp;
	explored->printNode = &printData;

	if(proot->type == SCHIEBER && proot->position == NEUTRAL) {
		Data_t *schieberL1 = constructData();
		*schieberL1 = *proot;
		schieberL1->position = LEVEL1;
		insertFirst(explored, schieberL1);

		Data_t *schieberL0 = constructData();
		*schieberL0 = *proot;
		schieberL0->position = LEVEL0;
		insertFirst(explored, schieberL0);
	} else {
		Data_t *tmpData = constructData();
		*tmpData = *proot;
		insertFirst(explored, tmpData);
	}

	listNode_t *frontier = explored->head;

	while (frontier) {
		// explored
		if(is_samedata_tmp(frontier->data, ptarget)) {
			//    	ov_logfile_info("found path");

			Data_t *currentNodeData = frontier->data;
			while (currentNodeData) {
				insertFirst(path, currentNodeData);
				currentNodeData = currentNodeData->pparent;
			};
			((Data_t*) path->last->data)->type = ptarget->type;
			((Data_t*) path->last->data)->actions = ptarget->actions;
			//			Data_t* currentNodeData =
			//copyData(frontier->data); 			while(currentNodeData){ 				insertFirst(path,
			//currentNodeData); 				currentNodeData = copyData(currentNodeData->pparent);
			//			}
			// free
			destructList(explored);
			return 0;
		}
		// add
		list_t *children = constructList(sizeof(Data_t));
		children->is_same = &is_samedata_tmp;
		get_children(children, frontier->data);
		listNode_t *child = NULL;
		listIterate(children, child)
		{
			if(!listFind(explored, child->data)) {
				insertLast(explored, child->data);
			}
			//			listPrint(explored);
		}
		destructList(children);

		// next
		frontier = frontier->next;
	}

	// free
	destructList(explored);
	return 1;
}

#define SEPERATOR " "
OV_RESULT initDataFromStr(Data_t* data, OV_STRING path) {
	if(!data || !path) return OV_ERR_BADPARAM;

	OV_UINT len = 0;
	OV_STRING* splited = ov_string_split(path, SEPERATOR, &len);

	OV_INSTPTR_ov_object pobj = ov_path_getobjectpointer(splited[0], 2);
	// param check
	if(!pobj) {
		ov_string_freelist(splited);
		return OV_ERR_BADPARAM;
	}
	initData(data, pobj);
	if(len == 1) {
		ov_string_freelist(splited);
		return 0;
	}
	//schieber
	if(data->type == SCHIEBER) {
		if(ov_string_compare(splited[1], "1") == OV_STRCMP_EQUAL) {
			data->position = LEVEL1;
			return 0;
		} else if(ov_string_compare(splited[1], "0") == OV_STRCMP_EQUAL) {
			data->position = LEVEL0;
			return 0;
			//todo: free mem
		} else
			ov_logfile_error("bad action %s on object %s", splited[1],
				data->self->v_identifier);
		return OV_ERR_BADPARAM;
	}

	//ofen || drehtisch
	data->actions = constructList(sizeof(Data_t));
	Data_t* actionNode = constructData();
	if(ov_string_compare(splited[1], "Heat") == OV_STRCMP_EQUAL) {
		if(isDataOfen(data)) {
			data->type = OFEN;
			*actionNode = *data;
			actionNode->actionOfParent = HEAT;
			insertLast(data->actions, actionNode);
		} else {
			ov_logfile_error("bad action %s on object %s", splited[1],
				data->self->v_identifier);
			ov_string_freelist(splited);
			return OV_ERR_BADPARAM;
		}
	} else if(ov_string_compare(splited[1], "Turn") == OV_STRCMP_EQUAL) {
		if(isDataDrehtisch(data)) {
			data->type = DREHTISCH;
			*actionNode = *data;
			actionNode->actionOfParent = TURN;
			insertLast(data->actions, actionNode);
		} else {
			ov_logfile_error("bad action %s on object %s", splited[1],
				data->self->v_identifier);
			ov_string_freelist(splited);
			return OV_ERR_BADPARAM;
		}
	} else {
		ov_logfile_error("bad action %s on object %s", splited[1],
			data->self->v_identifier);
		ov_string_freelist(splited);
		return OV_ERR_BADPARAM;
	}
	ov_string_freelist(splited);
	return 0;
}

OV_STRING get_param(Data_t* data) {
	if(!data) return NULL;
	switch (data->type) {
		case ROLLER:
			return NULL;
			break;
		case SCHIEBER:
			return NULL;
			break;
		case OFEN:
			return NULL;
			break;
		case DREHTISCH:
			return NULL;
			break;
		default:
			return NULL;
	}
}

OV_RESULT TGraph_graph_execute(OV_INSTPTR_TGraph_graph pinst) {
// variables
	OV_RESULT result = OV_ERR_OK;
	Data_t* recipes = NULL;

// param check
	if(!ov_path_getobjectpointer(pinst->v_topologie, 2)) {
		ov_logfile_error("topology could not be found");
		return OV_ERR_BADPARAM;
	}

// get start
	OV_STRING pathStr = NULL;
	ov_string_print(&pathStr, "%s/%s", pinst->v_topologie, pinst->v_start);
	OV_INSTPTR_ov_object proot = ov_path_getobjectpointer(pathStr, 2);
// param check
	if(!proot) {
		ov_logfile_error("start object could not be found");
		return OV_ERR_BADPARAM;
	}

// get recipes
	OV_UINT numberOfStations = 1 + pinst->v_recipe.veclen;
	recipes = Ov_HeapMalloc(numberOfStations * sizeof(Data_t));
	initData(&recipes[0], proot);
	Ov_StaticPtrCast(SMSTopologie_Schieber, proot)->v_CurrentLocation;
	for (OV_UINT i = 0; i < pinst->v_recipe.veclen; ++i) {
		ov_string_print(&pathStr, "%s/%s", pinst->v_topologie,
			pinst->v_recipe.value[i]);
		result = initDataFromStr(&recipes[i + 1], pathStr);
		// check
		if(Ov_Fail(result)) {
			Ov_HeapFree(recipes);
			return result;
		}
	}
	ov_string_setvalue(&pathStr, NULL);

// bfs
	OV_UINT pathLengthSum = 0;
	Data_t from;
	from = recipes[0];

	for (OV_UINT i = 1; i < numberOfStations; ++i) {
		// list
		list_t *path = constructList(sizeof(Data_t));
		path->printNode = &printData;
		result = BFS(path, &from, &recipes[i]);
		if(Ov_Fail(result)) {
			ov_logfile_error("No path found from %s to %s", from.self->v_identifier,
				recipes[i].self->v_identifier);
			//free
			Ov_HeapFree(recipes);
			destructList(path);
			return result;
		}
		//debug
		// listPrint(path);
		//cut start element

//		if(((Data_t*) path->head->data)->actions)
//			destructList(((Data_t*) path->head->data)->actions);
//		deleteFirst(path);

		// setting next from
		from = *(Data_t *) (path->last->data);
		from.pparent = NULL;
		from.depth = 1;
		from.actions = NULL;

		// expanding arrays
		listNode_t *element = NULL;
		OV_UINT pathLength = listLength(path)-1;
		listIterate(path, element)
		{
			//skipping first element
			if(!element->prev)
				continue;
			//debug
			// if(((Data_t *) element->data)->actions) {
			// 	((list_t*) ((Data_t *) element->data)->actions)->printNode = &printData;
			// 	listPrint(((Data_t *) element->data)->actions);
			// }

			pathLength +=
					((Data_t *) element->data)->actions ?
							listLength(((Data_t *) element->data)->actions) : 0;
		}
		pathLengthSum += pathLength;
		Ov_SetDynamicVectorLength(&pinst->v_pathNode, pathLengthSum, STRING);
		Ov_SetDynamicVectorLength(&pinst->v_pathDir, pathLengthSum, STRING);
		Ov_SetDynamicVectorLength(&pinst->v_parameter, pathLengthSum, STRING);

		// printing
		OV_STRING *currentNode = (pinst->v_pathNode.value)
				+ (pathLengthSum - pathLength);
		OV_STRING *currentAction = (pinst->v_pathDir.value)
				+ (pathLengthSum - pathLength);
		OV_STRING *currentParam = (pinst->v_parameter.value)
				+ (pathLengthSum - pathLength);

		OV_INT i = 0;
		listIterate(path, element)
		{
			//skipping first element
			if(!element->prev)
				continue;

			ov_string_setvalue(currentNode + i,
				((Data_t *) element->data)->self->v_identifier);
			ov_string_setvalue(currentAction + i,
				actionToStr[((Data_t *) element->data)->actionOfParent]);
			//todo: if first is action
			ov_string_setvalue(currentParam + i, "");
			i++;

			//actions
			if(((Data_t*) element->data)->actions) {
				listNode_t *actionElement = NULL;
				listIterate(((Data_t*)element->data)->actions, actionElement)
				{
					Data_t* actionElementData = (Data_t *) actionElement->data;
					ov_string_setvalue(currentNode + i,
						actionElementData->self->v_identifier);
					ov_string_setvalue(currentAction + i,
						actionToStr[actionElementData->actionOfParent]);

					switch (actionElementData->type) {
						case OFEN:
							ov_string_setvalue(currentParam + i, "150");
							break;
						case DREHTISCH:
							ov_string_setvalue(currentParam + i, "180");
							break;
						default:
							ov_string_setvalue(currentParam + i, "");
							break;
					}
					i++;
				}
			}
		}
		//free
		listIterate(path, element)
		{
			if(((Data_t *) element->data)->actions)
				destructList(((Data_t *) element->data)->actions);
		}
		destructList(path);
	}
	return result;
}



#endif /* DIJKSTRA_H_ */
