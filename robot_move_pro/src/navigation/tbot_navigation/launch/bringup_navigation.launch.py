#!/usr/bin/env python3
"""Bring up the physical robot stack plus Nav2 + AMCL."""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import EnvironmentVariable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node


def generate_launch_description():
    pkg_nav = get_package_share_directory("tbot_navigation")
    pkg_desc = get_package_share_directory("tbot_description")
    pkg_ekf = get_package_share_directory("robot_localization_config")
    pkg_lidar = get_package_share_directory("lslidar_driver")

    urdf_file = os.path.join(pkg_desc, "urdf", "tbot.urdf")
    with open(urdf_file, "r", encoding="utf-8") as robot_file:
        robot_description = robot_file.read()

    default_params_file = os.path.join(pkg_nav, "config", "nav2_params.yaml")
    default_map_file = PathJoinSubstitution(
        [EnvironmentVariable("HOME"), "Desktop", "robot_move", "maps", "my_map.yaml"]
    )

    map_file = LaunchConfiguration("map")
    params_file = LaunchConfiguration("params_file")
    use_sim_time = LaunchConfiguration("use_sim_time")
    autostart = LaunchConfiguration("autostart")
    log_level = LaunchConfiguration("log_level")

    start_base = LaunchConfiguration("start_base")
    start_ekf = LaunchConfiguration("start_ekf")
    start_lidar = LaunchConfiguration("start_lidar")
    start_description = LaunchConfiguration("start_description")
    start_nav2 = LaunchConfiguration("start_nav2")

    base_node = Node(
        condition=IfCondition(start_base),
        package="my_base",
        executable="my_base",
        name="wheeltec_robot",
        output="screen",
        parameters=[{"pub_odom_tf": False}],
    )

    ekf_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ekf, "launch", "localization_launch.py")
        ),
        condition=IfCondition(start_ekf),
        launch_arguments={"use_sim_time": use_sim_time}.items(),
    )

    lidar_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(pkg_lidar, "launch", "lsn10p_launch.py")),
        condition=IfCondition(start_lidar),
    )

    robot_state_publisher_node = Node(
        condition=IfCondition(start_description),
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="screen",
        parameters=[
            {
                "use_sim_time": use_sim_time,
                "robot_description": robot_description,
            }
        ],
    )

    nav2_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(pkg_nav, "launch", "nav2_amcl_launch.py")),
        condition=IfCondition(start_nav2),
        launch_arguments={
            "map": map_file,
            "params_file": params_file,
            "use_sim_time": use_sim_time,
            "autostart": autostart,
            "log_level": log_level,
        }.items(),
    )

    delayed_nav2 = TimerAction(period=3.0, actions=[nav2_launch])

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "map",
                default_value=default_map_file,
                description="Full path to the saved map yaml file.",
            ),
            DeclareLaunchArgument(
                "params_file",
                default_value=default_params_file,
                description="Full path to the Nav2 parameters file.",
            ),
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="false",
                description="Use simulation clock if true.",
            ),
            DeclareLaunchArgument(
                "autostart",
                default_value="true",
                description="Automatically activate Nav2 lifecycle nodes.",
            ),
            DeclareLaunchArgument(
                "log_level",
                default_value="info",
                description="Nav2 logging level.",
            ),
            DeclareLaunchArgument(
                "start_base",
                default_value="true",
                description="Start my_base with pub_odom_tf disabled.",
            ),
            DeclareLaunchArgument(
                "start_ekf",
                default_value="true",
                description="Start robot_localization EKF.",
            ),
            DeclareLaunchArgument(
                "start_lidar",
                default_value="true",
                description="Start the LSLidar driver.",
            ),
            DeclareLaunchArgument(
                "start_description",
                default_value="true",
                description="Start robot_state_publisher with tbot.urdf.",
            ),
            DeclareLaunchArgument(
                "start_nav2",
                default_value="true",
                description="Start Nav2 + AMCL.",
            ),
            base_node,
            ekf_launch,
            lidar_launch,
            robot_state_publisher_node,
            delayed_nav2,
        ]
    )
