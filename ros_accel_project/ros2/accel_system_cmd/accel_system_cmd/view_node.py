import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import json

class AccelView(Node):
    def __init__(self):
        super().__init__('accel_view')

        self.sub = self.create_subscription(
            String,
            '/accel',
            self.callback,
            10
        )

    def callback(self, msg):
        data = json.loads(msg.data)

        self.get_logger().info(
            f"X={data['x']} | Y={data['y']} | Z={data['z']}"
        )


def main():
    rclpy.init()
    node = AccelView()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()