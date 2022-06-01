#include <CUnit/CUnitCI.h>
#include "simple_mem_tracker.c"
#include "common.h"


typedef int type;

// normal use, remove nodes[0] from list with some data
static void test_smt_remove_case0(void){
  // answers
  const int retval_ans = 0;
  const size_t nitems_ans = 5;
  // args
  smt_t *node_root = NULL;
  smt_t **nodes = NULL;
  // exec
  for(size_t i = 0; i < nitems_ans; i++){
    nodes = init_sample_linked_list(&node_root, nitems_ans);
    {
      int retval = smt_remove(&node_root, nodes[i]->ptr);
      CU_ASSERT(retval_ans == retval);
      my_free(nodes[i]->ptr);
    }
    {
      size_t nitems;
      int retval = smt_get_nitems(&nitems, node_root);
      CU_ASSERT(retval_ans   == retval);
      CU_ASSERT(nitems_ans-1 == nitems);
    }
    cleanup_sample_linked_list(&node_root, nodes);
  }
}

// abnormal use, remove unregistered from list with some data
static void test_smt_remove_case1(void){
  // answers
  const size_t nitems_ans = 5;
  // args
  smt_t *node_root = NULL;
  smt_t **nodes = NULL;
  // exec
  nodes = init_sample_linked_list(&node_root, nitems_ans);
  {
    int retval = smt_remove(&node_root, NULL);
    CU_ASSERT(-1 == retval);
  }
  {
    size_t nitems;
    int retval = smt_get_nitems(&nitems, node_root);
    CU_ASSERT(0 == retval);
    CU_ASSERT(nitems_ans == nitems);
  }
  cleanup_sample_linked_list(&node_root, nodes);
}

CUNIT_CI_RUN(
    "remove",
    CUNIT_CI_TEST(test_smt_remove_case0),
    CUNIT_CI_TEST(test_smt_remove_case1)
);

