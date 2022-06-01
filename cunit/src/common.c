#include <stdio.h>
#include <stdlib.h>
#include "common.h"


smt_t **init_sample_linked_list(smt_t **node_root, size_t nitems){
  // allocate "nitems" "pointer of smt_t" structures
  smt_t **nodes = NULL;
  nodes = calloc(nitems, sizeof(smt_t *));
  // allocate each node
  for(size_t i = 0; i < nitems; i++){
    // smt_t structure itself
    nodes[i] = calloc(1, sizeof(smt_t));
    // pointer to store data
    // count: 3, size: sizeof(int)
    size_t count = 3;
    size_t size = sizeof(int);
    nodes[i]->ptr = calloc(count, size);
    nodes[i]->count = count;
    nodes[i]->size = size;
  }
  // create link between node to node
  for(size_t i = 0; i < nitems-1; i++){
    nodes[i]->node_next = nodes[i+1];
  }
  // set root node
  *node_root = nodes[0];
  return nodes;
}

int cleanup_sample_linked_list(smt_t **node_root, smt_t **nodes){
  smt_t *node = *node_root;
  while(node != NULL){
    smt_t *node_next = node->node_next;
    free(node->ptr);
    free(node);
    node = node_next;
  }
  free(nodes);
  return 0;
}

