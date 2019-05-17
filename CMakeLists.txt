cmake_minimum_required(VERSION 3.0.0)

#set (CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/CMake")
#include(cotire)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "17")

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")
set( CMAKE_CXX_FLAGS " -pthread -ldl -lstdc++fs" )

add_executable(SystemMonitor ${SOURCES})
target_compile_options(SystemMonitor PRIVATE "-Wall")

target_compile_features(SystemMonitor PRIVATE cxx_std_17)

target_link_libraries(SystemMonitor   
${CURSES_LIBRARIES}
${CMAKE_DL_LIBS} 
)

#cotire(SystemMonitor)

