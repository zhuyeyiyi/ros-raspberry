# CMake generated Testfile for 
# Source directory: /home/pi/catkinws/src/vision_opencv/image_geometry/test
# Build directory: /home/pi/catkinws/build_isolated/image_geometry/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_image_geometry_nosetests_directed.py "/home/pi/catkinws/build_isolated/image_geometry/catkin_generated/env_cached.sh" "/usr/bin/python" "/opt/ros/indigo/share/catkin/cmake/test/run_tests.py" "/home/pi/catkinws/build_isolated/image_geometry/test_results/image_geometry/nosetests-directed.py.xml" "--return-code" "/usr/bin/cmake -E make_directory /home/pi/catkinws/build_isolated/image_geometry/test_results/image_geometry" "/usr/bin/nosetests-2.7 -P --process-timeout=60 /home/pi/catkinws/src/vision_opencv/image_geometry/test/directed.py --with-xunit --xunit-file=/home/pi/catkinws/build_isolated/image_geometry/test_results/image_geometry/nosetests-directed.py.xml")
add_test(_ctest_image_geometry_gtest_image_geometry-utest "/home/pi/catkinws/build_isolated/image_geometry/catkin_generated/env_cached.sh" "/usr/bin/python" "/opt/ros/indigo/share/catkin/cmake/test/run_tests.py" "/home/pi/catkinws/build_isolated/image_geometry/test_results/image_geometry/gtest-image_geometry-utest.xml" "--return-code" "/home/pi/catkinws/devel_isolated/image_geometry/lib/image_geometry/image_geometry-utest --gtest_output=xml:/home/pi/catkinws/build_isolated/image_geometry/test_results/image_geometry/gtest-image_geometry-utest.xml")
