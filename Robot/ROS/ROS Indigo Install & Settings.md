# ROS Indigo Install & Settings

Created 2018/03/15

---
## Install ROS
Use Exbot source in china:
```
sudo sh -c '. /etc/lsb-release && echo "deb http://ros.exbot.net/rospackage/ros/ubuntu/ $DISTRIB_CODENAME main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
sudo apt-get update
sudo apt-get install ros-indigo-desktop-full
echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt-get install python-rosinstall
sudo rosdep init
rosdep update
```

## Building ROS Packages with Catkin
### Create a catkin Workspace
If you followed the Ubuntu ROS installation instructions, all ROS packages and metapackages will be found under /opt/ros/release, where release is the name of the release of ROS you are using; e.g. /opt/ros/indigo. This is a read-only part of the file system and should not be changed except through the package manager, so you'll want to create a personal ROS directory in your home directory so you can (1) install third-party ROS packages that don't have Debian versions and (2) create your own ROS packages.
```
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace
cd ~/catkin_ws
catkin_make
```
After building any new catkin package(s), be sure to source the ~/catkin_ws/devel/setup.bash file and rebuild the ROS package path as follows:
```
source ~/catkin_ws/devel/setup.bash
rospack profile
```
> This will ensure that ROS can find any new packages, message types and Python modules belonging to the newly built package(s).Add the source command above to the end of your ~/.bashrc file so that new terminals will automatically pick up your catkin packages. 
```
echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc
```
### Doing a "make clean" with catkin
> CAUTION! Do not include the src directory in the rm command below or you will lose all your personal catkin source files!
```
cd ~/catkin_ws
\rm -rf devel build install
```
You would then remake any packages as usual:
```
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```
### Rebuilding a Single catkin Package
> If you update a single package in your catkin workspace and want to re-build just that package, use the following variation of catkin_make:
```
cd ~/catkin_ws
catkin_make --pkg package_name
```

### Networking Between a Robot and a Desktop Computer
An easy way to keep your computers synchronized is to install the Ubuntu chrony package on both your desktop and your robot:
```
sudo apt-get install chrony
```

### Installing Packages with Git
```
cd ~/catkin_ws/src
git clone git://repository/package_name
cd ~/catkin_ws
catkin_make
source devel/setup.bash
rospack profile
```
To update the package later on, run the commands:
```
cd ~/catkin_ws/src/package_name
git pull
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```
### Removing Packages from your Personal catkin Directory
To remove a package installed in your personal catkin directory, first remove the entire package source directory or move it to a different location outside of your ROS_PACKAGE_PATH. For example, to remove a package called my_catkin_package from your ~/catkin_ws/src directory, run the commands:
```
cd ~/catkin_ws/src
\rm -rf my_catkin_package
```
You also have to remove all catkin build objects. Unfortunately, there is no (easy) way to do this for just the package you removed—you have to remove all build objects for all packages and then rerun catkin_make:
```
cd ~/catkin_ws
\rm -rf devel build install
catkin_make
source devel/setup.bash
```
You can test that the package has been removed using the roscd command:
```
roscd my_ros_package
```
### Find Third-Party ROS Packages
+ Searching the ROS Wiki
+ Using the roslocate Command:
```
roslocate uri ros_arduino_bridge
```
which should yield the result:
```
Using ROS_DISTRO: indigo
Not found via rosdistro - falling back to information provided by rosdoc
https://github.com/hbrobotics/ros_arduino_bridge.git
```
This means that we can install the package into our personal catkin directory using the git command:
```
cd ~/catkin_ws/src
git clone git://github.com/hbrobotics/ros_arduino_bridge.git
cd ~/catkin_ws
catkin_make
source devel/setup.bash
rospack profile
```
+ Browsing the ROS Software Index
+ Google