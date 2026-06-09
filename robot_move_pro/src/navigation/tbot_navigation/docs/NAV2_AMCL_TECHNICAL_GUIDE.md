# tbot Nav2 + AMCL 技术文档

本文档说明本项目如何在已经完成 SLAM 建图的基础上，接入 Nav2 自动导航与 AMCL 蒙特卡洛定位。

## 1. 目标

当前项目已经具备底盘控制、雷达、里程计、IMU、EKF 融合定位和 URDF 模型。Nav2 + AMCL 部分的目标是把这些底层能力接成完整导航闭环：

```text
保存好的地图 map.yaml
        +
激光雷达 /scan
        +
EKF 里程计 /odom_fused
        +
TF: map -> odom -> base_footprint -> base_link -> laser
        |
        v
Nav2 自动规划和避障
        |
        v
/cmd_vel
        |
        v
my_base 串口控制 STM32 底盘
```

建图阶段使用 `slam_toolbox`；导航阶段使用 `map_server + AMCL + Nav2`。二者不要同时运行，因为它们都会尝试发布 `map -> odom`。

## 2. 新增内容

本次新增 ROS 2 包：

```text
src/tbot_navigation
```

主要文件：

```text
tbot_navigation/
  CMakeLists.txt
  package.xml
  config/nav2_params.yaml
  launch/nav2_amcl_launch.py
  launch/bringup_navigation.launch.py
  docs/NAV2_AMCL_TECHNICAL_GUIDE.md
```

文件作用：

| 文件 | 作用 |
|---|---|
| `config/nav2_params.yaml` | AMCL、Planner、Controller、Costmap、Behavior、Velocity Smoother 参数 |
| `launch/nav2_amcl_launch.py` | 只启动 Nav2 + AMCL，默认认为底盘、EKF、雷达、模型已启动 |
| `launch/bringup_navigation.launch.py` | 一键启动底盘、EKF、雷达、模型、Nav2 + AMCL |
| `docs/NAV2_AMCL_TECHNICAL_GUIDE.md` | 技术说明和调试流程 |

## 3. 系统架构

### 3.1 底盘层

`my_base` 负责和 STM32 串口通信：

```text
订阅: /cmd_vel
发布: /odom
发布: /imu/data_raw
发布: /battery_voltage
```

开启 Nav2 + EKF 时，必须让 `my_base` 保持运行，但禁用它的 odom TF：

```bash
ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
```

原因是 EKF 会发布 `odom -> base_footprint`。如果 `my_base` 也发布同一条 TF，TF 树会冲突。

### 3.2 定位层

EKF 负责融合轮式里程计和 IMU：

```text
输入: /odom
输入: /imu/data_raw
输出: /odom_fused
发布 TF: odom -> base_footprint
```

AMCL 负责根据已有地图和雷达定位：

```text
输入: map_server 加载的地图
输入: /scan
输入: TF odom -> base_footprint -> base_link -> laser
发布 TF: map -> odom
```

最终 TF 链应为：

```text
map -> odom -> base_footprint -> base_link -> laser
```

### 3.3 导航层

Nav2 负责路径规划、局部避障和速度输出：

```text
输入: 地图、TF、/scan、/odom_fused
输出: /cmd_vel
```

`/cmd_vel` 最终回到 `my_base`，再由 `my_base` 发送串口控制帧给 STM32。

## 4. 环境依赖

目标环境为 ROS 2 Jazzy。建议在机器人端安装：

```bash
sudo apt update
sudo apt install ros-$ROS_DISTRO-navigation2 ros-$ROS_DISTRO-nav2-bringup
sudo apt install ros-$ROS_DISTRO-slam-toolbox ros-$ROS_DISTRO-robot-localization
```

如需保存地图：

```bash
sudo apt install ros-$ROS_DISTRO-nav2-map-server
```

## 5. 编译

进入工作空间后编译：

```bash
cd ~/Desktop/robot_move/my_base_learn
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

如果只想编译新增导航包：

```bash
colcon build --packages-select tbot_navigation
source install/setup.bash
```

## 6. 地图准备

你已经验证过 SLAM 建图可用。推荐继续使用如下流程保存地图：

```bash
mkdir -p ~/Desktop/robot_move/maps
ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/maps/my_map
```

保存后应得到：

```text
~/Desktop/robot_move/maps/my_map.yaml
~/Desktop/robot_move/maps/my_map.pgm
```

导航阶段默认读取：

```text
~/Desktop/robot_move/maps/my_map.yaml
```

如果地图在别的位置，启动时通过 `map:=` 指定。

## 7. 推荐启动方式

### 7.1 一键启动

地图存在默认路径时：

```bash
ros2 launch tbot_navigation bringup_navigation.launch.py
```

指定地图：

```bash
ros2 launch tbot_navigation bringup_navigation.launch.py \
  map:=/home/pi/Desktop/robot_move/maps/my_map.yaml
```

这个启动文件会启动：

```text
my_base，且 pub_odom_tf=false
robot_localization EKF
robot_state_publisher
lslidar_driver
Nav2 + AMCL
```

### 7.2 分终端启动

如果希望更容易排查问题，可以分开启动。

终端 1：底盘

```bash
ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
```

终端 2：EKF

```bash
ros2 launch robot_localization_config localization_launch.py
```

终端 3：机器人模型

```bash
ros2 launch tbot_description display.launch
```

如果不想启动 RViz，也可以只使用 `bringup_navigation.launch.py` 中的 `robot_state_publisher`。

终端 4：雷达

```bash
ros2 launch lslidar_driver lsn10p_launch.py
```

终端 5：Nav2 + AMCL

```bash
ros2 launch tbot_navigation nav2_amcl_launch.py \
  map:=/home/pi/Desktop/robot_move/maps/my_map.yaml
```

## 8. RViz 操作流程

启动 Nav2 后打开 RViz：

```bash
rviz2
```

建议添加这些显示项：

```text
Map
LaserScan: /scan
TF
RobotModel
Global Costmap
Local Costmap
Path
Pose
```

操作步骤：

1. Fixed Frame 设置为 `map`。
2. 使用 `2D Pose Estimate` 在地图上给机器人设置初始位姿。
3. 观察雷达点云是否和地图墙体重合。
4. 使用 `Nav2 Goal` 点一个目标点。
5. 观察是否生成全局路径，机器人是否输出 `/cmd_vel` 并移动。

## 9. 关键参数说明

### 9.1 坐标系参数

`nav2_params.yaml` 中设置：

```yaml
global_frame: "map"
robot_base_frame: "base_footprint"
odom_frame_id: "odom"
odom_topic: "/odom_fused"
scan_topic: "/scan"
```

这些必须和项目实际输出保持一致。

### 9.2 AMCL 运动模型

当前配置使用：

```yaml
robot_model_type: "nav2_amcl::OmniMotionModel"
```

原因是当前 `my_base` 支持 `linear.x`、`linear.y` 和 `angular.z`，更接近全向底盘。

如果实际硬件只能前后运动和原地转向，没有横移能力，应改成：

```yaml
robot_model_type: "nav2_amcl::DifferentialMotionModel"
```

同时把 DWB 和 velocity_smoother 的 Y 方向速度设为 0：

```yaml
max_vel_y: 0.0
min_vel_y: 0.0
max_velocity: [0.18, 0.0, 0.60]
min_velocity: [-0.04, 0.0, -0.60]
```

### 9.3 速度限制

当前配置比较保守：

```yaml
max_vel_x: 0.18
max_vel_y: 0.10
max_vel_theta: 0.60
```

第一次真实运行建议继续低速，确认不会冲撞后再逐步提高。

### 9.4 机器人 footprint

当前近似为：

```yaml
footprint: "[[0.14, 0.12], [0.14, -0.12], [-0.14, -0.12], [-0.14, 0.12]]"
```

这表示机器人占地约：

```text
长 0.28 m
宽 0.24 m
```

如果实车尺寸不同，要根据实际外形调整，否则会出现离墙太近或过不去窄通道的问题。

## 10. 调试检查表

### 10.1 Topic 检查

```bash
ros2 topic list
ros2 topic echo /scan --once
ros2 topic echo /odom --once
ros2 topic echo /odom_fused --once
ros2 topic echo /cmd_vel --once
```

### 10.2 TF 检查

```bash
ros2 run tf2_ros tf2_echo odom base_footprint
ros2 run tf2_ros tf2_echo base_footprint laser
ros2 run tf2_ros tf2_echo map odom
```

导航时应满足：

```text
map -> odom       由 AMCL 发布
odom -> base_footprint 由 EKF 发布
base_footprint -> base_link -> laser 由 robot_state_publisher 发布
```

### 10.3 节点检查

```bash
ros2 node list
```

应能看到类似：

```text
/amcl
/map_server
/planner_server
/controller_server
/bt_navigator
/behavior_server
/velocity_smoother
/ekf_filter_node
/wheeltec_robot
/lslidar_driver_node
/robot_state_publisher
```

### 10.4 Lifecycle 状态

Nav2 节点应该进入 active：

```bash
ros2 lifecycle get /map_server
ros2 lifecycle get /amcl
ros2 lifecycle get /planner_server
ros2 lifecycle get /controller_server
```

## 11. 常见问题

### 11.1 RViz 中机器人不在地图上

可能原因：

- 没有用 `2D Pose Estimate` 设置初始位姿
- AMCL 没有收到 `/scan`
- `map -> odom` 没有发布
- 雷达方向和地图不匹配

处理：

```bash
ros2 topic echo /scan --once
ros2 run tf2_ros tf2_echo map odom
```

然后重新设置初始位姿。

### 11.2 Nav2 不动，但能规划路径

可能原因：

- `/cmd_vel` 没有到达 `my_base`
- `my_base` 串口没有打开
- controller_server 没有 active
- 局部 costmap 认为机器人周围全是障碍

处理：

```bash
ros2 topic echo /cmd_vel
ros2 lifecycle get /controller_server
ros2 topic echo /local_costmap/costmap --once
```

### 11.3 TF 冲突

不要同时让多个节点发布同一条 TF：

```text
map -> odom
odom -> base_footprint
```

规则：

```text
建图阶段: slam_toolbox 发布 map -> odom
导航阶段: AMCL 发布 map -> odom
EKF 阶段: EKF 发布 odom -> base_footprint
my_base: 发布 /odom，但禁用 odom TF
```

### 11.4 路径贴墙太近

调大：

```yaml
inflation_radius
footprint
```

例如把全局和局部 costmap 的 `inflation_radius` 从 `0.25` 提高到 `0.35`。

### 11.5 机器人原地乱转

可能原因：

- 初始位姿不准
- 雷达 `frame_id` 或安装方向不对
- `odom` 方向和实际运动方向相反
- AMCL 运动模型和底盘类型不匹配

处理顺序：

1. 先手动遥控确认前进时 `/odom_fused.twist.twist.linear.x` 方向正确。
2. 确认 RViz 中 `/scan` 和地图墙体能重合。
3. 如果底盘不是全向底盘，把 AMCL 和 DWB 改成差速配置。

## 12. 从 Nav2 到扫地机器人清扫逻辑

Nav2 + AMCL 能实现点到点导航：

```text
当前位置 -> 目标点
```

扫地机器人还需要上层覆盖清扫规划：

```text
读取地图
选择房间或区域
生成弓字形覆盖路径点
逐个调用 Nav2 NavigateToPose
同步控制风机 / 清扫机构
记录已清扫区域
低电量回充和断点续扫
```

因此本包是“自动导航基础层”。后续可以继续新增 `coverage_planner` 或 `cleaning_task_manager`，把清扫路径点持续发送给 Nav2。

## 13. 推荐验收标准

完成 Nav2 + AMCL 后，建议按以下标准验收：

1. RViz 能加载保存地图。
2. `/scan` 能和地图边界基本重合。
3. TF 链完整：`map -> odom -> base_footprint -> base_link -> laser`。
4. AMCL 粒子能收敛在机器人附近。
5. 点击 `Nav2 Goal` 后能生成全局路径。
6. 机器人能低速到达 1 到 2 米外目标点。
7. 中途出现障碍时局部 costmap 能更新，机器人能停止或绕行。
8. 到达目标后 `/cmd_vel` 能归零。

