#!/usr/bin/env python3
"""
Launch file for MPU6050 IMU node
"""

from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
import os
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Get package directory
    pkg_dir = get_package_share_directory('imu_mpu6050')

    # Declare launch arguments
    i2c_bus_arg = DeclareLaunchArgument(
        'i2c_bus',
        default_value='1',
        description='I2C bus number (usually 1 on Raspberry Pi)'
    )

    i2c_address_arg = DeclareLaunchArgument(
        'i2c_address',
        default_value='104',
        description='I2C address of MPU6050 in decimal (104 for 0x68, 105 for 0x69)'
    )

    frame_id_arg = DeclareLaunchArgument(
        'frame_id',
        default_value='imu_Link',
        description='Frame ID for IMU messages'
    )

    publish_rate_arg = DeclareLaunchArgument(
        'publish_rate',
        default_value='50.0',
        description='Publishing rate in Hz'
    )

    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value=os.path.join(pkg_dir, 'config', 'mpu6050_params.yaml'),
        description='Path to config file'
    )

    # Create node
    mpu6050_node = Node(
        package='imu_mpu6050',
        executable='mpu6050_node',
        name='mpu6050_node',
        output='screen',
        parameters=[
            LaunchConfiguration('config_file'),
            {
                'i2c_bus': ParameterValue(LaunchConfiguration('i2c_bus'), value_type=int),
                'i2c_address': ParameterValue(LaunchConfiguration('i2c_address'), value_type=int),
                'frame_id': LaunchConfiguration('frame_id'),
                'publish_rate': ParameterValue(LaunchConfiguration('publish_rate'), value_type=float),
            }
        ],
    )

    return LaunchDescription([
        i2c_bus_arg,
        i2c_address_arg,
        frame_id_arg,
        publish_rate_arg,
        config_file_arg,
        mpu6050_node,
    ])