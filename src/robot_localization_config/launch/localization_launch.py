#!/usr/bin/env python3
"""
Launch file for robot_localization EKF fusion
启动 robot_localization EKF 融合节点

This launch file:
- Starts the EKF node from robot_localization package
- Loads configuration from ekf.yaml
- Remaps output topic to /odom_fused
- Disables TF publishing from my_base (requires manual launch with pub_odom_tf:=false)

本启动文件：
- 启动 robot_localization 包的 EKF 节点
- 从 ekf.yaml 加载配置
- 将输出话题重映射为 /odom_fused
- 禁用 my_base 的 TF 发布（需要手动启动时设置 pub_odom_tf:=false）
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Get package directory
    pkg_dir = get_package_share_directory('robot_localization_config')

    # Path to EKF config file
    ekf_config_file = os.path.join(pkg_dir, 'config', 'ekf.yaml')

    # Declare launch arguments
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulation time if true'
    )

    return LaunchDescription([
        use_sim_time_arg,

        # EKF node from robot_localization
        Node(
            package='robot_localization',
            executable='ekf_node',
            name='ekf_filter_node',
            output='screen',
            parameters=[
                ekf_config_file,
                {'use_sim_time': LaunchConfiguration('use_sim_time')}
            ],
            # Remap output topics
            remappings=[
                ('/odometry/filtered', '/odom_fused'),
                ('/tf', '/tf'),
            ],
        ),
    ])