from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'imu_mpu6050'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        # Include launch files
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
        # Include config files
        (os.path.join('share', package_name, 'config'), glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Your Name',
    maintainer_email='your_email@example.com',
    description='MPU6050 IMU driver for ROS2 - publishes sensor_msgs/Imu messages',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'mpu6050_node = imu_mpu6050.mpu6050_node:main',
        ],
    },
)