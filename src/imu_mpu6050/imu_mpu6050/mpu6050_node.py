#!/usr/bin/env python3
"""
MPU6050 IMU Driver Node for ROS2
Reads accelerometer and gyroscope data from MPU6050 via I2C
and publishes sensor_msgs/Imu messages.

Author: ROS2 Developer
Date: 2026
"""

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from std_msgs.msg import Header
import math
import time

# Try to import smbus2, fallback to smbus if not available
try:
    from smbus2 import SMBus
    I2C_LIB = 'smbus2'
except ImportError:
    try:
        from smbus import SMBus
        I2C_LIB = 'smbus'
    except ImportError:
        SMBus = None
        I2C_LIB = None


class MPU6050Driver(Node):
    """
    ROS2 Node for MPU6050 IMU sensor
    """

    # MPU6050 I2C addresses
    MPU6050_ADDR_0x68 = 0x68  # Default address with AD0 pin LOW
    MPU6050_ADDR_0x69 = 0x69  # Address with AD0 pin HIGH

    # MPU6050 Registers
    PWR_MGMT_1 = 0x6B
    PWR_MGMT_2 = 0x6C
    ACCEL_XOUT_H = 0x3B
    ACCEL_YOUT_H = 0x3D
    ACCEL_ZOUT_H = 0x3F
    GYRO_XOUT_H = 0x43
    GYRO_YOUT_H = 0x45
    GYRO_ZOUT_H = 0x47
    CONFIG = 0x1A
    GYRO_CONFIG = 0x1B
    ACCEL_CONFIG = 0x1C
    WHO_AM_I = 0x75

    # Scale factors
    ACCEL_SCALE_2G = 16384.0  # LSB/g
    ACCEL_SCALE_4G = 8192.0
    ACCEL_SCALE_8G = 4096.0
    ACCEL_SCALE_16G = 2048.0

    GYRO_SCALE_250DEG = 131.0  # LSB/(deg/s)
    GYRO_SCALE_500DEG = 65.5
    GYRO_SCALE_1000DEG = 32.8
    GYRO_SCALE_2000DEG = 16.4

    def __init__(self):
        super().__init__('mpu6050_node')

        # Declare parameters
        self.declare_parameter('i2c_bus', 1)
        self.declare_parameter('i2c_address', 0x68)
        self.declare_parameter('frame_id', 'imu_Link')
        self.declare_parameter('publish_rate', 50.0)
        self.declare_parameter('accel_range', 2)  # 2, 4, 8, or 16 g
        self.declare_parameter('gyro_range', 250)  # 250, 500, 1000, or 2000 deg/s
        self.declare_parameter('enable_calibration', True)
        self.declare_parameter('calibration_samples', 100)

        # Get parameters
        self.i2c_bus = self.get_parameter('i2c_bus').value
        self.i2c_address = self.get_parameter('i2c_address').value
        self.frame_id = self.get_parameter('frame_id').value
        self.publish_rate = self.get_parameter('publish_rate').value
        self.accel_range = self.get_parameter('accel_range').value
        self.gyro_range = self.get_parameter('gyro_range').value
        self.enable_calibration = self.get_parameter('enable_calibration').value
        self.calibration_samples = self.get_parameter('calibration_samples').value

        # Check if I2C library is available
        if SMBus is None:
            self.get_logger().error(
                'No I2C library available! Please install smbus2 or smbus:\n'
                '  pip3 install smbus2\n'
                '  or: sudo apt install python3-smbus'
            )
            raise ImportError('I2C library not found')

        self.get_logger().info(f'Using I2C library: {I2C_LIB}')

        # Initialize I2C bus
        try:
            self.bus = SMBus(self.i2c_bus)
            self.get_logger().info(f'Opened I2C bus {self.i2c_bus}')
        except Exception as e:
            self.get_logger().error(f'Failed to open I2C bus {self.i2c_bus}: {e}')
            raise

        # Initialize MPU6050
        self.initialize_mpu6050()

        # Get scale factors
        self.accel_scale = self.get_accel_scale(self.accel_range)
        self.gyro_scale = self.get_gyro_scale(self.gyro_range)

        # Calibration offsets
        self.accel_offset = [0.0, 0.0, 0.0]
        self.gyro_offset = [0.0, 0.0, 0.0]

        # Perform calibration if enabled
        if self.enable_calibration:
            self.calibrate()

        # Create publisher
        self.imu_publisher = self.create_publisher(Imu, '/imu/data_raw', 10)

        # Create timer for publishing
        timer_period = 1.0 / self.publish_rate  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)

        self.get_logger().info(
            f'MPU6050 node initialized:\n'
            f'  I2C Bus: {self.i2c_bus}\n'
            f'  I2C Address: 0x{self.i2c_address:02X}\n'
            f'  Frame ID: {self.frame_id}\n'
            f'  Publish Rate: {self.publish_rate} Hz\n'
            f'  Accel Range: {self.accel_range}g\n'
            f'  Gyro Range: {self.gyro_range} deg/s\n'
            f'  Calibration: {self.enable_calibration}'
        )

    def initialize_mpu6050(self):
        """Initialize MPU6050 sensor"""
        try:
            # Check WHO_AM_I register
            who_am_i = self.bus.read_byte_data(self.i2c_address, self.WHO_AM_I)
            if who_am_i != 0x68:
                self.get_logger().warning(
                    f'WHO_AM_I returned 0x{who_am_i:02X}, expected 0x68. '
                    f'Device might not be MPU6050 or address might be wrong.'
                )

            # Wake up MPU6050 (write 0 to PWR_MGMT_1)
            self.bus.write_byte_data(self.i2c_address, self.PWR_MGMT_1, 0x00)

            # Set accelerometer range
            accel_config_val = self.get_accel_config_value(self.accel_range)
            self.bus.write_byte_data(self.i2c_address, self.ACCEL_CONFIG, accel_config_val)

            # Set gyroscope range
            gyro_config_val = self.get_gyro_config_value(self.gyro_range)
            self.bus.write_byte_data(self.i2c_address, self.GYRO_CONFIG, gyro_config_val)

            # Set digital low pass filter (DLPF) for better noise reduction
            # DLPF_CFG = 3 (Accel: 44Hz, Gyro: 42Hz)
            self.bus.write_byte_data(self.i2c_address, self.CONFIG, 0x03)

            time.sleep(0.1)  # Wait for sensor to stabilize

            self.get_logger().info('MPU6050 initialized successfully')

        except Exception as e:
            self.get_logger().error(f'Failed to initialize MPU6050: {e}')
            raise

    def get_accel_config_value(self, range_g):
        """Get accelerometer configuration value for given range"""
        config_values = {
            2: 0x00,   # ±2g
            4: 0x08,   # ±4g
            8: 0x10,   # ±8g
            16: 0x18,  # ±16g
        }
        return config_values.get(range_g, 0x00)

    def get_gyro_config_value(self, range_deg):
        """Get gyroscope configuration value for given range"""
        config_values = {
            250: 0x00,   # ±250°/s
            500: 0x08,   # ±500°/s
            1000: 0x10,  # ±1000°/s
            2000: 0x18,  # ±2000°/s
        }
        return config_values.get(range_deg, 0x00)

    def get_accel_scale(self, range_g):
        """Get accelerometer scale factor for given range"""
        scales = {
            2: self.ACCEL_SCALE_2G,
            4: self.ACCEL_SCALE_4G,
            8: self.ACCEL_SCALE_8G,
            16: self.ACCEL_SCALE_16G,
        }
        return scales.get(range_g, self.ACCEL_SCALE_2G)

    def get_gyro_scale(self, range_deg):
        """Get gyroscope scale factor for given range"""
        scales = {
            250: self.GYRO_SCALE_250DEG,
            500: self.GYRO_SCALE_500DEG,
            1000: self.GYRO_SCALE_1000DEG,
            2000: self.GYRO_SCALE_2000DEG,
        }
        return scales.get(range_deg, self.GYRO_SCALE_250DEG)

    def read_word(self, addr):
        """Read a 16-bit value from two consecutive registers"""
        high = self.bus.read_byte_data(self.i2c_address, addr)
        low = self.bus.read_byte_data(self.i2c_address, addr + 1)
        value = (high << 8) + low
        return value

    def read_word_2c(self, addr):
        """Read a signed 16-bit value from two consecutive registers"""
        value = self.read_word(addr)
        if value >= 0x8000:
            value = -((65535 - value) + 1)
        return value

    def calibrate(self):
        """Perform simple calibration by averaging readings when stationary"""
        self.get_logger().info(
            f'Starting calibration with {self.calibration_samples} samples... '
            f'Keep the robot stationary!'
        )

        accel_sum = [0.0, 0.0, 0.0]
        gyro_sum = [0.0, 0.0, 0.0]

        for i in range(self.calibration_samples):
            accel_raw = self.read_raw_accel()
            gyro_raw = self.read_raw_gyro()

            for j in range(3):
                accel_sum[j] += accel_raw[j]
                gyro_sum[j] += gyro_raw[j]

            if i % 10 == 0:
                self.get_logger().info(f'Calibration progress: {i}/{self.calibration_samples}')

            time.sleep(0.01)

        # Calculate offsets
        for j in range(3):
            self.accel_offset[j] = accel_sum[j] / self.calibration_samples
            self.gyro_offset[j] = gyro_sum[j] / self.calibration_samples

        # For Z-axis accelerometer, we expect ~1g when stationary (upward)
        # So we adjust the offset to make Z-axis read 1g when stationary
        # Note: This assumes the IMU is mounted flat with Z-axis pointing up
        self.accel_offset[2] -= self.accel_scale  # Subtract expected 1g

        self.get_logger().info(
            f'Calibration complete:\n'
            f'  Accel offsets: [{self.accel_offset[0]:.3f}, '
            f'{self.accel_offset[1]:.3f}, {self.accel_offset[2]:.3f}]\n'
            f'  Gyro offsets: [{self.gyro_offset[0]:.3f}, '
            f'{self.gyro_offset[1]:.3f}, {self.gyro_offset[2]:.3f}]'
        )

    def read_raw_accel(self):
        """Read raw accelerometer data"""
        accel_x = self.read_word_2c(self.ACCEL_XOUT_H)
        accel_y = self.read_word_2c(self.ACCEL_YOUT_H)
        accel_z = self.read_word_2c(self.ACCEL_ZOUT_H)
        return [accel_x, accel_y, accel_z]

    def read_raw_gyro(self):
        """Read raw gyroscope data"""
        gyro_x = self.read_word_2c(self.GYRO_XOUT_H)
        gyro_y = self.read_word_2c(self.GYRO_YOUT_H)
        gyro_z = self.read_word_2c(self.GYRO_ZOUT_H)
        return [gyro_x, gyro_y, gyro_z]

    def timer_callback(self):
        """Timer callback to read and publish IMU data"""
        try:
            # Read raw data
            accel_raw = self.read_raw_accel()
            gyro_raw = self.read_raw_gyro()

            # Apply calibration offsets
            accel_calibrated = [
                (accel_raw[0] - self.accel_offset[0]) / self.accel_scale,
                (accel_raw[1] - self.accel_offset[1]) / self.accel_scale,
                (accel_raw[2] - self.accel_offset[2]) / self.accel_scale,
            ]

            gyro_calibrated = [
                (gyro_raw[0] - self.gyro_offset[0]) / self.gyro_scale,
                (gyro_raw[1] - self.gyro_offset[1]) / self.gyro_scale,
                (gyro_raw[2] - self.gyro_offset[2]) / self.gyro_scale,
            ]

            # Convert gyroscope readings from deg/s to rad/s
            gyro_rad = [
                gyro_calibrated[0] * math.pi / 180.0,
                gyro_calibrated[1] * math.pi / 180.0,
                gyro_calibrated[2] * math.pi / 180.0,
            ]

            # Create Imu message
            imu_msg = Imu()

            # Header
            imu_msg.header = Header()
            imu_msg.header.stamp = self.get_clock().now().to_msg()
            imu_msg.header.frame_id = self.frame_id

            # Linear acceleration (m/s^2)
            # Assuming accel_scale is LSB/g, and 1g = 9.80665 m/s^2
            accel_ms2 = [
                accel_calibrated[0] * 9.80665,
                accel_calibrated[1] * 9.80665,
                accel_calibrated[2] * 9.80665,
            ]
            imu_msg.linear_acceleration.x = accel_ms2[0]
            imu_msg.linear_acceleration.y = accel_ms2[1]
            imu_msg.linear_acceleration.z = accel_ms2[2]

            # Angular velocity (rad/s)
            imu_msg.angular_velocity.x = gyro_rad[0]
            imu_msg.angular_velocity.y = gyro_rad[1]
            imu_msg.angular_velocity.z = gyro_rad[2]

            # Orientation - NOT provided by MPU6050 alone
            # Set to identity quaternion with covariance indicating unknown
            imu_msg.orientation.x = 0.0
            imu_msg.orientation.y = 0.0
            imu_msg.orientation.z = 0.0
            imu_msg.orientation.w = 1.0

            # Covariance matrices
            # For orientation: -1 indicates unknown/not provided
            imu_msg.orientation_covariance = [
                -1.0, 0.0, 0.0,
                0.0, -1.0, 0.0,
                0.0, 0.0, -1.0
            ]

            # For angular velocity: reasonable estimates
            # These values are typical for MPU6050
            gyro_var = 0.01  # rad/s variance estimate
            imu_msg.angular_velocity_covariance = [
                gyro_var, 0.0, 0.0,
                0.0, gyro_var, 0.0,
                0.0, 0.0, gyro_var
            ]

            # For linear acceleration: reasonable estimates
            accel_var = 0.02  # m/s^2 variance estimate
            imu_msg.linear_acceleration_covariance = [
                accel_var, 0.0, 0.0,
                0.0, accel_var, 0.0,
                0.0, 0.0, accel_var
            ]

            # Publish message
            self.imu_publisher.publish(imu_msg)

        except Exception as e:
            self.get_logger().error(f'Error reading MPU6050: {e}')

    def destroy_node(self):
        """Clean up when node is destroyed"""
        if hasattr(self, 'bus'):
            self.bus.close()
            self.get_logger().info('Closed I2C bus')
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)

    try:
        mpu6050_node = MPU6050Driver()
        rclpy.spin(mpu6050_node)
    except Exception as e:
        print(f'Failed to start MPU6050 node: {e}')
    finally:
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()