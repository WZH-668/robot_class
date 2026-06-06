# Robot Move Learn 项目指南

本项目基于ROS 2 Jazzy系统搭建机器人底盘控制与环境感知体系，实现轮趣小车底盘驱动、LSLidar激光雷达适配集成与自动化运动控制。  项目隶属于深圳职业技术学院未来技术学院核心实训课程《移动机器人基础》，课程以轮趣三轮智能小车为硬件载体，围绕ROS 2操作系统开展系统化理论学习与实操训练。本次实训以开发实用型扫地机器人为最终目标，完成小车全流程开发调试工作。  学习过程中可实现未知环境地图构建、自主路径行进、智能障碍物避让、自动吸尘清扫等核心功能，全面掌握移动机器人感知探测、路径规划、运动控制与指令执行全套技术，扎实锤炼智能机器人开发实操本领。

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

### E. 注意事项

- 树莓派终端不能直接执行 `Fan_Set(1)` 或 `Fan_Set(0)`，因为这是 STM32 固件内部函数
- 树莓派需要做的是通过串口发送协议帧，让 STM32 在接收逻辑中调用 `Fan_Set()`
- 若发送后风机无反应，需要继续确认这一路 USB 串口是否最终进入 STM32 的 `USART3` 协议处理逻辑
- 树莓派与 STM32 通信时应保证供电稳定，通信链路正常

