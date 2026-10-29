cmake_minimum_required(VERSION 3.0.2)
project(ros_walker_bot)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(move_bot src/move_bot.cpp)
target_link_libraries(move_bot ${catkin_LIBRARIES})
