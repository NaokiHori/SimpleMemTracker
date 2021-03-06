#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "simple_mem_tracker.h"

/*
 * example04:
 * 1. allocate five buffers having different types
 * 2. show their info
 * 3. deallocate them all at once
 */

typedef int           type0;
typedef char          type1;
typedef uint8_t       type2;
typedef double        type3;
typedef long long int type4;

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
  const size_t nitems = 20;
  smt_t *memories = NULL;
  type0 *buf0 = NULL;
  type1 *buf1 = NULL;
  type2 *buf2 = NULL;
  type3 *buf3 = NULL;
  type4 *buf4 = NULL;
  smt_t info;
  //
  SMT_CALLOC(buf0, &memories, nitems, sizeof(type0));
  for(size_t i = 0; i < nitems; i++){
    buf0[i] = -i;
  }
  SMT_CALLOC(buf1, &memories, nitems, sizeof(type1));
  for(size_t i = 0; i < nitems; i++){
    buf1[i] = 69+i;
  }
  SMT_CALLOC(buf2, &memories, nitems, sizeof(type2));
  for(size_t i = 0; i < nitems; i++){
    buf2[i] = i;
  }
  SMT_CALLOC(buf3, &memories, nitems, sizeof(type3));
  for(size_t i = 0; i < nitems; i++){
    buf3[i] = 1.*i;
  }
  SMT_CALLOC(buf4, &memories, nitems, sizeof(type4));
  for(size_t i = 0; i < nitems; i++){
    buf4[i] = 100*i;
  }
  if(smt_get_info(&info, memories, buf0) == 0){
    print_info(info);
  }
  if(smt_get_info(&info, memories, buf1) == 0){
    print_info(info);
  }
  if(smt_get_info(&info, memories, buf2) == 0){
    print_info(info);
  }
  if(smt_get_info(&info, memories, buf3) == 0){
    print_info(info);
  }
  if(smt_get_info(&info, memories, buf4) == 0){
    print_info(info);
  }
  smt_free_all(&memories);
  //
  return 0;
}

