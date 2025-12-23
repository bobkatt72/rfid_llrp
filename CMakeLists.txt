cmake_minimum_required(VERSION 3.8)
project(rfid_llrp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/TagObservation.msg"
  DEPENDENCIES builtin_interfaces
)

add_library(tcp_client
  src/tcp_client.cpp
)

target_include_directories(tcp_client PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(tcp_test src/tcp_test.cpp)
target_link_libraries(tcp_test tcp_client)
install(TARGETS tcp_test DESTINATION lib/${PROJECT_NAME})

add_library(llrp_framing
  src/llrp_framing.cpp
)

target_include_directories(llrp_framing PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(llrp_framing
  tcp_client
)

add_library(llrp_encoder
  src/llrp_encoder.cpp
)

target_include_directories(llrp_encoder PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(get_caps_test
  src/get_caps_test.cpp
)

target_link_libraries(get_caps_test
  tcp_client
  llrp_framing
  llrp_encoder
  llrp_status
)

install(TARGETS get_caps_test
  DESTINATION lib/${PROJECT_NAME}
)

add_library(llrp_status
  src/llrp_status.cpp
)

target_include_directories(llrp_status PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_export_dependencies(rosidl_default_runtime)

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

ament_package()
