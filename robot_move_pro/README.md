# robot_move_pro

`robot_move_pro` 是在原 `robot_class` 基础上整理出的新版 ROS 2 移动机器人工作空间。它的目标是保持原有可用功能不变，同时把工程结构整理成更清晰、后续更容易扩展的专业版架构。

本目录是一个新的独立工作区，原来的 `robot_class` 和其中所有包都没有被修改。

## 1. 工作区结构

```text
robot_move_pro/
  README.md
  docs/
  maps/
  scripts/
  src/
    base/
      my_base/
    sensors/
      imu_mpu6050/
      lslidar_driver/
    description/
      tbot_description/
    localization/
      robot_localization_config/
    navigation/
      tbot_navigation/
    apps/
      lidar_test/
      robot_control/
```

分类说明：

| 分类 | 内容 | 作用 |
|---|---|---|
| `base` | `my_base` | 底盘串口驱动、`/cmd_vel`、`/odom`、风机控制 |
| `sensors` | `imu_mpu6050`、`lslidar_driver` | IMU 与激光雷达数据采集 |
| `description` | `tbot_description` | URDF、STL 模型、机器人 TF 结构 |
| `localization` | `robot_localization_config` | EKF 融合定位 |
| `navigation` | `tbot_navigation` | Nav2 + AMCL 自动导航 |
| `apps` | `robot_control`、`lidar_test` | 实验控制、雷达测试、目标对准 |
| `maps` | 地图文件目录 | 保存 `my_map.yaml`、`my_map.pgm` |
| `docs` | 技术文档 | 架构、运行、迁移说明 |
| `scripts` | 工作区级辅助脚本预留 | 后续可放启动/检查脚本 |

## 2. 当前能力

新版工作区保留原项目已经完成的能力：

- 底盘串口控制
- `/cmd_vel` 运动控制
- `/odom` 轮式里程计
- `/imu/data_raw` IMU 数据
- `/battery_voltage` 电池电压
- `/fan_cmd` 风机控制
- LSLidar 雷达 `/scan`
- URDF 模型与 TF
- EKF 融合定位 `/odom_fused`
- `slam_toolbox` 建图接入基础
- Nav2 + AMCL 自动导航配置
- 圆形运动、弓字形运动测试
- 雷达距离检测和最近目标对准

尚未完整实现但已经为后续预留的方向：

- 房间/区域语义划分
- 自动覆盖式清扫路径规划
- 沿边清扫
- 漏扫补扫
- 回充与断点续扫
- 清扫任务管理器

## 3. 编译

在机器人端进入本工作区根目录：

```bash
cd ~/Desktop/robot_move/robot_move_pro
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

如果只编译导航包：

```bash
colcon build --packages-select tbot_navigation
source install/setup.bash
```

## 4. 推荐运行流程

### 4.1 手动控制底盘

```bash
ros2 run my_base my_base
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

### 4.2 雷达启动

```bash
ros2 launch lslidar_driver lsn10p_launch.py
```

### 4.3 EKF 融合定位

开启 EKF 时，`my_base` 仍然运行，但禁用它发布 `odom -> base_footprint`：

```bash
ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
ros2 launch robot_localization_config localization_launch.py
```

### 4.4 SLAM 建图

```bash
ros2 run my_base my_base --ros-args -p pub_odom_tf:=false
ros2 launch robot_localization_config localization_launch.py
ros2 launch tbot_description display.launch
ros2 launch lslidar_driver lsn10p_launch.py
ros2 launch slam_toolbox online_async_launch.py
```

保存地图：

```bash
mkdir -p ~/Desktop/robot_move/robot_move_pro/maps
ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/robot_move_pro/maps/my_map
```

### 4.5 Nav2 + AMCL 自动导航

导航时停止 `slam_toolbox`，然后启动：

```bash
ros2 launch tbot_navigation bringup_navigation.launch.py \
  map:=$HOME/Desktop/robot_move/robot_move_pro/maps/my_map.yaml
```

如果底层节点已经手动启动，只启动 Nav2 + AMCL：

```bash
ros2 launch tbot_navigation nav2_amcl_launch.py \
  map:=$HOME/Desktop/robot_move/robot_move_pro/maps/my_map.yaml
```

导航阶段 TF 关系：

```text
map -> odom                         AMCL 发布
odom -> base_footprint              EKF 发布
base_footprint -> base_link -> laser  URDF 发布
```

## 5. 关键约束

- 不要同时运行 `slam_toolbox` 和 AMCL，它们都会发布 `map -> odom`。
- 开 EKF 时，必须让 `my_base` 设置 `pub_odom_tf:=false`，避免 `odom -> base_footprint` 重复发布。
- `my_base` 和 `imu_mpu6050` 都能发布 `/imu/data_raw`，实车运行时建议只保留一路 IMU 来源。
- `maps/` 目录用于放实测地图，第一次导航前需要先保存地图。

## 6. 文档

详细说明见：

```text
docs/PROJECT_ARCHITECTURE.md
docs/RUNBOOK.md
docs/MIGRATION_NOTES.md
src/navigation/tbot_navigation/docs/NAV2_AMCL_TECHNICAL_GUIDE.md
```

