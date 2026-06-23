import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist


class TurtleControl(Node):

    def __init__(self):
        super().__init__('turtle_control')

        # Abonnement à l'accéléromètre
        self.subscription = self.create_subscription(
            Float32MultiArray,
            '/accel',
            self.callback,
            10
        )

        # Publisher vers TurtleSim
        self.publisher = self.create_publisher(
            Twist,
            '/turtle1/cmd_vel',
            10
        )

        self.get_logger().info("Turtle control node started")

    def callback(self, msg):

        # -------------------------
        # Lecture des axes
        # -------------------------
        x = msg.data[0]   # gauche / droite
        y = msg.data[1]   # avant / arrière

        # -------------------------
        # CALIBRATION + NORMALISATION
        # -------------------------
        linear_x = -y / 5.0     # avance/recul (inversé pour naturel)
        angular_z = -x / 5.0     # rotation

        # -------------------------
        # ZONE MORTE (évite tremblements)
        # -------------------------
        if abs(linear_x) < 0.15:
            linear_x = 0.0

        if abs(angular_z) < 0.15:
            angular_z = 0.0

        # -------------------------
        # LIMITATION DES VITESSES
        # -------------------------
        if linear_x > 2.0:
            linear_x = 2.0
        if linear_x < -2.0:
            linear_x = -2.0

        if angular_z > 3.0:
            angular_z = 3.0
        if angular_z < -3.0:
            angular_z = -3.0

        # -------------------------
        # ENVOI COMMANDE TURTLESIM
        # -------------------------
        cmd = Twist()
        cmd.linear.x = linear_x
        cmd.angular.z = angular_z

        self.publisher.publish(cmd)

        # Log debug
        self.get_logger().info(
            f"x={x:.2f}, y={y:.2f} -> vx={linear_x:.2f}, wz={angular_z:.2f}"
        )


def main():
    rclpy.init()
    node = TurtleControl()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()