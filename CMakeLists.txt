project(p2pnet)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

### Common files preparation
file(GLOB_RECURSE COMMON_SRCS "src/common/*")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Protobuf Generator
find_package(Protobuf REQUIRED)

### p2pnetd
file(GLOB_RECURSE DAEMON_SRCS "src/daemon/*")
SET(DAEMON_ALL_SRCS ${COMMON_SRCS} ${DAEMON_SRCS})
if(PROTOBUF_FOUND)
  file(GLOB_RECURSE PROTO_DAEMON_LIST "src/daemon/*.proto")
  PROTOBUF_GENERATE_CPP(PROTO_DAEMON_SOURCES PROTO_DAEMON_HEADERS ${PROTO_DAEMON_LIST})
  list(APPEND DAEMON_ALL_SRCS ${PROTO_DAEMON_SOURCES})
  list(APPEND DAEMON_ALL_SRCS ${PROTO_DAEMON_HEADERS})
endif()

SET(P2PNET_EXECUTABLE_NAME "${PROJECT_NAME}d")
add_executable(${P2PNET_EXECUTABLE_NAME} ${DAEMON_ALL_SRCS})

### libp2pnet
file(GLOB_RECURSE LIBRARY_SRCS "src/library/*")
SET(LIBRARY_ALL_SRCS ${COMMON_SRCS} DAEMON_SRCS)

SET(P2PNET_LIBRARY_NAME "lib${PROJECT_NAME}")

### Third-party Libraries

# Boost
set (Boost_COMPONENTS
	system
	filesystem
	date_time)

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS ${Boost_COMPONENTS} REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(${P2PNET_EXECUTABLE_NAME} ${Boost_LIBRARIES})
endif()
# /Boost

# Botan
find_package(Botan REQUIRED)

if(BOTAN_FOUND)
  include_directories(${BOTAN_INCLUDE_DIRS})
  target_link_libraries(${P2PNET_EXECUTABLE_NAME} ${BOTAN_LIBRARIES})
endif()
# /Botan

# Protobuf
# Found lately

if(PROTOBUF_FOUND)
  include_directories(${PROTOBUF_INCLUDE_DIRS})
  target_link_libraries(${P2PNET_EXECUTABLE_NAME} ${PROTOBUF_LIBRARIES})
endif()
# /Protobuf
