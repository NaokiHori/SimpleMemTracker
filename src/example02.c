#include <stdio.h>
#include <stdlib.h>
#include "simple_mem_tracker.h"

/*
 * example02:
 * 1. allocate two buffers having different types
 * 2. show their info
 * 3. deallocate them sequentially
 */

typedef int type0;
typedef char type1;

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
  type1 *buf1 = NULL;
  smt_t info;
  //
  SMT_CALLOC(buf0, &memories, nitems, sizeof(type0));
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = i;
  }
  SMT_CALLOC(buf1, &memories, nitems, sizeof(type1));
  for(size_t i = 0; i < nitems; i++){
    buf1[i] = 69+i;
  }
  if(smt_get_info(&info, memories, buf0) == 0){
    print_info(info);
  }
  if(smt_get_info(&info, memories, buf1) == 0){
    print_info(info);
  }
  smt_free(&memories, buf0);
  smt_free(&memories, buf1);
  //
  return 0;
}

