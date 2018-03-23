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
  $ rospack depends rospy
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
### rosnode
rosnode 显示当前运行的ROS节点信息。`rosnode list` 指令列出活跃的节点:
  ```
  rosnode list
  ```
+ rosnode ping	test connectivity to node
+ rosnode list	list active nodes
+ rosnode info	print information about node
+ rosnode machine	 list nodes running on a particular machine or list machines
+ rosnode kill	kill a running node
+ rosnode cleanup	 purge registration information of unreachable nodes
### rosrun
rosrun 允许你使用包名直接运行一个包内的节点(而不需要知道这个包的路径):
  ```
  rosrun [package_name] [node_name]
  ```
  使用Remapping Argument改变节点名称:
  ```
  rosrun package_name node_name __name:=new_node_name
  ```

## ROS Topics
### rqt_graph
rqt_graph是rqt程序包中的一部分,rqt_graph能够创建一个显示当前系统运行情况的动态图形:
  ```
  rosrun rqt_graph rqt_graph
  ```
### rostopic
rostopic命令工具能让你获取有关ROS话题的信息:
+ rostopic bw     display bandwidth used by topic
+ rostopic echo   print messages to screen
+ rostopic hz     display publishing rate of topic
+ rostopic list   print information about active topics
+ rostopic pub    publish data to topic
+ rostopic type   print topic type
#### rostopic type
rostopic type 命令用来查看所发布话题的**消息**类型
  ```
  rostopic type [topic]
  ```
e.g
  ```
  $ rostopic type /turtle1/cmd_vel
  geometry_msgs/Twist
  ```
可以使用**rosmsg**命令来查看消息的详细情况:
  ```
  $ rosmsg show geometry_msgs/Twist
  geometry_msgs/Vector3 linear
    float64 x
    float64 y
    float64 z
  geometry_msgs/Vector3 angular
    float64 x
    float64 y
    float64 z
  ```
#### rostopic pub
rostopic pub可以把数据发布到当前某个正在广播的话题上:
  ```
  rostopic pub [topic] [msg_type] [args]
  ```
e.g
  ```
  rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, 1.8]'
  ```
-1（单个破折号）这个参数选项使rostopic发布一条消息后马上退出(once).
  ```
  rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist -r 1 -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, 1.8]'
  ```
这条命令以1Hz的频率发布速度命令到速度话题上. -r命令(rate).
#### rqt_plot
rqt_plot命令可以实时显示一个发布到某个话题上的数据变化图形,在窗口左上角的一个文本框里面你可以添加需要绘制的话题.(rostopic type & rosmsg 查看消息内容)

## ROS service&parameter
### ROS Services
服务（services）是节点之间通讯的另一种方式。服务允许节点发送请求（request） 并获得一个响应（response）.
+ rosservice list         输出可用服务的信息
+ rosservice call         调用带参数的服务
+ rosservice type         输出服务类型
+ rosservice find         依据类型寻找服务find services by service type
+ rosservice uri          输出服务的ROSRPC uri
```
rosservice call [service] [args]
```
e.g
  ```
  rosservice call clear
  ```
查看再生（spawn）服务的信息:
  ```
  $ rosservice type spawn| rossrv show
  float32 x
  float32 y
  float32 theta
  string name
  ---
  string name
  ```
这个服务使我们可以在给定的位置和角度生成一只新的乌龟。名字参数是可选的:
  ```
  rosservice call spawn 2 2 0.2 ""
  ```
服务返回了新产生的乌龟的名字：
  ```
  name: turtle2
  ```
### rossrv
+ rossrv show	  Show service description
+ rossrv list	  List all services
+ rossrv md5	  Display service md5sum
+ rossrv package	  List services in a package
+ rossrv packages	  List packages that contain services
### rosparam
rosparam能够存储并操作ROS 参数服务器（Parameter Server）上的数据。参数服务器能存储整型、浮点、布尔、字符串、字典和列表等数据类型。rosparam使用YAML标记语言的语法。一般而言，YAML的表述很自然：1 是整型, 1.0 是浮点型, one是字符串, true是布尔, [1, 2, 3]是整型列表, {a: b, c: d}是字典. rosparam有很多指令可以用来操作参数，如下所示:
+ rosparam set [param_name]               设置参数
+ rosparam get [param_name]               获取参数,使用`rosparam get /`来显示参数服务器上的所有内容
+ rosparam load [file_name] [namespace]   从文件读取参数
+ rosparam dump [file_name]               向文件中写入参数
+ rosparam delete                         删除参数
+ rosparam list                           列出参数名
`rosparam dump`, `rosparam load`可存储信息以备重新读取.

## rqt_console & roslaunch
绍如何使用rqt_console和rqt_logger_level进行调试，以及如何使用roslaunch同时运行多个节点.
### rqt_console & rqt_logger_level
rqt_console属于ROS日志框架(logging framework)的一部分，用来显示节点的输出信息。
  ```
  rosrun rqt_console rqt_console
  ```
rqt_logger_level允许修改节点运行时输出信息的日志等级（logger levels）（包括 Debug、Warn、Info和Error）
  ```
  rosrun rqt_logger_level rqt_logger_level
  ```
日志等级按以下优先顺序排列：
  ```
  Fatal
  Error
  Warn
  Info
  Debug
  ```
Fatal是最高优先级，Debug是最低优先级。通过设置日志等级你可以获取该等级及其以上优先等级的所有日志消息。比如，将日志等级设为Warn时，你会得到Warn、Error和Fatal这三个等级的所有日志消息。

### roslaunch
roslaunch可以用来启动定义在launch文件中的多个节点:
  ```
  roslaunch [package] [filename.launch]
  ```
**launch文件编写**

## rosed编辑ROS中的文件
rosed是rosbash 的一部分。利用它可以直接通过package名来获取到待编辑的文件而无需指定该文件的存储路径.
  ```
  rosed [package_name] [filename]
  ```
rosed默认的编辑器是vim。如果想要将其他的编辑器设置成默认的，需要修改你的 ~/.bashrc 文件，增加如下语句:
  ```
  export EDITOR='emacs -nw'
  ```
这将emacs设置成为默认编辑器。

## 创建ROS消息和ROS服务
+ 消息(msg): msg文件就是一个描述ROS中所使用消息类型的简单文本。它们会被用来生成不同语言的源代码。
+ 服务(srv): 一个srv文件描述一项服务。它包含两个部分：请求和响应。
msg文件存放在package的msg目录下，srv文件则存放在srv目录下。
msg文件实际上就是每行声明一个数据类型和变量名。可以使用的数据类型如下：
  ```
  int8, int16, int32, int64 (plus uint*)
  float32, float64
  string
  time, duration
  other msg files
  variable-length array[] and fixed-length array[C]
  ```
在ROS中有一个特殊的数据类型：Header，它含有时间戳和坐标系信息。在msg文件的第一行经常可以看到Header header的声明.

srv文件分为请求和响应两部分，由'---'分隔。下面是srv的一个样例：
  ```
  int64 A
  int64 B
  ---
  int64 Sum
  ```
其中 A 和 B 是请求, 而Sum 是响应
### 使用 msg
#### 创建 msg
  ```
  $ cd ~/catkin_ws/src/beginner_tutorials
  $ mkdir msg
  $ echo "int64 num" > msg/Num.msg
  ```
接下来，还有关键的一步：我们要确保msg文件被转换成为C++，Python和其他语言的源代码：
1. 查看package.xml, 确保它包含一下两条语句:
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
在构建的时候，只需要"message_generation"。然而，在运行的时候，需要"message_runtime"。
2. 在 CMakeLists.txt文件中，利用find_packag函数，增加对message_generation的依赖，这样就可以生成消息了。你可以直接在COMPONENTS的列表里增加message_generation(有时候你会发现，即使你没有调用find_package,你也可以编译通过。这是因为catkin把你所有的package都整合在一起，因此，如果其他的package调用了find_package，你的package的依赖就会是同样的配置。但是，在你单独编译时，忘记调用find_package会很容易出错。)：
    ```
    # Do not just add this line to your CMakeLists.txt, modify the existing line
    find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)
    ```


3. 同样，你需要确保你设置了运行依赖：
    ```
    catkin_package(
      ...
      CATKIN_DEPENDS message_runtime ...
      ...)
    ```
4. 找到`add_message_files`代码块,去掉注释符号#，用你的.msg文件替代Message*.msg:
    ```
    add_message_files(
      FILES
      Num.msg
    )
    ```
5. 手动添加.msg文件后，我们要确保CMake知道在什么时候重新配置我们的project。 确保添加了如下代码:
    ```
    generate_messages()
    ```
现在，你可以生成自己的消息源代码了。
#### 使用 rosmsg
  ```
  $ rosmsg show beginner_tutorials/Num
  int64 num
  $ rosmsg show Num
  [beginner_tutorials/Num]:
  int64 num
  ```
### 使用 srv
#### 创建一个srv
  ```
  $ roscd beginner_tutorials
  $ mkdir srv
  ```
使用roscp从其他的package中复制一个服务:
  ```
  $ roscp [package_name] [file_to_copy_path] [copy_path]
  ```
同样要确保srv文件被转换成C++，Python和其他语言的源代码:
1. 同样在CMakeLists.txt文件中增加了对message_generation的依赖,message_generation 对msg和srv都起作用
2. 找到`add_service_files`代码块,去掉注释符号#，用你自己的srv文件名替换掉那些Service*.srv文件:
    ```
    add_service_files(
      FILES
      AddTwoInts.srv
    )
    ```
现在，你可以生成自己的服务源代码了。
#### 使用 rossrv
```
$ rossrv show beginner_tutorials/AddTwoInts 
int64 a
int64 b
---
int64 sum

$ rossrv show AddTwoInts
[beginner_tutorials/AddTwoInts]:
int64 a
int64 b
---
int64 sum

[rospy_tutorials/AddTwoInts]:
int64 a
int64 b
---
int64 sum
```
### msg和srv都需要的步骤
接下来，在CMakeLists.txt中找到如下部分:
  ```
  # generate_messages(
  #   DEPENDENCIES
  # #  std_msgs  # Or other packages containing msgs
  # )
  ```
去掉注释并附加上所有你消息文件所依赖的那些含有.msg文件的package（这个例子是依赖std_msgs,不要添加roscpp,rospy)，结果如下:
  ```
  generate_messages(
    DEPENDENCIES
    std_msgs
  )
  ```
由于增加了新的消息，所以我们需要重新编译我们的package：
  ```
  # In your catkin workspace
  $ cd ../..
  $ catkin_make
  $ cd -
  ```
所有在msg路径下的.msg文件都将转换为ROS所支持语言的源代码。生成的C++头文件将会放置在~/catkin_ws/devel/include/beginner_tutorials/。 Python脚本语言会在 ~/catkin_ws/devel/lib/python2.7/dist-packages/beginner_tutorials/msg 目录下创建。 lisp文件会出现在 ~/catkin_ws/devel/share/common-lisp/ros/beginner_tutorials/msg/ 路径下.
### 回顾
+ rospack = ros+pack(age): provides information related to ROS packages
+ rosstack = ros+stack: provides information related to ROS stacks
+ roscd = ros+cd: changes directory to a ROS package or stack
+ rosls = ros+ls: lists files in a ROS package
+ roscp = ros+cp: copies files from/to a ROS package
+ rosmsg = ros+msg: provides information related to ROS message definitions
+ rossrv = ros+srv: provides information related to ROS service definitions
+ rosmake = ros+make: makes (compiles) a ROS package