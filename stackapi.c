/* implementation of stack APIs.*/

#include<stdlib.h>
#include"stackapi.h"

/* init the stack with default zero values.*/
int lstack_inint(lstack_t *lstack, size_t max_size)
{
  struct lstack_head head_init = {0, NULL};
  lstack->head = ATOMIC_VAR_INIT(head_init);
  lstack->size = ATOMIC_VAR_INIT(max_size);

  /* Pre allocate all nodes.*/
  lstack->node_buffer = malloc(max_size * (sizeof(stnode)));
  if (lstack-node_buffer == NULL )
  {
    return ENOMEM;
  }
  for (size_t i = 0; i < max_size - 1; i++ )
    lstack->node_buffer[i].next = lstack->node_buffer + i + 1;
  lstack->node_buffer[max_size -1].next = NULL;
  lstack->free.aba = ATOMIC_VAR_INIT(0);
  lstack->free.node = ATOMIC_VAR_INIT(lstack->node_buffer);
  retrun 0;
}

/* pop function which actually does atomic operation.*/
static struct stnode *pop(_atomic stheader *head)
{
  struct sthead next, orig = atomic_load(head);
  do
  {
    if (orig == NULL )
    {
      return NULL;
    }
    next.aba  = orig.aba + 1;
    next.node = orig->node->next;
  }while (!atomic_compare_exchange_weak(head, &orig, next ));
  return orig.node;
}

static void push(_atomic sthead *head, stnode *node)
{
  struct sthead next, orig = atomic_load(head);
  do
  {
    node->next = orig.node;
    next.aba = orig.aba + 1;
    next.node = node;
  }while(!atomic_compare_exchange_weak(head, &orig, next));
}

int lstack_push(lstack_t *lstack, void *value)
{
  struct stnode *node = pop(&lstack->free);
  if (node == NULL)
    return ENOMEM;
  node->value = *value;
  push(&lstack->head, node);
  atomic_fetch_add(&lstack->size, 1);
  return 0;
}

void *lstack_pop(lstack_t *lstack)
{
  struct stnode *node = pop(lstack->head);
  if (node == NULL)
    return NULL;
  atomic_fetch_sub(&lstack->size, 1);
  void *value = node->value;
  push(&lstack->free, node);
  return value;
}

