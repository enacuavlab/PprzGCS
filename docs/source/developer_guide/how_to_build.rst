.. developer_guide how_to_build

How to build from sources
=========================

First, get all submodules:

.. code-block:: bash

    git submodule update --init --recursive

Ubuntu 20.04 and above
______________________
 
Install the dependencies:

.. code-block:: bash

    sudo apt install libsqlite3-dev libzip-dev qtbase5-dev libqt5svg5-dev libqt5texttospeech5-dev libproj-dev mesa-common-dev libglu1-mesa-dev

cd to the PprzGCS root, and build the needed libraries:

.. code-block:: bash

    ./build.sh libs

Now you should be able to build the app with:

.. code-block:: bash

    ./build.sh app


Ubuntu 18.04
____________

First, you need to `install Qt5.12.0 or above. <https://www.qt.io/download-open-source>`_

Setup in your `.bashrc` the environnement variable `Qt5_DIR` for cmake to find Qt5:

.. code-block:: bash

    export Qt5_DIR="/path/to/Qt/5.12.0/gcc_64/lib/cmake/Qt5"

Install the dependencies:

.. code-block:: bash

    sudo apt install libsqlite3-dev libzip-dev mesa-common-dev libglu1-mesa-dev

cd to the PprzGCS root, and build the needed libraries:

.. code-block:: bash

    build_proj=true ./build.sh libs

Now you should be able to build the app with:

.. code-block:: bash

    ./build.sh app
    
Running PprzGCS
_______________


To be able to run it from the paparazzi center, you need to add the ``build/pprzgcs`` directory to your ``$PATH``. Adapt this command and add it to your .bashrc:

.. code-block:: bash

    export PATH="/path/to/PprzGCS/build/pprzgcs:$PATH"

You should now be able to launch it from the Paparazzi center.

If you want to run it from the terminal, set the ``PAPARAZZI_HOME`` and the ``PAPARAZZI_SRC`` environment variables to your paparazzi path, then just run ``./build/pprzgcs/pprzgcs``


