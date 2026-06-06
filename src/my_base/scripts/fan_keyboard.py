#!/usr/bin/env python3
import sys
import termios
import tty

import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool


class FanKeyboard(Node):
    def __init__(self):
        super().__init__("fan_keyboard")
        self.pub = self.create_publisher(Bool, "/fan_cmd", 10)

    def send(self, state):
        msg = Bool()
        msg.data = state
        self.pub.publish(msg)
        self.get_logger().info("fan on" if state else "fan off")


def get_key():
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        return sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)


def main():
    rclpy.init()
    node = FanKeyboard()

    print("f: fan on, g: fan off, q: quit")
    try:
        while rclpy.ok():
            key = get_key()
            if key == "f":
                node.send(True)
            elif key == "g":
                node.send(False)
            elif key == "q":
                break
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
