#include <stdio.h>
#include <stdlib.h>
#include "simple_mem_tracker.h"

/*
 * example05:
 * 1. allocate one integer buffer in static function
 * 2. deallocate in main
 */

typedef int type0;

static int my_static_function(smt_t **memories, type0 **buf, const size_t nitems){
  SMT_CALLOC(*buf, memories, nitems, sizeof(type0));
  return 0;
}

int main(void){
  const size_t nitems = 10;
  smt_t *memories = NULL;
  type0 *buf0 = NULL;
  //
  my_static_function(&memories, &buf0, nitems);
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = i;
  }
  smt_free(&memories, buf0);
  //
  return 0;
}

