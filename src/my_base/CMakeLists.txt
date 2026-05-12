cmake_minimum_required(VERSION 3.8)
project(my_base)

# 设置运行时库搜索路径
set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)



# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

# 直接定义串口库的路径和头文件路径
set(SERIAL_LIB "/usr/local/lib/libserial.so")
include_directories(/usr/local/include)

add_executable(my_base src/my_base.cpp)
ament_target_dependencies(my_base nav_msgs rclcpp tf2_geometry_msgs)
target_link_libraries(my_base ${SERIAL_LIB})

#ament_target_dependencies(my_base  serial )
install(TARGETS
  my_base
  DESTINATION lib/${PROJECT_NAME}
)
ament_package()
