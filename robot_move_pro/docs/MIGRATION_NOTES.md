# 从 robot_class 到 robot_move_pro 的迁移说明

## 1. 迁移方式

本次没有修改旧项目 `robot_class` 中的任何包，而是在 `robot_move_pro` 中创建了新的工作区结构，并复制现有包作为新工作区的基础。

旧项目：

```text
robot_class/src/
```

新项目：

```text
robot_move_pro/src/
```

## 2. 包迁移对应关系

| 原路径 | 新路径 |
|---|---|
| `robot_class/src/my_base` | `robot_move_pro/src/base/my_base` |
| `robot_class/src/imu_mpu6050` | `robot_move_pro/src/sensors/imu_mpu6050` |
| `robot_class/src/lslidar_driver` | `robot_move_pro/src/sensors/lslidar_driver` |
| `robot_class/src/tbot_description` | `robot_move_pro/src/description/tbot_description` |
| `robot_class/src/robot_localization_config` | `robot_move_pro/src/localization/robot_localization_config` |
| `robot_class/src/tbot_navigation` | `robot_move_pro/src/navigation/tbot_navigation` |
| `robot_class/src/robot_control` | `robot_move_pro/src/apps/robot_control` |
| `robot_class/src/lidar_test` | `robot_move_pro/src/apps/lidar_test` |

## 3. 为什么这样整理

原项目所有包直接堆在 `src` 下，能运行，但随着 Nav2、清扫任务、回充、覆盖规划等模块增加，会越来越难维护。

新结构按职责分层：

```text
base        底盘
sensors     传感器
description 模型
localization 定位
navigation  导航
apps        实验应用
```

这样后续新增功能时，可以清楚判断应该放在哪一层。

## 4. 保持不变的部分

以下内容保持原功能和包名不变：

- `my_base`
- `imu_mpu6050`
- `lslidar_driver`
- `tbot_description`
- `robot_localization_config`
- `tbot_navigation`
- `robot_control`
- `lidar_test`

ROS2 包名没有变化，因此运行命令仍然是：

```bash
ros2 run my_base my_base
ros2 launch lslidar_driver lsn10p_launch.py
ros2 launch robot_localization_config localization_launch.py
ros2 launch tbot_navigation bringup_navigation.launch.py
```

## 5. 需要注意的变化

地图默认建议放到新工作区：

```text
robot_move_pro/maps/
```

所以保存地图命令建议使用：

```bash
ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/robot_move_pro/maps/my_map
```

启动 Nav2 时指定：

```bash
ros2 launch tbot_navigation bringup_navigation.launch.py \
  map:=$HOME/Desktop/robot_move/robot_move_pro/maps/my_map.yaml
```

## 6. 后续建议

后续如果继续开发，建议只在 `robot_move_pro` 上做新功能，旧的 `robot_class` 作为历史可运行版本保留。

