# Tutorials #

---

This video was made with rxDeveloper version 1.0b, there are some fancy new features in the newer versions!
<a href='http://www.youtube.com/watch?feature=player_embedded&v=RvGsbhp3kWE' target='_blank'><img src='http://img.youtube.com/vi/RvGsbhp3kWE/0.jpg' width='425' height=344 /></a>

---




---


## Launchfiles ##
### creating a launchfile ###
![http://rxdeveloper-ros-pkg.googlecode.com/files/component-connector.png](http://rxdeveloper-ros-pkg.googlecode.com/files/component-connector.png)
In the **Component Connector**-tab you can drag & drop the available nodes to the main window. Other tags, or nodes/nodelets without a [specificationfiles](Tutorial#Specificationfiles.md) can be added by clicking the corresponding buttons.
The toolbar contains buttons to switch between three work modes:
  1. Drag&Drop
  1. Remap
  1. Delete
Switching to the **remap**-mode lets you draw a connection between two nodes by pressing the left mouse button on a node and releasing it over another node. The first node will include the `<remap>-tag` in the launchfile. A **remap-editor** will show you possible remap-topics which you can select or enter your own text.

![http://rxdeveloper-ros-pkg.googlecode.com/files/remap-edit.png](http://rxdeveloper-ros-pkg.googlecode.com/files/remap-edit.png)

All Items you add have their own item-editor, here you can add detailed information or in some cases other items.
After that you can save the launchfile.

### editing a launchfile ###
You can load a launchfile and edit it as described in the "creating a launchfile"-step. If the launchfile was saved with the rxDeveloper before, the item positions get recovered automatically.

### components / a place for includefiles ###
![http://rxdeveloper-ros-pkg.googlecode.com/files/component.png](http://rxdeveloper-ros-pkg.googlecode.com/files/component.png)
The **Components**-tab of the sidebar can be used for often used includefiles. If you save a launchfile to the package subfolder "component" it will be listed in the **Components**-tab.
  * `../<packagename>/component/<mylaunch>.launch`
This way you can create a launchfile containing your robot and laser rangefinder drivers once and easily reuse them in different projects.
You can double click or right click and add existing components to the main window. They will be displayed as **includefiles**, but you can fully extract them in the **includefile-editor** (double click the includefile).

---

## Testing and debugging ##
![http://rxdeveloper-ros-pkg.googlecode.com/files/testing.png](http://rxdeveloper-ros-pkg.googlecode.com/files/testing.png)
The toolbar has a "play" and a "stop" button, so that you can **run launchfiles** right from the rxDeveloper without executing a terminal application.
After that you can run a ROS-tool from the "Tools"-menu. You can choose between the following tools:
  * rxgraph
  * rviz
  * dynamic reconfigure gui
  * rxloggerlevel
  * rxconsole
  * roswtf
The output of roswtf is displayed in a information window.

---

## Specificationfiles ##
Specificationfiles are YAML-files for nodes, nodelets or dynamic reconfigure nodes containing information about the type, the package, publications, subscriptions, services and parameters.
These files are very usefull but not mandatory. You can use the rxDeveloper without specificationfiles, but it is more comfortable with them.

### where are they used ###
rxDeveloper uses specification files to support the user in many ways:
  1. New node tags can be created by dragging and dropping
  1. If you want to remap topics, a list of possible topics shows up
    * remap arrows are automatically created when loading a launchfile
  1. If you add a private parameter to a node, a list of possible parameters shows up (with default values)
    * the values for parameters with ranges get checked for out-of-range problems

### how to create files ###
A specificationfile looks like [this](specexample.md). It has to be in a package subfolder "node" in order to be found by the rxdeveloper and end with ".node".
  * `../<packagename>/node/<myNode>.node`

The **type** is the name of a node, **package** is the package the node belongs to. Every specificationfile must contain these two lines.
Subscriptions, publications, services and parameters are optional, but should be entered for a better functionality. Default value and range for parameters are optional, too.

rxDeveloper has a **specfile-editor**. You can edit existing specfiles by right-clicking them in the "Available Nodes/Nodelets"-list and selecting "view/edit specfile". The editor is also launched if you create a new specfile in the **Component Creator**-tab.
![http://rxdeveloper-ros-pkg.googlecode.com/files/specfileedit.node.png](http://rxdeveloper-ros-pkg.googlecode.com/files/specfileedit.node.png)

---

## Package and file creation ##
![http://rxdeveloper-ros-pkg.googlecode.com/files/component-creator.png](http://rxdeveloper-ros-pkg.googlecode.com/files/component-creator.png)

In the **Component Creator**-Tab you can browse existing packages of your working directory. The working directory can be specified in the settings.
Creating a new package will add it to the list of packages and you can also add sourcefiles wherever you want and specificationfiles (specfiles will automatically be created in the subfolder "node").
With the buttons "rosdep install" and "rosmake" you can make your ROS packages.

**Version 1.1** supports specfile creation from running nodes. In the component creation tab you now can add new specfiles with automatically created specification information for running nodes.

**Version 1.2** supports the creation of c++-files from a specfile. Files will be automatically created in the subfolder "src" of the given package and need modification. You can add a line to the _CMakeLists.txt_ for the new c++-file, too.

**Version 1.3b** supports the creation of python-files from a specfile. Files will be automatically created in the subfolder "nodes" of the given package and need modification.
The contextmenu of the packagebrowser allows you to make pythonfiles executable.
![http://rxdeveloper-ros-pkg.googlecode.com/files/create_c++.png](http://rxdeveloper-ros-pkg.googlecode.com/files/create_c++.png)