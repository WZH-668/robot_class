# 机器人底盘驱动调试记录 (2026-05-12)

本项目在运行 `my_base` 驱动包时遇到了环境加载、库链接和硬件识别三个层面的问题。以下是详细的排查与解决过程。

## 1. 基础环境问题：ROS 2 指令无法识别
- **时间**: 15:40
- **现象**: 输入 `ros2 run ...` 提示 `ros2: command not found`。
- **原因**: 当前终端未加载 ROS 2 及工作空间的 `setup.bash` 环境。
- **解决**: 
  - 将 `source /opt/ros/jazzy/setup.bash` 添加至 `~/.bashrc`。
  - 将项目路径下的 `source install/setup.bash` 添加至 `~/.bashrc`。
  - 执行 `source ~/.bashrc` 使其立即生效。

## 2. 编译链接问题：动态库符号查找错误
- **时间**: 15:55
- **现象**: 运行 `my_base` 节点时崩溃，提示 `symbol lookup error: ... undefined symbol: _ZN6serial6Serial11setBaudrateEj`。
- **排查**:
  - 使用 `nm -D /usr/local/lib/libserial.so` 确认库文件中包含该符号。
  - 使用 `ldd` 发现程序运行时错误地加载了系统自带的 `/lib/aarch64-linux-gnu/libserial.so`，而非 `/usr/local/lib` 下的正确版本。
- **解决**:
  - 修改 `CMakeLists.txt`，设置 `CMAKE_INSTALL_RPATH` 为 `/usr/local/lib`。
  - 在 `target_link_libraries` 中显式指定链接 `/usr/local/lib/libserial.so`。

## 3. 硬件识别问题：串口设备路径不匹配
- **时间**: 16:15
- **现象**: 报错 `IO Exception (2): No such file or directory`，无法打开 `/dev/wheeltec_controller`。
- **排查**:
  - `lsusb` 确认硬件已连接（ID 1a86:55d4）。
  - `ls /dev/tty*` 发现设备未被识别为 `/dev/ttyUSB*` 或 `wheeltec_controller`，而是被内核识别为了 `/dev/ttyACM0`。
- **解决**:
  - 修改 `src/my_base/src/my_base.cpp`，将 `setPort` 的路径从 `/dev/wheeltec_controller` 修改为 `/dev/ttyACM0`。
  - 重新执行 `colcon build --packages-select my_base`。

## 4. 逻辑调整：前后与左右移动指令对调
- **时间**: 16:30
- **需求**: 根据实际操控需求，将前进/后退（linear.x）与向左/向右（linear.y）的映射关系对调。
- **解决**: 修改 `src/my_base/src/my_base.cpp` 中的 `Cmd_Vel_Callback` 函数：
  - 硬件 X 轴指令改为接收 `twist_aux->linear.y`。
  - 硬件 Y 轴指令改为接收 `twist_aux->linear.x`。

## 5. 最终状态
- **状态**: 已完成逻辑对调并成功编译。
- **结果**: 机器人现在的运动响应已按照要求进行了轴向对调。