import rclpy
from rclpy.node import Node

from turtlesim.msg import Pose

import serial


class TurtlePoseSerialNode(Node):

    def __init__(self):
        super().__init__('turtle_pose_serial_node')

        # Adapter le port plus tard
        self.ser = serial.Serial(
            '/dev/ttyACM0',
            115200,
            timeout=1
        )

        self.subscription = self.create_subscription(
            Pose,
            '/turtle1/pose',
            self.pose_callback,
            10
        )

        self.get_logger().info("Turtle Pose Serial Node démarré")

    def pose_callback(self, msg):

        line = f"{msg.x:.2f},{msg.y:.2f},{msg.theta:.2f}\n"

        self.ser.write(line.encode())

        self.get_logger().info(
            f"Envoye : {line.strip()}"
        )


def main(args=None):

    rclpy.init(args=args)

    node = TurtlePoseSerialNode()

    rclpy.spin(node)

    node.ser.close()

    node.destroy_node()

    rclpy.shutdown()


if __name__ == '__main__':
    main()