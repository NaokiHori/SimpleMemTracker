#include <CUnit/CUnitCI.h>
#include "simple_mem_tracker.c"
#include "common.h"


typedef int type;

// normal use, attach to empty list
static void test_kernel_smt_attach_case0(void){
  size_t count = 10;
  size_t size = sizeof(type);
  type *ptr = my_calloc(count, size);
  // answers
  const int retval_ans = 0;
  // args
  smt_t *node_root = NULL;
  // exec
  {
    int retval = kernel_smt_attach(&node_root, ptr, count, size, "ptrname", __FILE__, __LINE__);
    CU_ASSERT(retval_ans == retval);
    CU_ASSERT(node_root->ptr == ptr);
    CU_ASSERT(node_root->count == count);
    CU_ASSERT(node_root->size == size);
  }
  {
    size_t nitems;
    int retval = kernel_smt_get_nitems(&nitems, node_root);
    CU_ASSERT(0 == retval);
    CU_ASSERT(1 == nitems);
  }
  my_free(ptr);
  my_free(node_root);
}

// normal use, attach to list with some data
static void test_kernel_smt_attach_case1(void){
  size_t count = 10;
  size_t size = sizeof(type);
  type *ptr = my_calloc(count, size);
  // answers
  const int retval_ans = 0;
  // args
  smt_t *node_root = NULL;
  smt_t **nodes = NULL;
  // exec
  {
    nodes = init_sample_linked_list(&node_root, 5);
    int retval = kernel_smt_attach(&node_root, ptr, count, size, "ptrname", __FILE__, __LINE__);
    CU_ASSERT(retval_ans == retval);
    CU_ASSERT(node_root->ptr == ptr);
    CU_ASSERT(node_root->count == count);
    CU_ASSERT(node_root->size == size);
  }
  {
    size_t nitems;
    int retval = kernel_smt_get_nitems(&nitems, node_root);
    CU_ASSERT(0 == retval);
    CU_ASSERT(6 == nitems);
  }
  cleanup_sample_linked_list(&node_root, nodes);
}

CUNIT_CI_RUN(
    "attach",
    CUNIT_CI_TEST(test_kernel_smt_attach_case0),
    CUNIT_CI_TEST(test_kernel_smt_attach_case1)
);

