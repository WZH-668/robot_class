# robot_move_pro 运行手册

## 1. 准备环境

```bash
cd ~/Desktop/robot_move/robot_move_pro
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

依赖建议：

```bash
sudo apt update
sudo apt install ros-$ROS_DISTRO-navigation2 ros-$ROS_DISTRO-nav2-bringup
sudo apt install ros-$ROS_DISTRO-slam-toolbox ros-$ROS_DISTRO-robot-localization
sudo apt install ros-$ROS_DISTRO-nav2-map-server
```

## 2. 单模块检查

### 2.1 底盘

```bash
ros2 run my_base my_base
```

检查：

```bash
ros2 topic echo /odom --once
ros2 topic echo /battery_voltage --once
```

### 2.2 雷达

```bash
ros2 launch lslidar_driver lsn10p_launch.py
```

检查：

```bash
ros2 topic echo /scan --once
```

### 2.3 模型

```bash
ros2 launch tbot_description display.launch
```

检查：

```bash
ros2 run tf2_ros tf2_echo base_link laser
```

### 2.4 EKF

```bash
ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
ros2 launch robot_localization_config localization_launch.py
```

检查：

```bash
ros2 topic echo /odom_fused --once
ros2 run tf2_ros tf2_echo odom base_footprint
```

## 3. 建图流程

启动：

```bash
ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
ros2 launch robot_localization_config localization_launch.py
ros2 launch tbot_description display.launch
ros2 launch lslidar_driver lsn10p_launch.py
ros2 launch slam_toolbox online_async_launch.py
```

保存：

```bash
mkdir -p ~/Desktop/robot_move/robot_move_pro/maps
ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/robot_move_pro/maps/my_map
```

## 4. 导航流程

停止 `slam_toolbox` 后启动：

```bash
ros2 launch tbot_navigation bringup_navigation.launch.py \
  map:=$HOME/Desktop/robot_move/robot_move_pro/maps/my_map.yaml
```

RViz 操作：

1. Fixed Frame 设置为 `map`。
2. 使用 `2D Pose Estimate` 设置初始位姿。
3. 确认 `/scan` 与地图墙体重合。
4. 使用 `Nav2 Goal` 发送目标点。

## 5. 常用排错命令

```bash
ros2 topic list
ros2 node list
ros2 topic echo /scan --once
ros2 topic echo /odom --once
ros2 topic echo /odom_fused --once
ros2 topic echo /cmd_vel
ros2 run tf2_ros tf2_echo map odom
ros2 run tf2_ros tf2_echo odom base_footprint
ros2 run tf2_ros tf2_echo base_footprint laser
```

Nav2 lifecycle：

```bash
ros2 lifecycle get /map_server
ros2 lifecycle get /amcl
ros2 lifecycle get /planner_server
ros2 lifecycle get /controller_server
```

## 6. 典型问题

### 6.1 地图能显示但机器人定位不准

处理：

1. 重新用 `2D Pose Estimate` 设置初始位姿。
2. 检查 `/scan` 是否与地图墙体方向一致。
3. 检查 `map -> odom` 是否由 AMCL 发布。

### 6.2 机器人不动

处理：

1. 检查 `/cmd_vel` 是否有输出。
2. 检查 `my_base` 串口是否打开成功。
3. 检查 Nav2 lifecycle 是否 active。
4. 检查 local costmap 是否把周围全判成障碍。

### 6.3 TF 冲突

规则：

```text
建图阶段: slam_toolbox 发布 map -> odom
导航阶段: AMCL 发布 map -> odom
EKF: 发布 odom -> base_footprint
my_base: 发布 /odom，但禁用 odom TF
```

不要同时运行 `slam_toolbox` 和 AMCL。

