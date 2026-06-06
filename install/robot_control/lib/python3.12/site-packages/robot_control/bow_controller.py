#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist


class BowController(Node):
    def __init__(self):
        super().__init__('bow_controller')
        self.cmd_pub = self.create_publisher(
            Twist,
            '/cmd_vel',
            10
        )
        self.timer = self.create_timer(
            0.1,
            self.control_loop
        )
        # 速度参数
        self.linear_speed = -0.2
        self.angular_speed = 0.5
        # 时间参数
        self.forward_time = 3.0
        self.shift_time = 1.5
        self.turn_time = 3.14
        # 状态机
        self.state = 0
        # 转向方向
        self.turn_left = True
        # ===== 新增：循环控制 =====
        self.cycle_count = 0
        self.max_cycles = 3   # 只走 3 次弓字
        self.state_start_time = self.get_clock().now()
        self.get_logger().info("开始弓字型运动")
    def control_loop(self):
        now = self.get_clock().now()
        elapsed = (
            now - self.state_start_time
        ).nanoseconds / 1e9
        msg = Twist()
        # 0：直走
        if self.state == 0:
            msg.linear.x = self.linear_speed
            if elapsed > self.forward_time:
                self.state = 1
                self.state_start_time = now
        # 1：第一次转弯
        elif self.state == 1:
            if self.turn_left:
                msg.angular.z = self.angular_speed
            else:
                msg.angular.z = -self.angular_speed
            if elapsed > self.turn_time:
                self.state = 2
                self.state_start_time = now
        # 2：行间距
        elif self.state == 2:
            msg.linear.x = self.linear_speed
            if elapsed > self.shift_time:
                self.state = 3
                self.state_start_time = now

        # 3：第二次转弯
        elif self.state == 3:
            if self.turn_left:
                msg.angular.z = self.angular_speed
            else:
                msg.angular.z = -self.angular_speed
            if elapsed > self.turn_time:
                # 切换方向（弓字关键）
                self.turn_left = not self.turn_left
                # ===== 关键：计数 =====
                self.cycle_count += 1
                self.get_logger().info(
                    f"完成弓字次数: {self.cycle_count}"
                )
                if self.cycle_count >= self.max_cycles:
                    self.stop_robot()
                    self.get_logger().info(
                        "达到最大次数，停止"
                    )
                    return
                self.state = 0
                self.state_start_time = now
        self.cmd_pub.publish(msg)

    def stop_robot(self):
        msg = Twist()
        self.cmd_pub.publish(msg)
        self.get_logger().info("机器人停止")


def main():
    rclpy.init()
    node = BowController()
    try:

        rclpy.spin(node)
    except KeyboardInterrupt:
        node.stop_robot()

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
