import rclpy
import math
from rclpy.node import Node
from sensor_msgs.msg import LaserScan
class LaserScanNode(Node):
    def __init__(self):
        super().__init__('laser_scan_node')
        self.sub = self.create_subscription(
            LaserScan,
            '/scan',
            self.scan_callback,
            10
        )
        self.get_logger().info("✅ 镭神雷达订阅成功")
    def scan_callback(self, msg):
        ranges = msg.ranges
        total = len(ranges)
        # --------------------- 镭神 0~360° 正确算法 ---------------------
        # 0°    = 正前方
        # 90°   = 正左方
        # 270°  = 正右方
        # ----------------------------------------------------------------
        front_idx = self.get_index(msg, 1.0)
        left_idx  = self.get_index(msg, 90.0)
        right_idx = self.get_index(msg, 270.0)
        # 获取有效距离
        front = self.get_valid(ranges, front_idx)
        left  = self.get_valid(ranges, left_idx)
        right = self.get_valid(ranges, right_idx)
        self.get_logger().info(f"前方：{front:.2f}m | 左：{left:.2f}m | 右：{right:.2f}m")
    def get_index(self, msg, angle_degree):
        """镭神雷达专用：角度转索引（0~360°）"""
        angle_rad = math.radians(angle_degree)
        index = int((angle_rad - msg.angle_min) / msg.angle_increment)
        return index
    def get_valid(self, ranges, idx):
        if 0 <= idx < len(ranges):
            dist = ranges[idx]
            if math.isfinite(dist):
                return dist
        return 0.0
def main(args=None):
    rclpy.init(args=args)
    node = LaserScanNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()
if __name__ == '__main__':
    main()
