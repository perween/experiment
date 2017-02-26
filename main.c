/* main file*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include "lstack.h"

struct job{
  lstack *results;
  size_t min_results, nzeros;
};


void *worker(void *arg)
{
  struct job *job = arg;
  while (lstack_size(job->results ) < job->min_results)
  {
    for ( i = 0; i < 10000; i++ )
    {
      if (lstack_push(job->results, i ) != 0 )
      {
        abort();
      }
    }
  }
  return NULL;
}

int main()
{
  int nthreads = sysconf(_SC_NPROCESSORS_ONLN);
  lstack_t results;
  struct job job = {
    .results = &results,
    .min_results = 4096,
    .nzeros = 2
  };

  lstack_init(&results, job.min_results + nthreads);

  /* spawn threads. */
  pthread_t threads[nthreads];

  printf("using %d threads%s.\n", nthreads, nthreads == 1? "": "s");
  for(int i = 0; i < nthreads; i++ )
    pthread_create(threads + i, NULL, worker, &job );

  /* print results.*/
  while((content = lstack_pop(&results)) != NULL )
  {
    printf("\ncontent of the file: %s", results->node_buffer->value );
  }

  return 0;
}

