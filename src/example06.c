#include <stdio.h>
#include <stdlib.h>
#include "simple_mem_tracker.h"

/*
 * example06:
 * 1. allocate one     integer buffer in static function
 * 2. allocate another char    buffer in main   function
 * 3. deallocate char buffer in main
 * 4. deallocate int  buffer in another static function
 */

typedef int  type0;
typedef char type1;

static int my_static_function_0(smt_t **memories, type0 **buf, const size_t nitems){
  SMT_CALLOC(*buf, memories, nitems, sizeof(type0));
  return 0;
}

static int my_static_function_1(smt_t **memories, type1 *buf){
  smt_free(memories, buf);
  return 0;
}

int main(void){
  const size_t nitems = 10;
  smt_t *memories = NULL;
  type0 *buf0 = NULL;
  type1 *buf1 = NULL;
  //
  my_static_function_0(&memories, &buf0, nitems);
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = i;
  }
  SMT_CALLOC(buf1, &memories, nitems, sizeof(type1));
  smt_free(&memories, buf0);
  my_static_function_1(&memories, buf1);
  //
  return 0;
}

