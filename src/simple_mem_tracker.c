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
#define LOGGING(...){                                      \
  fprintf(stderr, "[SMT-LOG: %s:%3d] ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__);                            \
}
#else
#define LOGGING(...)
#endif

#define ERROR(...){                                        \
  fprintf(stderr, "[SMT-ERR: %s:%3d] ", __FILE__, __LINE__); \
  fprintf(stderr, __VA_ARGS__);                            \
}

/* take care of actual allocation / deallocation */
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

/* get_nitems */
static int kernel_smt_get_nitems(size_t *nitems, smt_t *node_root){
  smt_t *node_curr = node_root;
  for(*nitems = 0; node_curr != NULL; node_curr = node_curr->node_next, (*nitems)++){
    if(*nitems >= NITEMS_MAX){
      return RETVAL_FAILURE;
    }
  }
  return RETVAL_SUCCESS;
}

static void smt_get_nitems(size_t *nitems, smt_t *node_root){
  // assert (# of nodes < NITEMS_MAX)
  if(kernel_smt_get_nitems(nitems, node_root) != RETVAL_SUCCESS){
    ERROR("More than %d buffers are allocated.\n", NITEMS_MAX);
    ERROR("This is not accepted by default.\n");
    ERROR("Define NITEMS_MAX explicitly to change this behaviour\n");
    exit(EXIT_FAILURE);
  }
}

/* search pointer */
static int search(smt_t **node_root, smt_t **node_curr, smt_t **node_prev, const void *ptr){
  /*
   * return RETVAL_SUCCESS if "ptr" is found,
   * otherwise return RETVAL_FAILURE
   */
  for(*node_prev = NULL, *node_curr = *node_root;
      *node_curr != NULL;
      *node_prev = *node_curr, *node_curr = (*node_curr)->node_next
  ){
    if((*node_curr)->ptr == ptr){
      return RETVAL_SUCCESS;
    }
  }
  return RETVAL_FAILURE;
}

/* functions which increase members */
static int kernel_smt_attach(smt_t **node_root, void *ptr, const size_t count, const size_t size, char *ptrname, char *file, const int line){
  // too many members should be rejected
  size_t nitems;
  if(kernel_smt_get_nitems(&nitems, *node_root) != RETVAL_SUCCESS){
    return RETVAL_FAILURE;
  }
  smt_t *node_prev = NULL;
  smt_t *node_curr = NULL;
  // check duplication
  // we should NOT be able to find the new pointer in the already-registered list
  if(search(node_root, &node_curr, &node_prev, ptr) != RETVAL_FAILURE){
    return RETVAL_FAILURE;
  }
  // allocate a node to hold ptr information
  node_curr = my_calloc(1, sizeof(smt_t));
  // next node is current root (new node is new root node)
  node_curr->node_next = *node_root;
  // assign other info
  node_curr->ptr = ptr;
  node_curr->count = count;
  node_curr->size = size;
  node_curr->ptrname = ptrname;
  node_curr->file = file;
  node_curr->line = line;
  // now curr node is the root
  *node_root = node_curr;
  LOGGING("ADD %p (registered as %s at %s:%d) count=%zu size=%zu\n",
      node_curr->ptr,
      node_curr->ptrname,
      node_curr->file,
      node_curr->line,
      node_curr->count,
      node_curr->size
  );
  return RETVAL_SUCCESS;
}

void *smt_calloc(smt_t **memories, const size_t count, const size_t size, const char ptrname[], const char file[], const int line){
  // allocate pointer
  void *ptr = my_calloc(count, size);
  if(kernel_smt_attach(memories, ptr, count, size, (char *)ptrname, (char *)file, line) != RETVAL_SUCCESS){
    ERROR("More than %d buffers are allocated.\n", NITEMS_MAX);
    ERROR("This is not accepted by default.\n");
    ERROR("Define NITEMS_MAX explicitly to change this behaviour\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *smt_attach(smt_t **memories, void *ptr, const char ptrname[], const char file[], const int line){
  // no information given, so assign zeros
  const size_t count = 0;
  const size_t size  = 0;
  kernel_smt_attach(memories, ptr, count, size, (char *)ptrname, (char *)file, line);
  return ptr;
}

/* functions which decrease members */
static int kernel_smt_detach(smt_t **node_root, const void *ptr){
  smt_t *node_prev = NULL;
  smt_t *node_curr = NULL;
  // we should be able to find it, since we know there is
  if(search(node_root, &node_curr, &node_prev, ptr) != RETVAL_SUCCESS){
    return RETVAL_FAILURE;
  }
  if(node_prev == NULL){
    // update root node
    *node_root = node_curr->node_next;
  }else{
    // update link
    node_prev->node_next = node_curr->node_next;
  }
  LOGGING("RMV %p (registered as %s at %s:%d) count=%zu size=%zu\n",
      node_curr->ptr,
      node_curr->ptrname,
      node_curr->file,
      node_curr->line,
      node_curr->count,
      node_curr->size
  );
  my_free(node_curr);
  return RETVAL_SUCCESS;
}

void smt_detach(smt_t **memories, const void *ptr){
  if(kernel_smt_detach(memories, ptr) != RETVAL_SUCCESS){
    ERROR("Cannot find %p in the allocated list\n", ptr);
    exit(EXIT_FAILURE);
  }
}

void smt_free(smt_t **memories, void *ptr){
  if(kernel_smt_detach(memories, ptr) != RETVAL_SUCCESS){
    ERROR("Cannot find %p in the allocated list\n", ptr);
    exit(EXIT_FAILURE);
  }
  my_free(ptr);
}

void smt_free_all(smt_t **memories){
  while(*memories != NULL){
    void *ptr = (*memories)->ptr;
    smt_free(memories, ptr);
  }
}

/* auxiliary functions */
int smt_get_info(smt_t *info, smt_t *memories, const void *ptr){
  size_t nitems;
  smt_get_nitems(&nitems, memories);
  for(size_t i = 0; i < nitems; memories = memories->node_next, i++){
    smt_t memory = *memories;
    if(ptr == memory.ptr){
      *info = memory;
      return RETVAL_SUCCESS;
    }
  }
  return RETVAL_FAILURE;
}

#undef LOGGING
#undef ERROR
#if defined(NITEMS_MAX_IS_NOT_GIVEN_BY_THE_USER)
#undef NITEMS_MAX
#endif
#undef RETVAL_SUCCESS
#undef RETVAL_FAILURE

