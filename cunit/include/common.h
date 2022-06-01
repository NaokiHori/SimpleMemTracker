#if !defined(COMMON_H)
#define COMMON_H

#include "../include/simple_mem_tracker.h"

extern smt_t **init_sample_linked_list(smt_t **node_root, size_t nitems);
extern int cleanup_sample_linked_list(smt_t **node_root, smt_t **nodes);

#endif // COMMON_H
