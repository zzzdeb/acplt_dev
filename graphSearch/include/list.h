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

listNode_t* createNode(void* data){
	listNode_t* node = (listNode_t*)Ov_HeapMalloc(sizeof(listNode_t));
	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

list_t* constructList(size_t dataSize){
	list_t* list = Ov_HeapMalloc(sizeof(list_t));
	list->head = NULL;
	list->last = NULL;

	list->dataSize = dataSize;
	return list;
}

OV_RESULT ovlist_init(list_t* list){
	list->head = NULL;
	list->last = NULL;
	list->printNode = NULL;
	list->compare = NULL;
	list->is_same = NULL;
	return 0;
}

int destructList(list_t* list){
	  if (list == NULL) {
	    return 1;
	  }

	  while (list->head!= NULL) {
	    listNode_t *node = list->head;
	    list->head = node->next;
	    Ov_HeapFree(node);
	  }

	  Ov_HeapFree(list);

	  return 0;
	}

//is list empty
bool isEmpty(list_t* list) {
   return list->head == NULL;
}

int listLength(list_t* list) {
   int length = 0;
   listNode_t *current;

   for(current = list->head; current != NULL; current = current->next){
      length++;
   }

   return length;
}

//display the list in from first to last
void listPrint(list_t* list) {

   //start from the beginning
   listNode_t *ptr = list->head;

   //navigate till the end of the list
   printf("\n[ ");

   while(ptr != NULL) {
      (*(list->printNode))(ptr);
      ptr = ptr->next;
   }

   printf(" ]");
}

//insert link at the first location
void insertFirst(list_t* list, void* data) {

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
void insertLast(list_t* list, void* data)
{

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
listNode_t* deleteFirst(list_t* list) {

   //save reference to first link
   listNode_t *tempLink = list->head;

   //if only one link
   if(list->head->next == NULL){
      list->last = NULL;
   } else {
      list->head->next->prev = NULL;
   }

   list->head = list->head->next;
   //return the deleted link
   return tempLink;
}

//delete link at the last location

listNode_t* deleteLast(list_t* list) {
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

//find a link with given key

listNode_t* listFind(list_t* list, void* data) {

   //start from the first link
   listNode_t* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return NULL;
   }


   //navigate through list
   while(!(*(list->is_same))(current->data, data)) {
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

void listConcat(list_t* l1, list_t* l2){
	l1->last->next = l2->head;
	l2->head->prev = l1->last;
	l1->last = l2->last;
  Ov_HeapFree(l2);
}


//Macros
#define listIterate(list, element) for(element = (list != NULL) ? (list)->head: NULL; element != NULL; element = element->next)

#endif /* LIST_H_ */
