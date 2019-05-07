cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_REQUIRED_FLAGS -std=c++17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


add_executable(SystemMonitor ${SOURCES})
target_compile_options(SystemMonitor PRIVATE "-w")
set_property(TARGET SystemMonitor PROPERTY CXX_STANDARD 17)
target_link_libraries(SystemMonitor ${CURSES_LIBRARIES})


