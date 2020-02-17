.. OpenROAD documentation master file, created by
   sphinx-quickstart on Mon Feb 17 12:17:21 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to OpenROAD's documentation!
====================================

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

How to navigate this documentation
-----------------------------------

* If you are a **user**, start with the *Getting Started* guide, and then move on to the *User Guide*.
* If you are willing to **contribute**, see the *Getting Involved* section.
* If you are a **developer** with EDA background, learn more about how you can use OpenROAD as the infrastructure for your tools in the *Developer Guide* section.

See *FAQs* and *Capabilities/Limitations* for relevant background on the project.

How to get in touch
--------------------

We maintain the following channels for communication:

+ Project homepage and news: https://theopenroadproject.org
+ Twitter: https://twitter.com/OpenROAD_EDA
+ Issues and bugs: https://github.com/The-OpenROAD-Project/OpenROAD/issues
+ Inquiries: openroad@eng.ucsd.edu

.. toctree::
   :maxdepth: 3
   :caption: Contents:

   user/getting-started
   user/user-guide
   user/tool-limitations
   contrib/getting-involved
   contrib/developer-guide
   user/faqs
   

.. _OpenDB: https://github.com/The-OpenROAD-Project/OpenDB
.. _OpenSTA: https://github.com/The-OpenROAD-Project/OpenSTA