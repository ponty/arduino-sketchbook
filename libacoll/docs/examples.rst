Examples
===========

.. highlight:: c

All numbers are generated from source code using the simavr_ simulator.

-----------
Snippets
-----------

Template:

.. literalinclude:: generated_template.c

.. csv-table::
    :file: generated_examples.csv
    :header-rows: 1

--------------
Full programs
--------------

simple
--------------------------

Code (FreeMemory.pde):

..  [[[cog
..  cog.outl('' )
..  from pyavrutils import support
..  examples = support.find_examples('.')
..  for x in examples:
..      cog.outl('')
..      cog.outl(x)
..      cog.outl('')
..      cog.outl('.. literalinclude:: .' + x )
..  cog.outl('' )
..  ]]]


./Coll/examples/OpAmpDacTest/OpAmpDacTest.pde

.. literalinclude:: ../Coll/examples/OpAmpDacTest/OpAmpDacTest.pde

./Coll/examples/PWM/PWM.pde

.. literalinclude:: ../Coll/examples/PWM/PWM.pde

./Coll/examples/ReadPinMode/ReadPinMode.pde

.. literalinclude:: ../Coll/examples/ReadPinMode/ReadPinMode.pde

./Coll/examples/OpAmpDac/OpAmpDac.pde

.. literalinclude:: ../Coll/examples/OpAmpDac/OpAmpDac.pde

./Coll/examples/FreeMemory/FreeMemory.pde

.. literalinclude:: ../Coll/examples/FreeMemory/FreeMemory.pde

./Coll/examples/ReadVcc/ReadVcc.pde

.. literalinclude:: ../Coll/examples/ReadVcc/ReadVcc.pde

./Coll/examples/OpAmpDacTriangle/OpAmpDacTriangle.pde

.. literalinclude:: ../Coll/examples/OpAmpDacTriangle/OpAmpDacTriangle.pde

./Coll/examples/Filter/Filter.pde

.. literalinclude:: ../Coll/examples/Filter/Filter.pde

..  [[[end]]]



.. include:: links


