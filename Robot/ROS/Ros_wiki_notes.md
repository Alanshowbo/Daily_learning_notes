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
+ 协议标签:`<license>TODO</license>`
+ 依赖标签:`<build_depend>roscpp</build_depend>`...