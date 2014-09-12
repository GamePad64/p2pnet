project(p2pnet)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

### Setting compiler properties
file(GLOB_RECURSE COMMON_SRCS "src/common/*")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
# Use -flto flag to enable GCC's link-time optimization.
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -use-gold-plugin")
	SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -use-gold-plugin")
	SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} -use-gold-plugin")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
# Use -flto flag to enable GCC's link-time optimization.
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
	SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
	SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} -flto")
ENDIF()

# Protobuf Generator
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
  file(GLOB_RECURSE PROTO_COMMON_LIST "src/common/*.proto")
  PROTOBUF_GENERATE_CPP(PROTO_COMMON_SOURCES PROTO_COMMON_HEADERS ${PROTO_COMMON_LIST})
  list(APPEND COMMON_SRCS ${PROTO_COMMON_SOURCES})
  list(APPEND COMMON_SRCS ${PROTO_COMMON_HEADERS})
endif()

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
SET(LIBRARY_ALL_SRCS ${COMMON_SRCS} ${LIBRARY_SRCS})
list(APPEND LIBRARY_ALL_SRCS "src/p2pnet.cpp")
list(APPEND LIBRARY_ALL_SRCS "src/p2pnet.h")

SET(P2PNET_LIBRARY_NAME "${PROJECT_NAME}")
add_library(${P2PNET_LIBRARY_NAME} SHARED ${LIBRARY_ALL_SRCS})
set_target_properties(${P2PNET_LIBRARY_NAME} PROPERTIES COMPILE_DEFINITIONS "BUILDING_LIBP2PNET")
set_target_properties(${P2PNET_LIBRARY_NAME} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden -ffunction-sections")

#============================================================================
# Third-party Libraries
#============================================================================

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

#============================================================================
# Documentation generation
#============================================================================

#-- Add an Option to toggle the generation of the API documentation
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  #-- Configure the Template Doxyfile for our specific project
  configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
  #-- Add a custom target to run Doxygen when ever the project is built
  add_custom_target (Docs ALL COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
  # IF you do NOT want the documentation to be generated EVERY time you build the project
  # then leave out the 'ALL' keyword from the above command.
endif()
