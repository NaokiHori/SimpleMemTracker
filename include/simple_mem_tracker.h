#if !defined(SIMPLE_MEM_TRACKER_H)
#define SIMPLE_MEM_TRACKER_H

#include <stddef.h> // size_t

typedef struct smt_t_ smt_t;

// DONT ASSIGN ANY VALUES DIRECTLY
// TO THIS STRUCTURE
struct smt_t_ {
  /* crutial information to construct linked list */
  // pointer to the allocated data (returned by my_calloc)
  void *ptr;
  // pointer to the next node
  smt_t *node_next;
  /* auxiliary information */
  // number of items
  size_t count;
  // size of each data
  size_t size;
  // file name in which this buffer is allocated
  char *file;
  // line number at which this buffer is allocated
  int line;
};

/* calloc */
extern void *my_smt_calloc(smt_t **memories, const size_t count, const size_t size, const char file[], const int line);
#define smt_calloc(memories, count, size) my_smt_calloc((memories), (count), (size), __FILE__, __LINE__)
/* free */
extern void smt_free(smt_t **memories, void *ptr);
extern void smt_free_all(smt_t **memories);
/* check allocated memory */
extern int smt_get_info(smt_t *info, smt_t *memories, const void *ptr);

#endif // SIMPLE_MEM_TRACKER_H
