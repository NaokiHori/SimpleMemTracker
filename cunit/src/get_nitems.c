#include <CUnit/CUnitCI.h>
#include "simple_mem_tracker.c"
#include "common.h"


// normal use, empty link list
static void test_smt_get_nitems_case0(void){
  // answers
  const size_t nitems_ans = 0;
  const int retval_ans = 0;
  // args
  size_t nitems;
  smt_t *node_root = NULL;
  // exec
  int retval = smt_get_nitems(&nitems, node_root);
  CU_ASSERT(retval_ans == retval);
  CU_ASSERT(nitems_ans == nitems);
}

// normal use, single node
static void test_smt_get_nitems_case1(void){
  // answers
  const size_t nitems_ans = 1;
  const int retval_ans = 0;
  // args
  size_t nitems;
  smt_t *node_root = NULL;
  // exec
  smt_t **nodes = init_sample_linked_list(&node_root, nitems_ans);
  int retval = smt_get_nitems(&nitems, node_root);
  CU_ASSERT(retval_ans == retval);
  CU_ASSERT(nitems_ans == nitems);
  cleanup_sample_linked_list(&node_root, nodes);
}

// normal use, many nodes
static void test_smt_get_nitems_case2(void){
  // answers
  const size_t nitems_ans = 100;
  const int retval_ans = 0;
  // args
  size_t nitems;
  smt_t *node_root = NULL;
  // exec
  smt_t **nodes = init_sample_linked_list(&node_root, nitems_ans);
  int retval = smt_get_nitems(&nitems, node_root);
  CU_ASSERT(retval_ans == retval);
  CU_ASSERT(nitems_ans == nitems);
  cleanup_sample_linked_list(&node_root, nodes);
}

// abnormal use, too many nodes
static void test_smt_get_nitems_case3(void){
  // answers
  const size_t nitems_ans = 2*USHRT_MAX;
  const int retval_ans = -1;
  // args
  size_t nitems;
  smt_t *node_root = NULL;
  // exec
  smt_t **nodes = init_sample_linked_list(&node_root, nitems_ans);
  int retval = smt_get_nitems(&nitems, node_root);
  CU_ASSERT(retval_ans == retval);
  cleanup_sample_linked_list(&node_root, nodes);
}

// abnormal use, recursive
static void test_smt_get_nitems_case4(void){
  // answers
  const size_t nitems_ans = 2;
  const int retval_ans = -1;
  // args
  size_t nitems;
  smt_t *node_root = NULL;
  // exec
  smt_t **nodes = init_sample_linked_list(&node_root, nitems_ans);
  node_root->node_next->node_next = node_root;
  int retval = smt_get_nitems(&nitems, node_root);
  CU_ASSERT(retval_ans == retval);
  // cleanup_sample_linked_list cannot be used, since recursive list is very strange
  // in real application, the whole process will be killed
  for(size_t i = 0; i < nitems_ans; i++){
    my_free(nodes[i]->ptr);
    my_free(nodes[i]);
  }
  my_free(nodes);
}

CUNIT_CI_RUN(
    "get_nitems",
    CUNIT_CI_TEST(test_smt_get_nitems_case0),
    CUNIT_CI_TEST(test_smt_get_nitems_case1),
    CUNIT_CI_TEST(test_smt_get_nitems_case2),
    CUNIT_CI_TEST(test_smt_get_nitems_case3),
    CUNIT_CI_TEST(test_smt_get_nitems_case4)
);

