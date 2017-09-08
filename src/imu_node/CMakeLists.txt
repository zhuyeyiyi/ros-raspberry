cmake_minimum_required(VERSION 2.8.3)
project(imu_node)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  geometry_msgs
  message_generation
)

## Generate messages in the 'msg' folder
add_message_files(FILES Heading.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs sensor_msgs geometry_msgs message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(imu_pub src/imu_pub.cpp src/imu_data_decode.cpp src/packet.cpp)

target_link_libraries(imu_pub   ${catkin_LIBRARIES} )
