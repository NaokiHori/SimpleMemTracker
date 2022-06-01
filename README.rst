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

* ``smt_calloc``: allocate buffer (wrapping ``calloc``) and register its information to ``memories``, e.g.:

   .. code-block:: c

      int  *buf0 = smt_calloc(&memories, nitems, sizeof(int ));
      char *buf1 = smt_calloc(&memories, nitems, sizeof(char));

* ``smt_get_info``: check information of the registered buffer, e.g.:

   .. code-block:: c

      smt_get_info(&info, memories, buf0)

   giving

   .. code-block:: text

      ptr: 0x600002e64240, allocated at src/main.c:16, count: 10, size: 4

* ``smt_free``: deallocate buffer (wrapping ``free``) and remove its information from ``memories``, e.g.:

   .. code-block:: c

      smt_calloc(&memories, buf0);
      smt_calloc(&memories, buf1);

* ``smt_free_all``: deallocate all buffers at once associated with ``memories``, e.g.:

   .. code-block:: c

      smt_free_all(&memories);

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

