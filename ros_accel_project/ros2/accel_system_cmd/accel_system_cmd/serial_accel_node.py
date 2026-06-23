import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray
import serial

class SerialAccelNode(Node):

    def __init__(self):
        super().__init__('serial_accel_node')

        self.pub = self.create_publisher(Float32MultiArray, '/accel', 10)

        self.serial_port = serial.Serial('/dev/ttyACM1', 115200, timeout=1)

        self.timer = self.create_timer(0.1, self.read_serial)

    def read_serial(self):

        line = self.serial_port.readline().decode().strip()

        if line:
            try:
                x, y, z = map(float, line.split(','))

                msg = Float32MultiArray()
                msg.data = [x, y, z]

                self.pub.publish(msg)

                self.get_logger().info(str(msg.data))

            except:
                pass


def main():
    rclpy.init()
    node = SerialAccelNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()