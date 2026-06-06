#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class CircleController(Node):
    def __init__(self):
        super().__init__('circle_controller')
        self.cmd_pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.timer = self.create_timer(0.1, self.publish_vel)
        self.get_logger().info("开始走圆形")

    def publish_vel(self):
        msg = Twist()
        msg.linear.x = 0.05       # 前进速度
        msg.angular.z = 0.50    # 转弯速度
        self.cmd_pub.publish(msg)

def main():
    rclpy.init()
    node = CircleController()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

