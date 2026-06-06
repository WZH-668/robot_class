import rclpy
import math
from rclpy.node import Node
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

class LaserTrackNode(Node):
    def __init__(self):
        super().__init__('laser_track_node')

        # 雷达订阅
        self.laser_sub = self.create_subscription(
            LaserScan,
            '/scan',
            self.scan_callback,
            10
        )
        # 底盘控制发布
        self.cmd_vel_pub = self.create_publisher(Twist, '/cmd_vel', 10)

        # 核心参数
        self.min_valid_dist = 0.2       # 20cm以内过滤
        self.angle_range_limit = 180.0    # 只处理 ±180° 范围内目标
        self.angular_kp = 0.03           # 转向比例系数，可微调

        self.get_logger().info("✅ 雷达自动对准节点启动：20cm外最近点 + ±180°范围内自动朝向")

    def scan_callback(self, msg):
        ranges = msg.ranges
        angle_min = msg.angle_min
        angle_inc = msg.angle_increment

        nearest_dist = float('inf')
        nearest_angle = 0.0

        # 遍历所有雷达点
        for idx, dist in enumerate(ranges):
            # 过滤无效距离 + 20cm以内
            if not math.isfinite(dist) or dist < self.min_valid_dist:
                continue

            # 计算当前点角度(机器人坐标系：0°正前，左+右-)
            current_rad = angle_min + idx * angle_inc
            current_deg = math.degrees(current_rad)
            current_deg = self.normalize_angle(current_deg)

            # 只保留 -20° ~ 20° 范围内的点
            if not (-self.angle_range_limit <= current_deg <= self.angle_range_limit):
                continue

            # 更新最近点
            if dist < nearest_dist:
                nearest_dist = dist
                nearest_angle = current_deg

        # 无有效目标：停止转向
        if nearest_dist == float('inf'):
            self.pub_vel(linear=0.0, angular=0.0)
            self.get_logger().info("无有效目标，保持静止")
            return

        # 角度偏差 = 目标点角度
        angle_err = nearest_angle
        # 比例控制转向
        angular_speed = angle_err * self.angular_kp

        # 限幅防止转向过快
        angular_speed = max(min(angular_speed, 0.5), -0.5)

        # 发布控制指令，不前进，只原地转向对准
        self.pub_vel(linear=0.0, angular=angular_speed)
        self.get_logger().info(
            f"最近点距离:{nearest_dist:.2f}m | 目标角度:{nearest_angle:.1f}° | 转向速度:{angular_speed:.3f}"
        )

    def normalize_angle(self, angle):
        """角度归一到 -180 ~ 180"""
        angle %= 360.0
        if angle > 180.0:
            angle -= 360.0
        return angle

    def pub_vel(self, linear, angular):
        twist = Twist()
        twist.linear.x = linear
        twist.angular.z = angular
        self.cmd_vel_pub.publish(twist)

def main(args=None):
    rclpy.init(args=args)
    node = LaserTrackNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
    