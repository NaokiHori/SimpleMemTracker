#include <stdio.h>
#include <stdlib.h>
#include "simple_mem_tracker.h"


typedef int type0;
typedef double type1;

int main(void){
  const size_t nitems = 10;
  smt_t *memories = NULL;
  type0 *buf0 = NULL;
  type1 *buf1 = NULL;
  smt_t info;
  //
  buf0 = smt_calloc(&memories, sizeof(type0), nitems);
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = i;
  }
  buf1 = smt_calloc(&memories, sizeof(type1), nitems);
  for(size_t i = 0; i < nitems; i++){
    buf1[i] = 1.*i;
  }
  if(smt_get_info(&info, memories, buf0) == 0){
    printf("ptr: %p, count: %zu size: %zu\n", info.ptr, info.count, info.size);
  }
  if(smt_get_info(&info, memories, buf1) == 0){
    printf("ptr: %p, count: %zu size: %zu\n", info.ptr, info.count, info.size);
  }
  smt_free(&memories, buf0);
  smt_free(&memories, buf1);
  //
  buf0 = smt_calloc(&memories, sizeof(type0), nitems);
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = i;
  }
  buf1 = smt_calloc(&memories, sizeof(type1), nitems);
  for(size_t i = 0; i < nitems; i++){
    buf1[i] = 1.*i;
  }
  smt_free_all(&memories);
  //
  return 0;
}

