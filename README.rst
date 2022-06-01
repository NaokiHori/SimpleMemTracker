################
SimpleMemTracker
################

|License|_

|DocDeployment|_

|WorkflowStatus|_ |UnitTest|_ |MemoryLeak|_

|LastCommit|_

.. |License| image:: https://img.shields.io/github/license/NaokiHori/SimpleMemTracker
.. _License: https://opensource.org/licenses/MIT

.. |DocDeployment| image:: https://github.com/NaokiHori/SimpleMemTracker/actions/workflows/documentation.yml/badge.svg
.. _DocDeployment: https://naokihori.github.io/SimpleMemTracker/

.. |WorkflowStatus| image:: https://github.com/NaokiHori/SimpleMemTracker/actions/workflows/ci.yml/badge.svg?branch=master
.. _WorkflowStatus: https://github.com/NaokiHori/SimpleMemTracker/actions/workflows/ci.yml

.. |UnitTest| image:: https://github.com/NaokiHori/SimpleMemTracker/blob/artifacts/.github/workflows/artifacts/badge_cunit.svg
.. _UnitTest: https://github.com/NaokiHori/Alpine-Dockerfiles/tree/cunit

.. |MemoryLeak| image:: https://github.com/NaokiHori/SimpleMemTracker/blob/artifacts/.github/workflows/artifacts/badge_valgrind.svg
.. _MemoryLeak: https://github.com/NaokiHori/Alpine-Dockerfiles/tree/valgrind

.. |LastCommit| image:: https://img.shields.io/github/last-commit/NaokiHori/SimpleMemTracker/master
.. _LastCommit: https://github.com/NaokiHori/SimpleMemTracker/commits/master

`Linked-list <https://github.com/NaokiHori/SimpleLinkedList>`_-based simple memory tracker (smt) in C99 with the following three functions

* ``smt_calloc`` (wrapper of ``malloc(3), calloc``)

* ``smt_free`` (wrapper of ``malloc(3), free``)

* ``smt_free_all`` (garbage collector)

By calling the last function, all allocated buffers associated with a memory pool can be released all at once, which could be useful for error handlings.

Please refer to the `documentation <https://naokihori.github.io/SimpleMemTracker/>`_ for details.

