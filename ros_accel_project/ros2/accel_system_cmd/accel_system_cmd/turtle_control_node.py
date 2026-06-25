import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist


class TurtleControl(Node):

    def __init__(self):
        super().__init__('turtle_control')

        self.subscription = self.create_subscription(
            Float32MultiArray,
            '/accel',
            self.accel_callback,
            10
        )

        self.publisher = self.create_publisher(
            Twist,
            '/turtle1/cmd_vel',
            10
        )

        self.deadband = 1.0

        self.get_logger().info("Turtle control node started")

    def accel_callback(self, msg):

        x = msg.data[0]
        y = msg.data[1]

        # Zone morte
        if abs(x) < self.deadband:
            x = 0.0

        if abs(y) < self.deadband:
            y = 0.0

        cmd = Twist()

        # Avancer / reculer
        cmd.linear.x = y * 0.2

        # Tourner
        cmd.angular.z = x * 0.2

        self.publisher.publish(cmd)

        self.get_logger().info(
            f"X={x:.2f}  Y={y:.2f}  "
            f"V={cmd.linear.x:.2f}  W={cmd.angular.z:.2f}"
        )


def main(args=None):

    rclpy.init(args=args)

    node = TurtleControl()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()