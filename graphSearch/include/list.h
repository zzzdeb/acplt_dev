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

struct listNode {
   struct listNode *next;
   struct listNode *prev;

   void* data;
};

typedef struct list {
	//this link always point to first Link
	struct listNode *head;

	//this link always point to last Link
	struct listNode *last;

	int dataSize;
  void (*printNode)(struct listNode*);
  void (*compare)(struct listNode*, struct listNode*);
} list_t;



list_t* constructList(size_t dataSize){
	list_t* list = malloc(sizeof(list_t));
	list->head = NULL;
	list->last = NULL;

	list->dataSize = dataSize;
}

int destructList(list_t* list){
	  if (list == NULL) {
	    return 1;
	  }

	  while (list->head!= NULL) {
	    struct listNode *node = list->head;
	    list->head = node->next;
	    free(node);
	  }

	  free(list);

	  return 0;
	}

//is list empty
bool isEmpty(list_t* list) {
   return list->head == NULL;
}

int length(list_t* list) {
   int length = 0;
   struct listNode *current;

   for(current = list->head; current != NULL; current = current->next){
      length++;
   }

   return length;
}

//display the list in from first to last
void displayForward(list_t* list) {

   //start from the beginning
   struct listNode *ptr = list->head;

   //navigate till the end of the list
   printf("\n[ ");

   while(ptr != NULL) {
      (*(list->printNode))(ptr);
      ptr = ptr->next;
   }

   printf(" ]");
}

//display the list from last to first
void displayBackward(list_t* list) {

   //start from the last
   struct listNode *ptr = list->last;

   //navigate till the start of the list
   printf("\n[ ");

   while(ptr != NULL) {

      //print data
  	 (*(list->printNode))(ptr);

      //move to next item
      ptr = ptr ->prev;

   }

}

//insert link at the first location
void insertFirst(list_t* list, void* data) {

   //create a link
   struct listNode *link = (struct listNode*) malloc(sizeof(struct listNode));

   link->data = data;

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
   struct listNode *link = (struct listNode*) malloc(sizeof(struct listNode));
   link->data = data;

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
struct listNode* deleteFirst(list_t* list) {

   //save reference to first link
   struct listNode *tempLink = list->head;

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

struct listNode* deleteLast(list_t* list) {
   //save reference to last link
   struct listNode *tempLink = list->last;

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

//delete a link with given key

//struct node* delete(list_t* list, int key) {
//
//   //start from the first link
//   struct node* current = list->head;
//   struct node* previous = NULL;
//
//   //if list is empty
//   if(list->head == NULL) {
//      return NULL;
//   }
//
//   //navigate through list
//   while(current->key != key) {
//      //if it is last node
//
//      if(current->next == NULL) {
//         return NULL;
//      } else {
//         //store reference to current link
//         previous = current;
//
//         //move to next link
//         current = current->next;
//      }
//   }
//
//   //found a match, update the link
//   if(current == list->head) {
//      //change first to point to next link
//  	 list->head = list->head->next;
//   } else {
//      //bypass the current link
//      current->prev->next = current->next;
//   }
//
//   if(current == list->last) {
//      //change last to point to prev link
//  	 list->last = current->prev;
//   } else {
//      current->next->prev = current->prev;
//   }
//
//   return current;
//}

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

#endif /* LIST_H_ */
