# CMake generated Testfile for 
# Source directory: /home/pi/catkinws/src/laser_geometry
# Build directory: /home/pi/catkinws/build_isolated/laser_geometry
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_laser_geometry_gtest_projection_test "/home/pi/catkinws/build_isolated/laser_geometry/catkin_generated/env_cached.sh" "/usr/bin/python" "/opt/ros/indigo/share/catkin/cmake/test/run_tests.py" "/home/pi/catkinws/build_isolated/laser_geometry/test_results/laser_geometry/gtest-projection_test.xml" "--return-code" "/home/pi/catkinws/devel_isolated/laser_geometry/lib/laser_geometry/projection_test --gtest_output=xml:/home/pi/catkinws/build_isolated/laser_geometry/test_results/laser_geometry/gtest-projection_test.xml")
add_test(_ctest_laser_geometry_nosetests_test.projection_test.py "/home/pi/catkinws/build_isolated/laser_geometry/catkin_generated/env_cached.sh" "/usr/bin/python" "/opt/ros/indigo/share/catkin/cmake/test/run_tests.py" "/home/pi/catkinws/build_isolated/laser_geometry/test_results/laser_geometry/nosetests-test.projection_test.py.xml" "--return-code" "/usr/bin/cmake -E make_directory /home/pi/catkinws/build_isolated/laser_geometry/test_results/laser_geometry" "/usr/bin/nosetests-2.7 -P --process-timeout=60 /home/pi/catkinws/src/laser_geometry/test/projection_test.py --with-xunit --xunit-file=/home/pi/catkinws/build_isolated/laser_geometry/test_results/laser_geometry/nosetests-test.projection_test.py.xml")
subdirs(gtest)
