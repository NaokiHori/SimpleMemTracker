################
SimpleMemTracker
################

|License|_

|WorkflowStatus|_ |DocDeployment|_

|UnitTest|_ |IntegrationTest|_

|LastCommit|_

.. |License| image:: https://img.shields.io/github/license/NaokiHori/SimpleMemTracker
.. _License: https://opensource.org/licenses/MIT

.. |WorkflowStatus| image:: https://github.com/NaokiHori/SimpleMemTracker/actions/workflows/ci.yml/badge.svg?branch=master
.. _WorkflowStatus: https://github.com/NaokiHori/SimpleMemTracker/actions/workflows/ci.yml
.. |DocDeployment| image:: https://github.com/NaokiHori/SimpleMemTracker/actions/workflows/documentation.yml/badge.svg
.. _DocDeployment: https://naokihori.github.io/SimpleMemTracker/

.. |UnitTest| image:: https://github.com/NaokiHori/SimpleMemTracker/blob/artifacts/.github/workflows/artifacts/badge_cunit.svg
.. _UnitTest: https://github.com/NaokiHori/Alpine-Dockerfiles/tree/cunit
.. |IntegrationTest| image:: https://github.com/NaokiHori/SimpleMemTracker/blob/artifacts/.github/workflows/artifacts/badge_valgrind.svg
.. _IntegrationTest: https://github.com/NaokiHori/Alpine-Dockerfiles/tree/valgrind

.. |LastCommit| image:: https://img.shields.io/github/last-commit/NaokiHori/SimpleMemTracker/master
.. _LastCommit: https://github.com/NaokiHori/SimpleMemTracker/commits/master

********
Overview
********

`Linked-list <https://github.com/NaokiHori/SimpleLinkedList>`_-based simple memory tracker (smt) in C99, where the following functions are implemented:

* ``smt_t *memories``: a structure (singly linked list) holding information of memory groups

* ``SMT_CALLOC`` (macro of ``smt_calloc``): register buffer information to ``memories`` after allocating it, e.g.:

   .. code-block:: c

      int  *buf0 = NULL;
      char *buf1 = NULL;
      SMT_CALLOC(buf0, &memories, nitems, sizeof(int ));
      SMT_CALLOC(buf1, &memories, nitems, sizeof(char));
      // buffers are non-NULL here

* ``SMT_ATTACH`` (macro of `smt_attach`): register buffer information to ``memories``, which has been allocated by the user already (**not** using ``smt_calloc``), e.g.:

   .. code-block:: c

      // being allocated externally (without using this lib)
      int *buf0 = calloc(nitems, sizeof(int));
      // register its info here
      SMT_ATTACH(buf0, &memories);

* ``smt_get_info``: check information of the registered buffer, e.g.:

   .. code-block:: c

      smt_t info;
      smt_get_info(&info, memories, buf0)

   giving

   .. code-block:: text

      0x600002ba0240 (registered as buf0 at src/example01.c:32) count=10 size=4

* ``smt_free``: erase buffer information from ``memories`` and deallocate the buffer itself, e.g.:

   .. code-block:: c

      smt_free(&memories, buf0);
      smt_free(&memories, buf1);
      // NULL is assigned to buffers here

* ``smt_detach``: erase buffer information from ``memories`` but **do not** deallocate the buffer itself, e.g.:

   .. code-block:: c

      smt_detach(&memories, buf0);
      smt_detach(&memories, buf1);
      // buffers are still alive here
      // users are responsible for freeing them by themselves

* ``smt_free_all``: deallocate all buffers associated with ``memories``, e.g.:

   .. code-block:: c

      smt_free_all(&memories);
      // NULL is assigned to all buffers included by the memory group

The last function could be useful for error handlings for instance.

Please refer to the `documentation <https://naokihori.github.io/SimpleMemTracker/>`_ for details.

Also some examples which are used as integration tests can be found in ``src`` directory.
Unit tests are available under ``cunit`` directory.

***********
Quick start
***********

C compiler (later than `C99`) is needed.

.. code-block:: console

   $ git clone https://github.com/NaokiHori/SimpleMemTracker

   $ cd SimpleMemTracker

   $ make

   $ ./example01.out

If you have `Valgrind <https://valgrind.org>`_ installed (or you might use `a Docker image <https://github.com/NaokiHori/Alpine-Dockerfiles/tree/valgrind>`_), you can check the memory leak tests by yourselves against all examples under ``src`` directory, which are performed as `a part of continuous integration processes <https://github.com/NaokiHori/SimpleMemTracker/blob/master/.github/workflows/ci.yml>`_.

*****
Usage
*****

No installation is needed and no external library dependency exists; after copying a header file ``include/simple_mem_tracker.h`` and its source file ``src/simple_mem_tracker.c`` to the corresponding places of your project, all functions implemented there can be used.

