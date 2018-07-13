/*
 * queue.h
 *
 *  Created on: May 22, 2018
 *      Author: zzz
 */


/*
 * queue.c
 *
 *  Created on: May 22, 2018
 *      Author: zzz
 */
#include <stdlib.h>
#include <stdio.h>

#define SUCCESS 0
#define ERR_INVAL 1
#define ERR_NOMEM 2

#define FALSE 0
#define TRUE 1

struct queue_node_s {
  struct queue_node_s *next;
  void *data;
};

struct queue_s {
  struct queue_node_s *front;
  struct queue_node_s *back;
};

typedef struct queue_s queue_t;

int queue_destroy(queue_t *queue);

int queue_empty(queue_t *queue);

queue_t *queue_new(void);

void *queue_dequeue(queue_t *queue);

int queue_enqueue(queue_t *queue, void *data);
