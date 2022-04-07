cmake_minimum_required(VERSION 3.0.2)
project(collision_prompt)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES automatic_charging
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(COLLISION_PROMPT_NODE src/collision_prompt.cpp)
target_link_libraries(COLLISION_PROMPT_NODE ${catkin_LIBRARIES} -lpthread )

install(TARGETS COLLISION_PROMPT_NODE
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(
  DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(
  DIRECTORY voice
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
