from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    return LaunchDescription([

        Node(
            package='accel_system_cmd',
            executable='esp_receiver_node',
            name='esp_receiver_node'
        ),

        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim'
        ),

        Node(
            package='accel_system_cmd',
            executable='turtle_control',
            name='turtle_control'
        ),

        Node(
            package='accel_system_cmd',
            executable='turtle_pose_serial_node',
            name='pose_serial'
        ),
    ])