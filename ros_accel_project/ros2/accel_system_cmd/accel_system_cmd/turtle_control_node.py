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
        # Variables du filtre
        self.fx = 0.0
        self.fy = 0.0

        # Coefficient de filtrage
        self.alpha = 0.4

        # Zone morte
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

           # Filtre exponentiel
        self.fx = (1 - self.alpha) * self.fx + self.alpha * x
        self.fy = (1 - self.alpha) * self.fy + self.alpha * y

        cmd = Twist()

         # Commande
        cmd.linear.x = self.fy * 0.2
        cmd.angular.z = self.fx * 0.2

         

        self.publisher.publish(cmd)

        self.get_logger().info(
        f"X={self.fx:.2f} "
        f"Y={self.fy:.2f} "
        f"V={cmd.linear.x:.2f} "
        f"W={cmd.angular.z:.2f}"
    )

def main(args=None):

    rclpy.init(args=args)

    node = TurtleControl()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()