import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray

from flask import Flask, request
import threading


# -------------------------
# ROS NODE
# -------------------------
class ESPReceiver(Node):

    def __init__(self):
        super().__init__('esp_receiver_node')

        self.publisher = self.create_publisher(
            Float32MultiArray,
            '/accel',
            10
        )

        self.get_logger().info("ESP Receiver Node started")


    def publish_data(self, x, y, z):

        msg = Float32MultiArray()
        msg.data = [x, y, z]

        self.publisher.publish(msg)

        self.get_logger().info(f"Published: {msg.data}")


# -------------------------
# FLASK SERVER
# -------------------------
app = Flask(__name__)
node = None


@app.route('/accel', methods=['POST'])
def accel():

    global node

    data = request.json

    x = float(data["x"])
    y = float(data["y"])
    z = float(data["z"])

    node.publish_data(x, y, z)

    return "OK", 200


# -------------------------
# ROS THREAD
# -------------------------
def ros_thread():

    global node

    rclpy.init()
    node = ESPReceiver()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


# -------------------------
# MAIN
# -------------------------
def main():

    print("ESP → ROS bridge started")

    t = threading.Thread(target=ros_thread, daemon=True)
    t.start()

    app.run(host='0.0.0.0', port=5000)


if __name__ == '__main__':
    main()