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
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(serial QUIET)

add_executable(my_base src/my_base.cpp)
ament_target_dependencies(my_base nav_msgs rclcpp tf2_geometry_msgs geometry_msgs std_msgs sensor_msgs)

if(serial_FOUND)
  if(TARGET serial::serial)
    target_link_libraries(my_base serial::serial)
  elseif(TARGET serial)
    target_link_libraries(my_base serial)
  elseif(DEFINED serial_LIBRARIES)
    target_link_libraries(my_base ${serial_LIBRARIES})
  endif()
  if(DEFINED serial_INCLUDE_DIRS)
    target_include_directories(my_base PRIVATE ${serial_INCLUDE_DIRS})
  endif()
else()
  target_include_directories(my_base PRIVATE /usr/local/include)
  target_link_libraries(my_base /usr/local/lib/libserial.so)
endif()


install(PROGRAMS
  scripts/fan_keyboard.py
  DESTINATION lib/${PROJECT_NAME}
)


install(TARGETS
  my_base
  DESTINATION lib/${PROJECT_NAME}
)
ament_package()
