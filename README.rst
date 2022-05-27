################
SimpleMemTracker
################

|License|_ |LastCommit|_

.. |License| image:: https://img.shields.io/github/license/NaokiHori/SimpleLinkedList
.. _License: https://opensource.org/licenses/MIT

.. |LastCommit| image:: https://img.shields.io/github/last-commit/NaokiHori/SimpleLinkedList/memory-tracker
.. _LastCommit: https://github.com/NaokiHori/SimpleLinkedList/commits/memory-tracker

Linked-list-based simple memory tracker (smt) in C99 with the following three functions

* ``smt_calloc`` (wrapper of ``malloc(3), calloc``)

* ``smt_free`` (wrapper of ``malloc(3), free``)

* ``smt_free_all`` (garbage collector)

**********
Dependency
**********

C compiler (later than C99), no dependency on external libraries

**********
Motivation
**********

Let's consider the following pseudo code for instance.

.. code-block:: c

   #include "my_library.h"


   static int my_static_function_0(void **buf){
     *buf = calloc( /* arguments */ );
     /**********************/
     /***                ***/
     /*** process buffer ***/
     /***                ***/
     /**********************/
     if( /* some error */ ){
       return -1;
     }
     return 0;
   }

   // simiar to my_static_function_0
   static int my_static_function_1(void **buf);
   static int my_static_function_2(void **buf);

   int my_library(const int arg){
     // buffers which will be allocated
     //   in static functions
     void *buf0 = NULL;
     void *buf1 = NULL;
     void *buf2 = NULL;
     // call functions, in which each buffer is allocated
     if(my_static_function_0(&buf0) != 0){
       // error, early return
       free(buf0);
       return -1;
     }
     if(my_static_function_1(&buf1) != 0){
       // error, early return
       free(buf0);
       free(buf1);
       return -1;
     }
     if(my_static_function_2(&buf2) != 0){
       // error, early return
       free(buf0);
       free(buf1);
       free(buf2);
       return -1;
     }
     /**********************/
     /***                ***/
     /*** process buffer ***/
     /***                ***/
     /**********************/
     // return success after deallocating all buffers
     free(buf0);
     free(buf1);
     free(buf2);
     return 0;
   }

There is one function which is open to the user (``my_library``), and are three static functions which are hidden to the user (e.g. ``my_static_function_0``).
The static functions are called sequentially by ``my_library``, in which each buffer (e.g. ``buf0``) is allocated (by calling ``calloc``) and processed.
When everything works properly, all allocated memories *inside this library* are freed (e.g. ``free(buf0,1,2)``) and returned (``return 0``) finally.

We notice a problem when one of the static function fails (``return -1``), which happens when the user input was invalid for instance.
Sometimes this library can simply kill the whole program and abort (e.g. using ``exit(EXIT_FAILURE)``), when the heap will be freed by the operating system automatically.
In other situations, however, the user might want this library to notify the failure (``return -1`` from ``my_library``) and do **not** terminate the whole process; in this case all temporally memories allocated in this library should be cleaned-up (or memory leaks occur).

To achieve this, buffers which have been allocated so far are freed in each condition in the above examples, which are quite noisy.

.. code-block:: text

   Actually it is sufficient to call ``free(buf0), free(buf1), free(buf2)``
     everywhere in the above example, since all buffers are NULL-initialised
     (``free(NULL)`` is well-defined).
   But I think it is sufficient to understand the message.

Also this is quite cumbersome since the programmer should remember all buffers allocated **including local buffers allocated in static functions**.
The cumbersomeness get worse especially when the functions are nested (imagine each static function calls other functions).

This library is developed to do this easily:

.. code-block:: c

   #include "my_library.h"
   #include "smt.h"


   static int my_static_function_0(&smt_t **memories, void **buf){
     *buf = smt_calloc(memories, /* arguments */ );
     /**********************/
     /***                ***/
     /*** process buffer ***/
     /***                ***/
     /**********************/
     if( /* some error */ ){
       return -1;
     }
     return 0;
   }

   // simiar to my_static_function_0
   static int my_static_function_1(void **buf);
   static int my_static_function_2(void **buf);

   int my_library(const int arg){
     // memory tracker (linked list)
     smt_t *memories = NULL;
     // buffers which will be allocated
     //   in static functions
     void *buf0 = NULL;
     void *buf1 = NULL;
     void *buf2 = NULL;
     // call functions, in which each buffer is allocated
     if(my_static_function_0(&memories, &buf0) != 0){
       // error, early return
       smt_free_all(&memories);
       return -1;
     }
     if(my_static_function_1(&memories, &buf1) != 0){
       // error, early return
       smt_free_all(&memories);
       return -1;
     }
     if(my_static_function_2(&memories, &buf2) != 0){
       // error, early return
       smt_free_all(&memories);
       return -1;
     }
     /**********************/
     /***                ***/
     /*** process buffer ***/
     /***                ***/
     /**********************/
     // return success after deallocating all buffers
     smt_free_all(&memories);
     return 0;
   }

where the differences are

* registration

   A structure ``smt_t`` is defined, which remember all allocated memories

* allocation

   ``calloc`` is replaced by its wrapper function ``smt_calloc``

* deallocation

   ``free`` is replaced by its wrapper function ``smt_free``.
   ``smt_free_all`` is available to deallocate all allocated memories.

All procedures are completely capsuled by this library and the user does not have to pay attention.

