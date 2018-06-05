# Apollo无人驾驶平台docs阅读notes

## 硬件安装：
+ [2.5版本安装指南](https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_2_5_hardware_system_installation_guide_v1.md)
+ 一些注意点：
    + CAN卡安装到IPC的PCI插口；
    + 必须预留电源插板，为IPC和GPS-IMU提供电源。电源插板还需要服务于车上的其他硬件，比如4G的路由器。电源插板应连接到车辆的电源系统；
    + GPS-IMU天线安装在车辆的顶部，露在外面；
    + GPS-IMU NovAtel SPAN-IGM-A1可放置在车辆的大部分地方，当SPAN-IGM-A1和GPS天线就位时，必须测量从SPAN-IGM-A1到GPS天线的距离。该距离标识为：X偏移，Y偏移和Z偏移。偏移误差必须在1厘米以内才能实现高精度；
    + 激光雷达安装：应避免来自Lidar的激光束被车辆前部或后部阻挡；激光器的垂直倾斜角度通常在相对于地平线的±2〜-24.8度的范围内；
    + Lidar与GPS连线（时间同步）需定制；
    + [激光雷达与组合惯导之间的标定](https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_1_5_lidar_calibration_guide_cn.md)，标定结果可用于将激光雷达检测的障碍物转换至IMU坐标系，进而转到世界坐标系下。
    + [传感器标定](https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_2_0_sensor_calibration_guide_cn.md)，相机到相机的标定，相机到多线激光雷达的标定，以及毫米波雷达到相机的标定
    + 多激光雷达，标定。

## 其它文档
+ [传感器安装](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/Guideline_sensor_Installation_apollo_2.5.md)
+ [激光雷达与惯导标定向导（中文版）](https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_1_5_lidar_calibration_guide_cn.md)
+ [激光雷达与惯导标定原理](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/lidar_calibration_cn.pdf)
+ [相机到相机、相机到激光雷达、毫米波雷达到相机、惯导到车辆标定向导（中文版）](https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_2_0_sensor_calibration_guide_cn.md)
+ [多激光雷达与GNSS标定向导](https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/multiple_lidar_gnss_calibration_guide.md)
+ [Apollo坐标系](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/coordination.pdf)
+ [Apollo软件架构](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/Apollo_2.0_Software_Architecture.md)
+ [3D 障碍物感知](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/3d_obstacle_perception_cn.md)
+ [Apollo 2.5 感知](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/perception_apollo_2.5.md)
+ [QP-Spline-Path 优化控制](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/qp_spline_path_optimizer.md)
+ [QP-Spline-ST-Speed 优化控制](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/qp_spline_st_speed_optimizer.md)
+ [参考线平滑](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/reference_line_smoother.md)
+ [交通信号灯感知](https://github.com/ApolloAuto/apollo/blob/master/docs/specs/traffic_light.md)