# -*- coding: utf-8 -*-
from __future__ import print_function
import argparse
import os
import stat
import sys

# find the import for catkin's python package - either from source space or from an installed underlay
if os.path.exists(os.path.join('/opt/ros/indigo/share/catkin/cmake', 'catkinConfig.cmake.in')):
    sys.path.insert(0, os.path.join('/opt/ros/indigo/share/catkin/cmake', '..', 'python'))
try:
    from catkin.environment_cache import generate_environment_script
except ImportError:
    # search for catkin package in all workspaces and prepend to path
    for workspace in "/home/pi/catkinws/devel_isolated/imu;/home/pi/catkinws/devel_isolated/serial;/home/pi/catkinws/devel_isolated/vision_opencv;/home/pi/catkinws/devel_isolated/tf_conversions;/home/pi/catkinws/devel_isolated/laser_geometry;/home/pi/catkinws/devel_isolated/tf;/home/pi/catkinws/devel_isolated/rplidar_ros;/home/pi/catkinws/devel_isolated/orocos_kinematics_dynamics;/home/pi/catkinws/devel_isolated/kdl_conversions;/home/pi/catkinws/devel_isolated/eigen_conversions;/home/pi/catkinws/devel_isolated/opencv_tests;/home/pi/catkinws/devel_isolated/image_geometry;/home/pi/catkinws/devel_isolated/geometry;/home/pi/catkinws/devel_isolated/cv_bridge;/home/pi/catkinws/devel_isolated/angles;/home/pi/catkinws/devel;/opt/ros/indigo".split(';'):
        python_path = os.path.join(workspace, 'lib/python2.7/dist-packages')
        if os.path.isdir(os.path.join(python_path, 'catkin')):
            sys.path.insert(0, python_path)
            break
    from catkin.environment_cache import generate_environment_script

code = generate_environment_script('/home/pi/catkinws/devel_isolated/vision_opencv/env.sh')

output_filename = '/home/pi/catkinws/build_isolated/vision_opencv/catkin_generated/setup_cached.sh'
with open(output_filename, 'w') as f:
    #print('Generate script for cached setup "%s"' % output_filename)
    f.write('\n'.join(code))

mode = os.stat(output_filename).st_mode
os.chmod(output_filename, mode | stat.S_IXUSR)
