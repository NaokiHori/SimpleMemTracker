#include <stdio.h>
#include <stdlib.h>
#include "simple_mem_tracker.h"

/*
 * example01:
 * 1. allocate one integer buffer
 * 2. show its info
 * 3. deallocate
 */

typedef int type0;

static int print_info(smt_t info){
  printf("%p (registered as %s at %s:%d) count=%zu size=%zu\n",
      info.ptr,
      info.ptrname,
      info.file,
      info.line,
      info.count,
      info.size
  );
  return 0;
}

int main(void){
  const size_t nitems = 10;
  smt_t *memories = NULL;
  type0 *buf0 = NULL;
  smt_t info;
  //
  SMT_CALLOC(buf0, &memories, nitems, sizeof(type0));
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = i;
  }
  if(smt_get_info(&info, memories, buf0) == 0){
    print_info(info);
  }
  smt_free(&memories, buf0);
  //
  return 0;
}

