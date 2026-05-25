# Robot Move Learn 项目指南

本项目是一个基于 **ROS 2 (Jazzy)** 的机器人底盘控制与感知系统，主要用于 Wheeltec 机器人底盘的驱动、LSLidar 激光雷达的集成以及自动化运动控制。

## 1. 核心功能包
- **`my_base`**: 底盘驱动包。负责串口通信（`/dev/wheeltec_controller`）、`/cmd_vel` 速度指令解析及 `odom` 里程计发布。
- **`lslidar_driver`**: 激光雷达驱动。支持力神 M10/N10 系列，发布 `/scan` 话题。
- **`robot_control`**: 运动控制应用。包含圆形运动和“弓”字形运动逻辑。
- **`lidar_test`**: 感知测试工具。实时监控雷达数据准确性。
- **`tbot_description`**: 机器人模型。包含 URDF 定义及 RViz 可视化配置。

## 2. 快速运行指南

在运行任何节点前，请确保已编译并加载环境：
```bash
cd ~/Desktop/robot_move/my_base_learn
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

### A. 底盘控制与运动
1. **启动底盘驱动**（必须先启动）：
   ```bash
   ros2 run my_base my_base
   ```
2. **键盘手动控制**：
   ```bash
   ros2 run teleop_twist_keyboard teleop_twist_keyboard
   ```
3. **执行自动化运动**（二选一）：
   - **走圆圈**：`ros2 run robot_control circle_controller`
   - **走“弓”字**：`ros2 run robot_control bow_controller`

### B. 雷达感知与测试
1. **启动雷达驱动**：
   ```bash
   ros2 launch lslidar_driver lsn10p_launch.py
   ```
2. **运行数据检测**：
   ```bash
   ros2 run lidar_test test_accurity
   ```

### C. 模型可视化
1. **启动 RViz 查看模型**：
   ```bash
   ros2 launch tbot_description display.launch.py
   ```

### D. 雷达建图（2D SLAM）
1. **安装建图工具**：
   ```bash
   sudo apt update
   sudo apt install ros-$ROS_DISTRO-slam-toolbox ros-$ROS_DISTRO-nav2-map-server
   ```
2. **依次启动 4 个节点**：
   ```bash
   ros2 run my_base my_base
   ```
   ```bash
   ros2 launch tbot_description display.launch.py
   ```
   ```bash
   ros2 launch lslidar_driver lsn10p_launch.py
   ```
   ```bash
   ros2 launch slam_toolbox online_async_launch.py
   ```
3. **确认输入正常**：
   - 底盘发布 `odom -> base_footprint`
   - 机器人模型提供 `base_link -> laser`
   - 雷达驱动发布 `/scan`，且 `frame_id` 为 `laser`
4. **常用检查命令**：
   ```bash
   ros2 topic echo /scan --once
   ```
   ```bash
   ros2 topic echo /odom --once
   ```
   ```bash
   ros2 run tf2_ros tf2_echo base_footprint laser
   ```
5. **建图操作建议**：
   - 缓慢推动或遥控机器人绕场地一圈，避免急转和打滑
   - 先验证前进时 `odom.linear.x` 为正，再正式建图
   - 若地图重影或漂移，优先检查里程计方向和雷达安装朝向
6. **保存地图**：
   ```bash
   mkdir -p ~/Desktop/robot_move/maps
   ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/maps/my_map
   ```

## 3. 硬件配置
- **底盘串口设备**: `/dev/wheeltec_controller`
- **底盘波特率**: `115200`
- **雷达串口设备**: `/dev/wheeltec_lidar`
- **雷达坐标系**: `laser`
- **雷达安装高度**: `0 0 0.122`（相对 `base_link`）
- **雷达型号**: LSLidar N10/M10 (UART 接口)