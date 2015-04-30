# Installation #

---

## What do I need? ##
The rxDeveloper is tested with
  * Ubuntu 10.04
  * Ubuntu 10.10
  * Ubuntu 11.04
  * Ubuntu 11.10
  * Linux Mint 11
  * Mac OS X
The main version works on **ROS Electric** and **ROS Fuerte**.
**ROS Diamondback** needs some changes in the dependency-handling for tinyxml and yaml-cpp, but a diamondback version is available, too. To get the sourcecode you also need **subversion**.

## How do I get it running? ROS Electric/Fuerte ##
  * download the sourcecode: "svn checkout http://rxdeveloper-ros-pkg.googlecode.com/svn/trunk/ rxdeveloper-ros-pkg-read-only"
  * make sure that you did the ROS environment setup and "rxdeveloper-ros-pkg-read-only" is in the _ROS\_PACKAGE\_PATH_.
  * compile: "rosmake rxDev" (if you get errors during compilation, an update of tinyxml to version 2.6x will help)
  * run: "rosrun rxDev rxdev" or start the binary "/rxDev/bin/rxdev"

## How do I get it running? ROS Diamondback ##
  * download the sourcecode: "svn checkout http://rxdeveloper-ros-pkg.googlecode.com/svn/tags/rxDev-diamond rxDev"
  * make sure that you did the ROS environment setup and "rxdeveloper-ros-pkg-read-only" is in the _ROS\_PACKAGE\_PATH_.
  * compile: "rosmake rxDev" (if you get errors during compilation, an update of tinyxml to version 2.6x will help)
  * run: "rosrun rxDev rxdev" or start the binary "/rxDev/bin/rxdev"