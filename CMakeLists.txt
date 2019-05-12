cmake_minimum_required(VERSION 3.0.0)

#set (CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/CMake")
#include(cotire)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Curses REQUIRED)
find_package(Boost COMPONENTS regex system filesystem REQUIRED)
set(Boost_USE_STATIC_LIBS   ON)

include_directories(${CURSES_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")
set( CMAKE_CXX_FLAGS " -pthread -ldl " )

add_executable(SystemMonitor ${SOURCES})
target_compile_options(SystemMonitor PRIVATE "-Wall")


target_link_libraries(SystemMonitor   
${Boost_LIBRARIES}
${CURSES_LIBRARIES}
${CMAKE_DL_LIBS} 
)

#cotire(SystemMonitor)

