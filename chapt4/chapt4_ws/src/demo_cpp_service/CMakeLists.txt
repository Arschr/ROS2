cmake_minimum_required(VERSION 3.8)
project(demo_cpp_service)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(chapt4_interfaces REQUIRED)

# 1. 创建可执行程序
add_executable(turtle_control src/turtle_control.cpp)
add_executable(patrol_client src/patrol_client.cpp)


# 2. 添加依赖
ament_target_dependencies(turtle_control
  rclcpp
  geometry_msgs
  turtlesim
  chapt4_interfaces
)

ament_target_dependencies(patrol_client
  rclcpp
  chapt4_interfaces
)

# 3. 安装可执行程序
install(TARGETS
  turtle_control
  patrol_client
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()