# ROS_wiki_notes
Created 2018/03/18

---
## ROS 文件系统工具
+ rospack:rospack允许获取软件包的有关信息.
  ```
  $ rospack find roscpp
  /opt/ros/indigo/share/roscpp
  ```
+ roscd:直接切换(cd)工作目录到某个软件包或者软件包集当中
  使用`roscd log`可以切换到ROS保存日记文件的目录下。
+ rosls:直接按软件包的名称而不是绝对路径执行ls命令（罗列目录）

## 创建ROS Package
### catkin packages必须满足这些要求：
  + 必须包含一个catkin compliant(编译) package.xml文件（提供关于package的元信息）;
  + 必须包含一个使用catkin的CMakeLists.txt文件。Catkin metapackages(元package)必须有一个CMakeLists.txt样本文件;
  + 在一个文件夹中不允许有超过两个的package（这就着意味着没有其他packages共享这个目录）.
### 创建一个catkin package
  1. 用catkin_create_pkg 脚本创建package(beginner_tutorials),catkin_create_pkg要求提供一个package的名字和它所需要的依赖:
      ```
      cd ~/catkin_ws/src
      catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
      ```
  2. 现在需要在catkin工作空间编译package:
      ```
      cd ~/catkin_ws
      catkin_make
      ```
### package 依赖
#### 第一层依赖
用catkin_create_pkg时，一些package提供了依赖。这些第一层依赖可以通过rospack工具重现:
  ```
  $ rospack depends1 beginner_tutorials 
  roscpp
  rospy
  std_msgs
  ```
列出了运行catkin_create_pkg命令时的一些依赖，这些依赖储存在package.xml文件中:
  ```xml
  <package>
  ...
  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>roscpp</build_depend>
  <build_depend>rospy</build_depend>
  <build_depend>std_msgs</build_depend>
  ...
  </package>
  ```
#### 间接依赖
许多情况下，依赖也有它的依赖,如:
  ```
  $ rospack depends1 rospy
  genpy
  roscpp
  rosgraph
  rosgraph_msgs
  roslib
  std_msgs
  ```
`rospack depends`可以显示所有递归的嵌套依赖:
  ```
  rospack depends beginner_tutorials 
  ```
### 定制package.xml
+ 描叙标签:`<description>The beginner_tutorials package</description>`
+ 维护者标签:`<maintainer email="shuixin@todo.todo">shuixin</maintainer>`
+ 许可标签:`<license>TODO</license>`
+ 依赖标签:`<build_depend>roscpp</build_depend>`, `<build_export_depend>roscpp</build_export_depend>`, `<exec_depend>roscpp</exec_depend>`...

## 编译ROS程序包
一旦安装了所需的系统依赖项，就可以开始编译刚创建的程序包.Debian通过apt或者其它软件包管理工具来安装ROS的，那么系统已经默认安装好所有依赖项。事先source你的环境配置(setup)文件:
  ```
  source /opt/ros/groovy/setup.bash
  ```
在catkin工作空间下:
  ```
  catkin_make [make_targets] [-DCMAKE_VARIABLES=...]
  ```
catkin_make 是一个命令行工具，它简化了catkin的标准工作流程。可以认为catkin_make是在CMake标准工作流程中依次调用了cmake 和 make.
CMake标准工作流程主要可以分为以下几个步骤：
  ```
  # 在一个CMake项目里
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ make install  # (可选)
  ```
每个CMake工程在编译时都会执行这个操作过程。相反，多个catkin项目可以放在工作空间中一起编译，工作流程如下：
  ```
  # In a catkin workspace
  $ catkin_make
  $ catkin_make install  # (可选)
  ```

## ROS节点
### 图概念
+ Nodes:节点,一个节点即为一个可执行文件，它可以通过ROS与其它节点进行通信。
+ Messages:消息，消息是一种ROS数据类型，用于订阅或发布到一个话题。
+ Topics:话题,节点可以发布消息到话题，也可以订阅话题以接收消息。
+ Master:节点管理器，ROS名称服务 (比如帮助节点找到彼此)。
+ rosout: ROS中相当于stdout/stderr。
+ roscore: 主机+ rosout + 参数服务器 (参数服务器会在后面介绍)。
### 节点
一个节点其实只不过是ROS程序包中的一个可执行文件。ROS节点可以使用ROS客户库与其他节点通信。节点可以发布或接收一个话题。节点也可以提供或使用某种服务。
### 客户端库
ROS客户端库允许使用不同编程语言编写的节点之间互相通信:
+ rospy = python 客户端库
+ roscpp = c++ 客户端库
### roscore
roscore 是你在运行所有ROS程序前首先要运行的命令:
  ```
  roscore
  ```