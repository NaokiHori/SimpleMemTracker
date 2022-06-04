#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // size_t
#include <string.h> // strlen
#include <limits.h> // USHRT_MAX
#include "simple_mem_tracker.h"


// maximum number of items
// more than 65535, sufficiently large for my purpose
#if !defined(NITEMS_MAX)
#define NITEMS_MAX USHRT_MAX
#define NITEMS_MAX_IS_NOT_GIVEN_BY_THE_USER
#endif

// errorcode (return value)
#define RETVAL_SUCCESS ( 0)
#define RETVAL_FAILURE (-1)

#if defined(LOGGING_SMT)
#define LOGGING(...){                  \
  fprintf(stderr, "[SMT - LOGGING] "); \
  fprintf(stderr, __VA_ARGS__);        \
}
#else
#define LOGGING(...)
#endif

#define ERROR(...){                                            \
  fprintf(stderr, "[SMT - ERROR] %s:%d ", __func__, __LINE__); \
  fprintf(stderr, __VA_ARGS__);                                \
}

static void *my_calloc(const size_t count, const size_t size){
  void *ptr = calloc(count, size);
  if(ptr == NULL){
    ERROR("Memory allocation error\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void my_free(void *ptr){
  free(ptr);
  ptr = NULL;
}

static int smt_get_nitems(size_t *nitems, smt_t *node_root){
  smt_t *node_curr = node_root;
  for(*nitems = 0; node_curr != NULL; node_curr = node_curr->node_next, (*nitems)++){
    if(*nitems >= NITEMS_MAX){
      return RETVAL_FAILURE;
    }
  }
  return RETVAL_SUCCESS;
}

static int smt_add(smt_t **node_root, void *ptr, const size_t count, const size_t size, const char file[], const int line){
  // allocate a node to hold ptr information
  smt_t *node_new = my_calloc(1, sizeof(smt_t));
  // assign members
  node_new->ptr = ptr;
  node_new->node_next = *node_root; // next node is root
  node_new->count = count;
  node_new->size = size;
  size_t nitems_file = strlen(file);
  node_new->file = my_calloc(nitems_file+1, sizeof(char));
  memcpy(node_new->file, file, sizeof(char)*nitems_file);
  node_new->file[nitems_file] = 0; // NUL
  node_new->line = line;
  // now new code is the root
  *node_root = node_new;
  LOGGING("allocated   (%p), count: %zu, size: %zu\n", ptr, count, size);
  return RETVAL_SUCCESS;
}

static int smt_remove(smt_t **node_root, const void *ptr){
  size_t nitems;
  smt_get_nitems(&nitems, *node_root);
  smt_t *node_prev = NULL;
  smt_t *node_curr = *node_root;
  for(size_t i = 0; i < nitems; i++){
    if(node_curr->ptr == ptr){
      if(node_prev == NULL){
        *node_root = node_curr->node_next;
      }else{
        node_prev->node_next = node_curr->node_next;
      }
      LOGGING("deallocated (%p), count: %zu, size: %zu\n", node_curr->ptr, node_curr->count, node_curr->size);
      my_free(node_curr->file);
      my_free(node_curr);
      return RETVAL_SUCCESS;
    }else{
      node_prev = node_curr;
      node_curr = node_curr->node_next;
    }
  }
  return RETVAL_FAILURE;
}

void *my_smt_calloc(smt_t **memories, const size_t count, const size_t size, const char file[], const int line){
  size_t nitems;
  if(smt_get_nitems(&nitems, *memories) != RETVAL_SUCCESS){
    ERROR("More than %d buffers are allocated.\n", NITEMS_MAX);
    ERROR("This is not accepted by default.\n");
    ERROR("Define NITEMS_MAX explicitly to change this behaviour\n");
    exit(EXIT_FAILURE);
  }
  void *ptr = my_calloc(count, size);
  smt_add(memories, ptr, count, size, file, line);
  return ptr;
}

void smt_free(smt_t **memories, void *ptr){
  if(ptr != NULL){
    if(smt_remove(memories, ptr) != RETVAL_SUCCESS){
      ERROR("Cannot find %p in the allocated list\n", ptr);
      exit(EXIT_FAILURE);
    }
    my_free(ptr);
  }
}

void smt_free_all(smt_t **memories){
  while(*memories != NULL){
    void *ptr = (*memories)->ptr;
    smt_free(memories, ptr);
  }
}

int smt_get_info(smt_t *info, smt_t *memories, const void *ptr){
  size_t nitems;
  smt_get_nitems(&nitems, memories);
  for(size_t i = 0; i < nitems; memories = memories->node_next, i++){
    smt_t memory = *memories;
    if(ptr == memory.ptr){
      *info = memory;
      return 0;
    }
  }
  return -1;
}

#undef LOGGING
#undef ERROR
#if defined(NITEMS_MAX_IS_NOT_GIVEN_BY_THE_USER)
#undef NITEMS_MAX
#endif
#undef RETVAL_SUCCESS
#undef RETVAL_FAILURE

