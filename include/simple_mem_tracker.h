#if !defined(SIMPLE_MEM_TRACKER_H)
#define SIMPLE_MEM_TRACKER_H

#include <stddef.h> // size_t

typedef struct smt_t_ smt_t;

// DONT ASSIGN ANY VALUES DIRECTLY
// TO THIS STRUCTURE
struct smt_t_ {
  /*** crutial information to construct linked list ***/
  // pointer to the allocated data (returned by my_calloc)
  void *ptr;
  // pointer to the next node
  smt_t *node_next;
  /****************************************************/
  /*** auxiliary information ***/
  // number of items
  size_t count;
  // size of each data
  size_t size;
  // variable name when registered
  char *ptrname;
  // file name in which this buffer is allocated
  char *file;
  // line number at which this buffer is allocated
  int line;
  /*****************************/
};

/* allocate pointer having given size and add it to memory pool */
extern void *smt_calloc(smt_t **memories, const size_t count, const size_t size, const char ptrname[], const char file[], const int line);
#define SMT_CALLOC(ptr, memories, count, size) { \
  (ptr) = smt_calloc((memories), (count), (size), #ptr, __FILE__, __LINE__); \
}
/* add already allocated pointer to memory pool */
extern void *smt_attach(smt_t **memories, void *ptr, const char ptrname[], const char file[], const int line);
#define SMT_ATTACH(ptr, memories) { \
  (ptr) = smt_attach((memories), (ptr), #ptr, __FILE__, __LINE__); \
}
/* remove from memory pool but keep pointer allocation */
extern void smt_detach(smt_t **memories, const void *ptr);
/* remove from memory pool and deallocate pointer */
extern void smt_free(smt_t **memories, void *ptr);
extern void smt_free_all(smt_t **memories);
/* check allocated memory */
extern int smt_get_info(smt_t *info, smt_t *memories, const void *ptr);

#endif // SIMPLE_MEM_TRACKER_H
