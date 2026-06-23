from setuptools import find_packages, setup

package_name = 'accel_system_cmd'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='dabirakoro',
    maintainer_email='dabirakoro@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
        'serial_accel_node = accel_system_cmd.serial_accel_node:main',
        'view_node = accel_system_cmd.view_node:main',
        'http_bridge = accel_system_cmd.http_bridge:main',
        'turtle_control = accel_system_cmd.turtle_control_node:main',
        'esp_receiver_node = accel_system_cmd.esp_receiver_node:main',
    ],
    },
)
