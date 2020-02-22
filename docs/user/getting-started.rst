Getting Started
================

Overview
---------

OpenROAD is a Chip Physical Design tool. 

The OpenROAD v1.0 tool, to be released in July 2020, will be capable of push-button,
DRC-clean RTL-to-GDS layout generation in a commercial FinFET process node. 
In its v1.0 form, it will be integrated on an incremental substrate provided
by the OpenDB_ database and the OpenSTA_ static timing engine. It will also offer users and
developers Tcl/Python scripting interfaces, and support SoC designs. OpenROAD v1.0 will thus
make substantial advances over the “files-based tool chain” seen at the project’s July 2019
"alpha" milestone. At the same time, the functionality of OpenROAD v1.0 will be highly limited
relative to that of commercial EDA tools that IC designers are familiar with. Further, the
development resources of the OpenROAD project are being largely focused on support of a
~July 2020 SoC tapeout in a commercial FinFET node.

Build Locally
--------------

The OpenROAD build requires the following packages:

  * cmake 3.9
  * gcc or clang
  * bison
  * flex
  * swig 3.0
  * boost
  * tcl 8.5
  * zlib

.. code-block:: shell

  git clone --recursive https://github.com/The-OpenROAD-Project/OpenROAD.git
  cd OpenROAD
  mkdir build
  cd build
  cmake ..
  make

.. note::

    If you are cloning OpenROAD within a corporate network, you might need to get past the proxy using:
    ``git config --global http.proxy <proxy_url>`` 

OpenROAD git submodules (cloned by the --recursive flag) are located in ``/src``.

The default build type is RELEASE to compile optimized code.
The resulting executable is in `build/resizer`.

Optional CMake variables passed as -D<var>=<value> arguments to CMake are show below.

.. code-block:: shell

    CMAKE_BUILD_TYPE DEBUG|RELEASE
    CMAKE_CXX_FLAGS - additional compiler flags
    TCL_LIB - path to tcl library
    TCL_HEADER - path to tcl.h
    ZLIB_ROOT - path to zlib
    CMAKE_INSTALL_PREFIX


The default install directory is `/usr/local`.
To install in a different directory with CMake use:

.. code-block:: shell

    cmake .. -DCMAKE_INSTALL_PREFIX=<prefix_path>


Alternatively, you can use the `DESTDIR` variable with make.

.. code-block:: shell

    make DESTDIR=<prefix_path> install


There are a set of regression tests in `/test`.

.. code-block:: shell
    
    test/regression
    src/resizer/test/regression


Quickstart
-----------

.. code-block:: shell

    openroad
        -help              show help and exit
        -version           show version and exit
        -no_init           do not read .openroad init file
        -no_splash         do not show the license splash at startup
        -exit              exit after reading cmd_file
        cmd_file           source cmd_file


OpenROAD sources the TCL command file `~/.openroad` unless the command
line option `-no_init` is specified.

OpenROAD then sources the command file cmd_file. Unless the `-exit`
command line flag is specified it enters and interactive TCL command
interpreter.

OpenROAD is run using TCL scripts. The following commands are used to read
and write design data.

.. code-block:: shell

    read_lef [-tech] [-library] filename
    read_def filename
    write_def [-version 5.8|5.6|5.5|5.4|5.3] filename
    read_verilog filename
    write_verilog filename
    read_db filename
    write_db filename


OpenROAD can be used to make a OpenDB database from LEF/DEF, or
Verilog (flat or hierarchical). Once the database is made it can be
saved as a file with the `write_db` command. OpenROAD can then read
the database with the `read_db` command without reading LEF/DEF or
Verilog.

The `read_lef` and `read_def` commands can be used to build an OpenDB
database as shown below. The `read_lef -tech` flag reads the
technology portion of a LEF file.  The `read_lef -library` flag reads
the MACROs in the LEF file.  If neither of the `-tech` and `-library`
flags are specified they default to `-tech -library` if no technology
has been read and `-library` if a technology exists in the database.

.. code-block:: shell

    read_lef liberty1.lef
    read_def reg1.def
    # Write the db for future runs.
    write_db reg1.db


The `read_verilog` command is used to build an OpenDB database as
shown below. Multiple verilog files for a hierarchical design can be
read.  The `link_design` command is used to flatten the design
and make a database.

.. code-block:: shell

    read_lef liberty1.lef
    read_verilog reg1.v
    link_design top
    # Write the db for future runs.
    write_db reg1.db


.. _OpenDB: https://github.com/The-OpenROAD-Project/OpenDB
.. _OpenSTA: https://github.com/The-OpenROAD-Project/OpenSTA
.. _`Toward an Open-Source Digital Flow: First Learnings from the OpenROAD Project`: https://vlsicad.ucsd.edu/Publications/Conferences/371/c371.pdf
.. _`doi:10.1145/3316781.3326334`: https://dl.acm.org/citation.cfm?id=3326334