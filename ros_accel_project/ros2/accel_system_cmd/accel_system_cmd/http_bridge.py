import threading

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray

from flask import Flask, jsonify


# -------------------------
# Données partagées
# -------------------------
latest_data = {
    "x": 0.0,
    "y": 0.0,
    "z": 0.0
}


# -------------------------
# Noeud ROS
# -------------------------
class BridgeNode(Node):

    def __init__(self):
        super().__init__('bridge_node')

        self.subscription = self.create_subscription(
            Float32MultiArray,
            '/accel',
            self.callback,
            10
        )

    def callback(self, msg):
        global latest_data

        if len(msg.data) >= 3:

            latest_data["x"] = float(msg.data[0])
            latest_data["y"] = float(msg.data[1])
            latest_data["z"] = float(msg.data[2])

            self.get_logger().info(
                f"X={latest_data['x']:.2f} "
                f"Y={latest_data['y']:.2f} "
                f"Z={latest_data['z']:.2f}"
            )


# -------------------------
# Flask
# -------------------------
app = Flask(__name__)


@app.route('/accel')
def accel():
    return jsonify(latest_data)


# -------------------------
# Thread ROS
# -------------------------
def ros_thread():

    rclpy.init()

    node = BridgeNode()

    try:
        rclpy.spin(node)

    finally:
        node.destroy_node()
        rclpy.shutdown()


# -------------------------
# Main
# -------------------------
def main():

    print("Bridge ROS -> HTTP demarre")

    thread = threading.Thread(
        target=ros_thread,
        daemon=True
    )

    thread.start()

    app.run(
        host='0.0.0.0',
        port=5000
    )


if __name__ == '__main__':
    main()