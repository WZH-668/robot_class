#!/usr/bin/env python3
"""Launch Nav2 in localization mode with map_server + AMCL.

This launch file assumes the robot base, EKF, lidar driver, and robot
description are already running. Use bringup_navigation.launch.py when you
want this package to start the robot-side nodes as well.
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import EnvironmentVariable, LaunchConfiguration, PathJoinSubstitution


def generate_launch_description():
    pkg_nav = get_package_share_directory("tbot_navigation")
    nav2_bringup_dir = get_package_share_directory("nav2_bringup")

    default_params_file = os.path.join(pkg_nav, "config", "nav2_params.yaml")
    default_map_file = PathJoinSubstitution(
        [EnvironmentVariable("HOME"), "Desktop", "robot_move", "maps", "my_map.yaml"]
    )

    namespace = LaunchConfiguration("namespace")
    use_namespace = LaunchConfiguration("use_namespace")
    map_file = LaunchConfiguration("map")
    use_sim_time = LaunchConfiguration("use_sim_time")
    params_file = LaunchConfiguration("params_file")
    autostart = LaunchConfiguration("autostart")
    use_composition = LaunchConfiguration("use_composition")
    use_respawn = LaunchConfiguration("use_respawn")
    log_level = LaunchConfiguration("log_level")

    bringup_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(nav2_bringup_dir, "launch", "bringup_launch.py")
        ),
        launch_arguments={
            "namespace": namespace,
            "use_namespace": use_namespace,
            "slam": "False",
            "map": map_file,
            "use_sim_time": use_sim_time,
            "params_file": params_file,
            "autostart": autostart,
            "use_composition": use_composition,
            "use_respawn": use_respawn,
            "log_level": log_level,
        }.items(),
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "namespace",
                default_value="",
                description="Top-level namespace for Nav2 nodes.",
            ),
            DeclareLaunchArgument(
                "use_namespace",
                default_value="false",
                description="Whether to apply the namespace to Nav2 nodes.",
            ),
            DeclareLaunchArgument(
                "map",
                default_value=default_map_file,
                description="Full path to the saved map yaml file.",
            ),
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="false",
                description="Use simulation clock if true.",
            ),
            DeclareLaunchArgument(
                "params_file",
                default_value=default_params_file,
                description="Full path to the Nav2 parameters file.",
            ),
            DeclareLaunchArgument(
                "autostart",
                default_value="true",
                description="Automatically configure and activate Nav2 lifecycle nodes.",
            ),
            DeclareLaunchArgument(
                "use_composition",
                default_value="false",
                description="Run Nav2 nodes in a composed container if true.",
            ),
            DeclareLaunchArgument(
                "use_respawn",
                default_value="false",
                description="Respawn Nav2 nodes if they crash.",
            ),
            DeclareLaunchArgument(
                "log_level",
                default_value="info",
                description="Nav2 logging level.",
            ),
            bringup_launch,
        ]
    )
