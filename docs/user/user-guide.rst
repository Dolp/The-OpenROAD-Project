User Guide
============

OpenROAD is divided into a number of tools that are orchestrated together to achieve RTL-to-GDS.
As of the current implmentation, the flow is divided into three stages:

1. Logic Synthesis: is performed by yosys_.
2. Floorplanning -> Global Routing: is performed by `OpenROAD App`_.
3. Detailed RoutingL is performed by `TritonRoute`_

To Run OpenROAD flow, we provide scripts to automate the RTL-to-GDS stages. 
Alternatively, you can run the individual steps manually.

[OPTION 1] RTL-to-GDS Flow
---------------------------

GitHub: https://github.com/The-OpenROAD-Project/OpenROAD-flow

Code Organization
^^^^^^^^^^^^^^^^^^^
This repository serves as an example RTL-to-GDS flow using the OpenROAD tools.

The two main components are:

1. **tools**: This directory contains the source code for the entire `openroad`
   app (via submodules) as well as other tools required for the flow. The script
   `build_openroad.sh` in this repository will automatically build the OpenROAD
   toolchain.

2. **flow**: This directory contains reference recipes and scripts to run      |
   designs through the flow. It also contains platforms and test designs.

Setup
^^^^^^

The flow has the following dependencies:
* OpenROAD
* KLayout
* TritonRoute
* Yosys

The dependencies can either be obtained from a pre-compiled build export or
built manually. See the [KLayout website](https://www.klayout.de/) for
installation instructions.

**Option 1: Installing build exports**

1.  Clone the OpenROAD-flow repository

.. code-block:: shell

    git clone --recursive https://github.com/The-OpenROAD-Project/OpenROAD-flow.git

2. Navigate to the "Releases" tab and download the latest release
3. Extract the tar to `OpenROAD-flow/tools/OpenROAD`
4. Update your shell environment

.. code-block:: shell
  
    source setup_env.sh


**Option 2: Building the tools using docker**

This build option leverages a multi-step docker flow to install the tools and
dependencies to a runner image. To follow these instructions, you must have
docker installed, permissions to run docker, and docker container network access
enabled. This step will create a runner image tagged as `openroad/flow`.

1.  Clone the OpenROAD-flow repository

.. code-block:: shell

    git clone --recursive https://github.com/The-OpenROAD-Project/OpenROAD-flow.git

2. Ensure your docker daemon is running and `docker` is in your PATH, then run
the docker build.

.. code-block:: shell
    
    ./build_openroad.sh

3. Start an interactive shell in a docker container using your user credentials

.. code-block:: shell

    docker run -u $(id -u ${USER}):$(id -g ${USER}) openroad/flow bash


**Option 3: Building the tools locally**

1. Reference the Dockerfiles and READMEs for the separate tools on the build steps
and dependencies.

.. code-block:: shell
    
    OpenROAD-flow/tools/OpenROAD/Dockerfile
    OpenROAD-flow/tools/yosys/Dockerfile
    OpenROAD-flow/tools/TritonRoute/Dockerfile

See the KLayout_ instructions for installing KLayout from source.

2. Run the build script

.. code-block:: shell
    
    ./build_openroad.sh

3. Update your shell environment

.. code-block:: shell
    
    source setup_env.sh

`klayout` must be added to the path manually.

Using the flow
^^^^^^^^^^^^^^^
See the flow [README](flow) for details about the flow and how
to run designs through the flow


[OPTION 2] Individual Flow Steps
---------------------------------

Logic Synthesis
^^^^^^^^^^^^^^^^


Floorplanning
^^^^^^^^^^^^^^


Placement
^^^^^^^^^^


Routing
^^^^^^^^

.. _yosys: https://github.com/The-OpenROAD-Project/yosys
.. _`OpenROAD App`: https://github.com/The-OpenROAD-Project/OpenROAD
.. _TritonRoute: https://github.com/The-OpenROAD-Project/TritonRoute
.. _KLayout: https://www.klayout.de