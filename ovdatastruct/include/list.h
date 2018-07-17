/*
 * list
 *
 *  Created on: May 22, 2018
 *      Author: zzz
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libov/ov_ov.h"
#include "libov/ov_macros.h"

//Macros
#define listIterate(list, element) for(element = (list != NULL) ? (list)->head: NULL; element != NULL; element = element->next)

typedef struct listNode {
	struct listNode *next;
	struct listNode *prev;

	void* data;
} listNode_t;

typedef struct list {
	//this link always point to first Link
	listNode_t *head;

	//this link always point to last Link
	listNode_t *last;

	int dataSize;
	void (*printNode)(listNode_t*);
	int (*compare)(void*, void*);
	OV_BOOL (*is_same)(void*, void*);
} list_t;

typedef list_t* list_p;

listNode_t* createNode(void* data);

OV_RESULT ovlist_init(list_t* list);

list_t* constructList(size_t dataSize);

int destructList(list_t* list);

//is list empty
bool isEmpty(list_t* list);

int listLength(list_t* list) ;

//display the list in from first to last
void listPrint(list_t* list);

//insert link at the first location
void insertFirst(list_t* list, void* data);

//insert link at the last location
void insertLast(list_t* list, void* data);

//delete first item
listNode_t* deleteFirst(list_t* list);
//delete link at the last location

listNode_t* deleteLast(list_t* list) ;

listNode_t* listRemove(list_t* list, void* data) ;

//find a link with given key

listNode_t* listFind(list_t* list, void* data);

//bool insertAfter(int key, int newKey, int data) {
//   //start from the first link
//   struct node *current = list->head;
//
//   //if list is empty
//   if(list->head == NULL) {
//      return false;
//   }
//
//   //navigate through list
//   while(current->key != key) {
//
//      //if it is last node
//      if(current->next == NULL) {
//         return false;
//      } else {
//         //move to next link
//         current = current->next;
//      }
//   }
//
//   //create a link
//   struct node *newLink = (struct node*) malloc(sizeof(struct node));
//   newLink->key = newKey;
//   newLink->data = data;
//
//   if(current == list->last) {
//      newLink->next = NULL;
//      list->last = newLink;
//   } else {
//      newLink->next = current->next;
//      current->next->prev = newLink;
//   }
//
//   newLink->prev = current;
//   current->next = newLink;
//   return true;
//}

//void* listGetMin(list_t* list) {
//	if(!listLength(Nodes))
//		return -1;
//
//	listNode_t* child = NULL;
//	OV_INT minChild = *(OV_INT*)Nodes->head->data;
//	OV_SINGLE minDist = dist[minChild];
//	listIterate(Nodes, child){
//		OV_INT current = *(OV_INT*)Nodes->head->data;
//		*((OV_INT*)child->data);
//	}
//	return minChild;
//}

void listConcat(list_t* l1, list_t* l2);

#endif /* LIST_H_ */
