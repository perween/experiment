#ifndef STACKAPI_H
#define STACKAPI_H

/* Define API which will be used for push and pop using atomic functions. */

#include<stdio.h>
#include<stdint.h>

#include<stdatomic.h>

/* structure which holds actual node data and next pointer.*/
typedef struct lstack_node{
  void *value;
  struct lstack_node *next;
}stnode;

/* structure which points to head of the linked list.*/
typedef struct lstack_head{
  unintptr_t aba;
  stnode *next;
}sthead;

/* main structure which keeps track of head pointer and free pointer. */
typedef struct lstack_t{
  stnode *node_buffer;
  _atomic sthead head, free;
  _atomic size_t size;
};

static inline size_t lstack_size(lstack_t *lstack)
{
  return atomic_load(&lstack->size);
}

static inline void ltack_free(lstack_t *lstack)
{
  free(lstack);
}

int lstack_init(lstack_t *lstack, size_t max_size);
int lstack_push(lstack_t *lstack, void *value);
void lstack_pop(lstack_t *lstack);

#endif

