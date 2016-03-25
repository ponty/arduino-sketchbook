libacoll is an arduino library for calculating free RAM size 

The sources are copied from http://www.arduino.cc/playground/Code/AvailableMemory

Links:
 * home: https://github.com/ponty/libacoll
 * documentation: http://ponty.github.com/libacoll

Features:
 - unit tests
 - examples
 - library size calculation
 - simulation
 - API documentation with doxygen
  
Known problems:
 - none
 
similar projects:
 - https://github.com/maniacbug/MemoryFree
 - http://www.arduino.cc/playground/Code/AvailableMemory  

Basic usage
============
::

    void setup() {
        Serial.begin(9600);
        Serial.print("freeMemory1()=");    
        Serial.println(freeMemory1());
    }
    
    void loop() {
    }

Manual Installation
=======================

http://arduino.cc/en/Guide/Environment#libraries

Automatic Installation
=======================

General
----------

 * install arduino_
 * install confduino_
 * install the library::

    # as root
    python -m confduino.libinstall https://github.com/ponty/libacoll/zipball/master

Ubuntu
----------
::

    sudo apt-get install arduino
    sudo apt-get install python-pip
    sudo pip install confduino
    sudo python -m confduino.libinstall https://github.com/ponty/libacoll/zipball/master

Ubuntu uninstall
-----------------
::

    sudo python -m confduino.libremove coll


.. _setuptools: http://peak.telecommunity.com/DevCenter/EasyInstall
.. _arduino: http://arduino.cc/
.. _python: http://www.python.org/
.. _simavr: http://gitorious.org/simavr
.. _swig: http://www.swig.org/
.. _confduino: https://github.com/ponty/confduino

