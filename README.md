# Robot Move Learn 项目指南

本项目基于ROS 2 Jazzy系统搭建机器人底盘控制与环境感知体系，实现轮趣小车底盘驱动、LSLidar激光雷达适配集成与自动化运动控制。  项目隶属于深圳职业技术学院未来技术学院核心实训课程《移动机器人基础》，课程以轮趣三轮智能小车为硬件载体，围绕ROS 2操作系统开展系统化理论学习与实操训练。本次实训以开发实用型扫地机器人为最终目标，完成小车全流程开发调试工作。  学习过程中可实现未知环境地图构建、自主路径行进、智能障碍物避让、自动吸尘清扫等核心功能，全面掌握移动机器人感知探测、路径规划、运动控制与指令执行全套技术，扎实锤炼智能机器人开发实操本领。

## 1. 核心功能包

- **`my_base`**: 底盘驱动包。负责串口通信（`/dev/wheeltec_controller`）、`/cmd_vel` 速度指令解析及 `odom` 里程计发布。
- **`lslidar_driver`**: 激光雷达驱动。支持力神 M10/N10 系列，发布 `/scan` 话题。
- **`robot_control`**: 运动控制应用。包含圆形运动和"弓"字形运动逻辑。
- **`lidar_test`**: 感知测试工具。包含雷达精度检测和自动追踪功能。
- **`tbot_description`**: 机器人模型。包含 URDF 定义及 RViz 可视化配置。
- **`imu_mpu6050`**: IMU 传感器驱动。读取 MPU6050 加速度计和陀螺仪数据，发布 `sensor_msgs/Imu` 消息。
- **`robot_localization_config`**: EKF 融合定位配置。融合轮式里程计和 IMU 数据，输出更精确的 `odom_fused` 定位信息。
- **`tbot_navigation`**: Nav2 自动导航配置包。加载已保存地图，通过 AMCL 定位，使用 Nav2 规划路径并输出 `/cmd_vel` 控制底盘。

## 2. 快速运行指南

在运行任何节点前，请确保已编译并加载环境：

```bash
# 进入包含 src 目录的 ROS 2 工作空间根目录
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
3. **雷达自动追踪（对准最近物体）**：
   ```bash
   ros2 run lidar_test trace_thing
   ```

### E. IMU 数据采集与 EKF 融合定位

1. **启动 MPU6050 IMU 节点**（I2C 接口）：
   ```bash
   ros2 launch imu_mpu6050 mpu6050_launch.py
   ```
   - 默认 I2C 地址：`0x68`
   - 发布话题：`/imu/data_raw`

2. **启动 EKF 融合定位**（需先关闭 my_base 的 TF 发布）：
   ```bash
   # 方式一：先启动底盘（不发布 odom TF）
   ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
   
   # 方式二：启动 EKF 融合节点
   ros2 launch robot_localization_config localization_launch.py
   ```
   - EKF 融合 `odom`（里程计）和 `imu/data_raw`（IMU）数据
   - 输出更精确的融合定位：`/odom_fused`
   - 发布 TF：`odom → base_footprint`

3. **IMU 依赖安装**（如未安装）：
   ```bash
   pip3 install smbus2
   # 或
   pip3 install smbus
   ```

### C. 模型可视化

1. **启动 RViz 查看模型**：
   ```bash
   ros2 launch tbot_description display.launch
   ```

### D. 雷达建图（2D SLAM）

1. **安装建图工具**：
   ```bash
   sudo apt update
   sudo apt install ros-$ROS_DISTRO-slam-toolbox ros-$ROS_DISTRO-nav2-map-server
   ```
2. **基础建图（仅雷达 + 底盘）**：
   ```bash
   ros2 run my_base my_base
   ```
   ```bash
   ros2 launch tbot_description display.launch
   ```
   ```bash
   ros2 launch lslidar_driver lsn10p_launch.py
   ```
   ```bash
   ros2 launch slam_toolbox online_async_launch.py
   ```

3. **带 IMU 的 EKF 融合建图（推荐，定位更稳定）**：
   ```bash
   ros2 launch imu_mpu6050 mpu6050_launch.py
   ```
   ```bash
   ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
   ```
   ```bash
   ros2 launch robot_localization_config localization_launch.py
   ```
   ```bash
   ros2 launch tbot_description display.launch
   ```
   ```bash
   ros2 launch lslidar_driver lsn10p_launch.py
   ```
   ```bash
   ros2 launch slam_toolbox online_async_launch.py
   ```

4. **确认输入正常**：
   - 底盘发布 `odom -> base_footprint`（或 EKF 发布 `odom -> base_footprint`）
   - 机器人模型提供 `base_link -> laser`
   - 雷达驱动发布 `/scan`，且 `frame_id` 为 `laser`
   - IMU 发布 `/imu/data_raw`（如使用 EKF 融合）
5. **常用检查命令**：
   ```bash
   ros2 topic echo /scan --once
   ```
   ```bash
   ros2 topic echo /odom --once
   ```
   ```bash
   ros2 topic echo /imu/data_raw --once
   ```
   ```bash
   ros2 run tf2_ros tf2_echo base_footprint laser
   ```
6. **建图操作建议**：
   - 缓慢推动或遥控机器人绕场地一圈，避免急转和打滑
   - 先验证前进时 `odom.linear.x` 为正，再正式建图
   - 若地图重影或漂移，优先检查里程计方向和雷达安装朝向
   - 使用 EKF 融合后，旋转和直线运动的定位精度会明显提升
7. **保存地图**：
   ```bash
   mkdir -p ~/Desktop/robot_move/maps
   ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/maps/my_map
   ```

### E. Nav2 + AMCL 自动导航

本项目已新增 `tbot_navigation` 包，用于在已有地图上运行 Nav2 自动导航。推荐流程是：

```text
先用 slam_toolbox 建图并保存地图
停止 slam_toolbox
再启动 map_server + AMCL + Nav2
```

导航阶段不要同时启动 `slam_toolbox`，因为 `slam_toolbox` 和 AMCL 都会发布 `map -> odom`，同时运行会造成 TF 冲突。

1. **安装 Nav2 依赖**：
   ```bash
   sudo apt update
   sudo apt install ros-$ROS_DISTRO-navigation2 ros-$ROS_DISTRO-nav2-bringup
   sudo apt install ros-$ROS_DISTRO-robot-localization ros-$ROS_DISTRO-nav2-map-server
   ```

2. **编译导航包**：
   ```bash
   colcon build --packages-select tbot_navigation
   source install/setup.bash
   ```

3. **一键启动实车导航**：
   ```bash
   ros2 launch tbot_navigation bringup_navigation.launch.py \
     map:=$HOME/Desktop/robot_move/maps/my_map.yaml
   ```

   该启动文件会启动：
   - `my_base`，并自动设置 `pub_odom_tf:=false`
   - `robot_localization` EKF，发布 `odom -> base_footprint`
   - `robot_state_publisher`，发布机器人模型 TF
   - `lslidar_driver`，发布 `/scan`
   - `map_server + AMCL + Nav2`

4. **如果底层节点已手动启动，只启动 Nav2 + AMCL**：
   ```bash
   ros2 launch tbot_navigation nav2_amcl_launch.py \
     map:=$HOME/Desktop/robot_move/maps/my_map.yaml
   ```

5. **RViz 操作**：
   ```bash
   rviz2
   ```
   - Fixed Frame 设置为 `map`
   - 使用 `2D Pose Estimate` 设置机器人初始位置
   - 确认 `/scan` 和地图墙体基本重合
   - 使用 `Nav2 Goal` 发送目标点

6. **导航阶段 TF 关系**：
   ```text
   map -> odom                         AMCL 发布
   odom -> base_footprint              EKF 发布
   base_footprint -> base_link -> laser  URDF 发布
   ```

7. **常用检查命令**：
   ```bash
   ros2 topic echo /scan --once
   ros2 topic echo /odom_fused --once
   ros2 topic echo /cmd_vel
   ros2 run tf2_ros tf2_echo map odom
   ros2 run tf2_ros tf2_echo odom base_footprint
   ros2 run tf2_ros tf2_echo base_footprint laser
   ```

8. **详细技术文档**：

   Nav2 + AMCL 的参数解释、启动流程、调试方法和常见问题见：

   ```text
   src/tbot_navigation/docs/NAV2_AMCL_TECHNICAL_GUIDE.md
   ```

## 3. 硬件配置

- **底盘串口设备**: `/dev/wheeltec_controller`
- **底盘波特率**: `115200`
- **雷达串口设备**: `/dev/wheeltec_lidar`
- **雷达坐标系**: `laser`
- **雷达安装高度**: `0 0 0.122`（相对 `base_link`）
- **雷达型号**: LSLidar N10/M10 (UART 接口)
- **IMU 设备**: MPU6050 (I2C 接口，地址 `0x68`)
- **IMU 安装位置**: 底盘中心，与 `base_link` 坐标系对齐

## 4. 树莓派通过 USB 串口控制风机

当前连接方式为：`树莓派 -> USB -> STM32 下位机`。

已确认串口设备为：`/dev/ttyACM0`

### A. 协议说明

- 总长度：`11` 字节
- 帧头：`0x7B`
- 帧尾：`0x7D`
- 波特率：`115200`
- `byte2` 的 `bit0=1` 表示开风机，`bit0=0` 表示关风机

### B. 开关风机命令

1. **开风机**：
   ```bash
   python3 -c "import serial; ser=serial.Serial('/dev/ttyACM0',115200,timeout=1); ser.write(bytes.fromhex('7B 00 01 00 00 00 00 00 00 7A 7D')); ser.close()"
   ```

2. **关风机**：
   ```bash
   python3 -c "import serial; ser=serial.Serial('/dev/ttyACM0',115200,timeout=1); ser.write(bytes.fromhex('7B 00 00 00 00 00 00 00 00 7B 7D')); ser.close()"
   ```

### C. 端口检查

先查看当前 USB 串口设备：

```bash
ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
```

如果输出为 ` /dev/ttyACM0`，则发送命令时应使用 `/dev/ttyACM0`，不能写成 `/dev/ttyUSB0`。

### D. 依赖安装

如果树莓派未安装串口 Python 库，先执行：

```bash
pip3 install pyserial
```

### E. 风机键盘控制节点

除了直接发送串口命令，也可以使用 ROS2 节点控制风机：

```bash
ros2 run my_base fan_keyboard
```

按键说明：
- `f`: 开风机
- `g`: 关风机  
- `q`: 退出

该节点发布 `/fan_cmd` 话题（`Bool` 类型），可通过话题订阅方式集成到其他控制逻辑中。

### F. 注意事项

- 树莓派终端不能直接执行 `Fan_Set(1)` 或 `Fan_Set(0)`，因为这是 STM32 固件内部函数
- 树莓派需要做的是通过串口发送协议帧，让 STM32 在接收逻辑中调用 `Fan_Set()`
- 若发送后风机无反应，需要继续确认这一路 USB 串口是否最终进入 STM32 的 `USART3` 协议处理逻辑
- 树莓派与 STM32 通信时应保证供电稳定，通信链路正常
- 使用 EKF 融合时，务必先禁用 `my_base` 的 TF 发布（`pub_odom_tf:=false`），避免 TF 冲突
- 使用 Nav2 + AMCL 导航时，务必停止 `slam_toolbox`，避免 `map -> odom` TF 冲突
- MPU6050 的 I2C 地址若不为 `0x68`，需在 `imu_mpu6050/config/mpu6050_params.yaml` 中修改

