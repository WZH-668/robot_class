# robot_move_pro 项目架构

## 1. 设计原则

`robot_move_pro` 的整理原则是：

1. 原有可运行包不改动。
2. 按功能层级重新组织目录。
3. 工作区级文档、地图、脚本和 ROS2 包分离。
4. 数据接口保持 ROS2 标准话题和 TF 约定。
5. 为后续扫地机器人任务层预留扩展位置。

## 2. 分层架构

```text
应用层 apps
  robot_control
  lidar_test

导航层 navigation
  tbot_navigation

定位层 localization
  robot_localization_config

模型层 description
  tbot_description

传感器层 sensors
  imu_mpu6050
  lslidar_driver

底盘层 base
  my_base
```

## 3. 数据流

### 3.1 底盘控制链路

```text
Nav2 / teleop / robot_control / lidar_test
        |
        v
      /cmd_vel
        |
        v
      my_base
        |
        v
STM32 下位机
```

`my_base` 是底盘控制入口，负责将 ROS2 的 `geometry_msgs/Twist` 转换为 STM32 串口控制帧。

### 3.2 里程计和 IMU 链路

```text
STM32 / 编码器 / IMU
        |
        v
      my_base
        |
        +--> /odom
        +--> /imu/data_raw
        +--> /battery_voltage
```

也可以使用独立 `imu_mpu6050` 包直接从 I2C 读取 MPU6050，但同一时间建议只保留一路 `/imu/data_raw` 发布者。

### 3.3 EKF 定位链路

```text
/odom
/imu/data_raw
        |
        v
robot_localization EKF
        |
        +--> /odom_fused
        +--> TF: odom -> base_footprint
```

开启 EKF 后，`my_base` 仍然发布 `/odom`，但不再发布 `odom -> base_footprint` TF。

### 3.4 建图链路

```text
/scan
/odom_fused
TF: odom -> base_footprint -> base_link -> laser
        |
        v
slam_toolbox
        |
        +--> /map
        +--> TF: map -> odom
```

建图完成后保存为：

```text
maps/my_map.yaml
maps/my_map.pgm
```

### 3.5 导航链路

```text
map_server 加载地图
/scan
/odom_fused
TF
        |
        v
AMCL + Nav2
        |
        +--> TF: map -> odom
        +--> /cmd_vel
```

导航阶段由 AMCL 发布 `map -> odom`，不能同时运行 `slam_toolbox`。

## 4. TF 规范

完整 TF 树：

```text
map
└── odom
    └── base_footprint
        └── base_link
            ├── laser
            ├── imu_Link
            ├── camera_Link
            └── ultrasonic_Link
```

发布者约定：

| TF | 发布者 |
|---|---|
| `map -> odom` | 建图时由 `slam_toolbox`，导航时由 AMCL |
| `odom -> base_footprint` | EKF |
| `base_footprint -> base_link -> laser` | `robot_state_publisher` |

## 5. 话题规范

| 话题 | 类型 | 主要发布者 | 用途 |
|---|---|---|---|
| `/cmd_vel` | `geometry_msgs/Twist` | Nav2 / teleop / 控制脚本 | 底盘速度指令 |
| `/odom` | `nav_msgs/Odometry` | `my_base` | 原始里程计 |
| `/odom_fused` | `nav_msgs/Odometry` | EKF | 融合后里程计 |
| `/imu/data_raw` | `sensor_msgs/Imu` | `my_base` 或 `imu_mpu6050` | IMU 数据 |
| `/scan` | `sensor_msgs/LaserScan` | `lslidar_driver` | 雷达扫描 |
| `/battery_voltage` | `std_msgs/Float32` | `my_base` | 电池电压 |
| `/fan_cmd` | `std_msgs/Bool` | `fan_keyboard` / 任务层 | 风机开关 |

## 6. 后续扩展建议

后续做扫地机器人完整逻辑时，建议新增包：

```text
src/cleaning/
  coverage_planner/
  cleaning_task_manager/
  docking_manager/
```

推荐职责：

| 包 | 职责 |
|---|---|
| `coverage_planner` | 根据地图生成弓字形覆盖路径 |
| `cleaning_task_manager` | 管理全屋、房间、区域清扫任务 |
| `docking_manager` | 回充、断点续扫、低电量处理 |

这些新增包应调用 Nav2 的 `NavigateToPose` 或 `NavigateThroughPoses`，不要绕过 Nav2 直接控制底盘，除非是低速贴边或特殊动作。

