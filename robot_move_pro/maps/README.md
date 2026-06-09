# maps

本目录用于保存 SLAM 建图结果。

推荐保存命令：

```bash
ros2 run nav2_map_server map_saver_cli -f ~/Desktop/robot_move/robot_move_pro/maps/my_map
```

保存后应包含：

```text
my_map.yaml
my_map.pgm
```

导航启动时使用：

```bash
ros2 launch tbot_navigation bringup_navigation.launch.py \
  map:=$HOME/Desktop/robot_move/robot_move_pro/maps/my_map.yaml
```

