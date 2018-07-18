/******************************************************************************
 *
 *   FILE
 *   ----
 *   list.c
 *
 *   History
 *   -------
 *   2018-07-13   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_ovdatastruct
#define OV_COMPILE_LIBRARY_ovdatastruct
#endif

#include "ovdatastruct.h"
#include "libov/ov_macros.h"
#include "list.h"

OV_DLLFNCEXPORT listNode_t* createNode(void* data) {
	listNode_t* node = (listNode_t*) Ov_HeapMalloc(sizeof(listNode_t));
	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

OV_DLLFNCEXPORT OV_RESULT ovlist_init(list_t* list) {
	list->head = NULL;
	list->last = NULL;
	list->printNode = NULL;
	list->compare = NULL;
	list->is_same = NULL;
	return 0;
}

OV_DLLFNCEXPORT list_t* constructList(size_t dataSize) {
	list_t* list = Ov_HeapMalloc(sizeof(list_t));
	ovlist_init(list);
	list->dataSize = dataSize;
	return list;
}

OV_DLLFNCEXPORT int destructList(list_t* list) {
	if(list == NULL) {
		return 1;
	}

	while (list->head != NULL) {
		listNode_t *node = list->head;
		list->head = node->next;
		Ov_HeapFree(node);
	}

	Ov_HeapFree(list);
	return 0;
}

OV_DLLFNCEXPORT bool isEmpty(list_t* list) {
	return list->head == NULL;
}

OV_DLLFNCEXPORT int listLength(list_t* list) {
	int length = 0;
	listNode_t *current;

	for (current = list->head; current != NULL; current = current->next) {
		length++;
	}

	return length;
}

//display the list in from first to last
OV_DLLFNCEXPORT void listPrint(list_t* list) {

	//start from the beginning
	listNode_t *ptr = list->head;

	//navigate till the end of the list
	printf("\n[ ");

	while (ptr != NULL) {
		(*(list->printNode))(ptr);
		ptr = ptr->next;
	}

	printf(" ]");
}

//insert link at the first location
OV_DLLFNCEXPORT void insertFirst(list_t* list, void* data) {

	//create a link
	listNode_t *link = createNode(data);

	if(isEmpty(list)) {
		//make it the last link
		list->last = link;
	} else {
		//update first prev link
		list->head->prev = link;
	}

	//point it to old first link
	link->next = list->head;

	//point first to new first link
	list->head = link;
}

//insert link at the last location
OV_DLLFNCEXPORT void insertLast(list_t* list, void* data) {

	//create a link
	listNode_t *link = createNode(data);

	if(isEmpty(list)) {
		//make it the last link
		list->head = link;
	} else {
		//make link a new last link
		list->last->next = link;

		//mark old last node as prev of new link
		link->prev = list->last;
	}

	//point last to new last node
	list->last = link;
}

//delete first item
OV_DLLFNCEXPORT listNode_t* deleteFirst(list_t* list) {

	//save reference to first link
	listNode_t *tempLink = list->head;

	//if only one link
	if(list->head->next == NULL) {
		list->last = NULL;
	} else {
		list->head->next->prev = NULL;
	}

	list->head = list->head->next;
	//return the deleted link
	return tempLink;
}

//delete link at the last location

OV_DLLFNCEXPORT listNode_t* deleteLast(list_t* list) {
	//save reference to last link
	listNode_t *tempLink = list->last;

	//if only one link
	if(list->head->next == NULL) {
		list->head = NULL;
	} else {
		list->last->prev->next = NULL;
	}

	list->last = list->last->prev;

	//return the deleted link
	return tempLink;
}

OV_DLLFNCEXPORT listNode_t* listRemove(list_t* list, void* data) {
	listNode_t* child;
	listIterate(list, child)
	{
		if((*list->is_same)(data, child->data)) {
			if(child->prev){
				child->prev->next = child->next;
			} else list->head = child->next;
			if(child->next)
				child->next->prev = child->prev;
			else list->last = child->prev;
			return child;
		}
	}
	return NULL;
}

//find a link with given key

OV_DLLFNCEXPORT listNode_t* listFind(list_t* list, void* data) {

//start from the first link
	listNode_t* current = list->head;

//if list is empty
	if(list->head == NULL) {
		return NULL;
	}

//navigate through list
	while (!(*(list->is_same))(current->data, data)) {
//if it is last node
		if(current->next == NULL) {
			return NULL;
		} else {
			//move to next link
			current = current->next;
		}
	}
//found a match
	return current;
}

OV_DLLFNCEXPORT void listConcat(list_t* l1, list_t* l2) {
	l1->last->next = l2->head;
	l2->head->prev = l1->last;
	l1->last = l2->last;
	Ov_HeapFree(l2);
}



OV_DLLFNCEXPORT void ovdatastruct_list_typemethod(
		OV_INSTPTR_fb_functionblock pfb, OV_TIME *pltc) {
	/*
	 *   local variables
	 */
//	OV_INSTPTR_ovdatastruct_list pinst = Ov_StaticPtrCast(ovdatastruct_list, pfb);

	return;
}

