cmake_minimum_required(VERSION 3.0.0)

find_package(Curses REQUIRED)
find_package(Boost COMPONENTS regex system filesystem REQUIRED)
set(Boost_USE_STATIC_LIBS   ON)

include_directories(${CURSES_INCLUDE_DIR})

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")
set( CMAKE_CXX_FLAGS " -pthread -ldl " )

add_executable(SystemMonitor ${SOURCES})
target_compile_options(SystemMonitor PRIVATE "-w")


target_link_libraries(SystemMonitor   
${Boost_LIBRARIES}
${CURSES_LIBRARIES}
${CMAKE_DL_LIBS} 
)


