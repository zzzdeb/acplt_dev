/*
 * search.c
 *
 *  Created on: May 22, 2018
 *      Author: zzz
 */
#include <stdio.h>

#include "queue.h"

typedef struct knoten_s {
	struct knoten_s *links;
	struct knoten_s *rechts;
	int wert;
} knoten;

void bfs(knoten *tree, void (*f)(int, void *), void *user) {

  if (tree == NULL) {

    return;

  }

  queue_t *queue = queue_new();

  queue_enqueue(queue, tree);

  while (!queue_empty(queue)) {

    knoten *k = queue_dequeue(queue);

    if (k->links != NULL) {

      queue_enqueue(queue, k->links);

    }

    if (k->rechts != NULL) {

      queue_enqueue(queue, k->rechts);

    }

    (*f)(k->wert, user);

  }

  queue_destroy(queue);

}
