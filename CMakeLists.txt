set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()
message("-- Build type: ${CMAKE_BUILD_TYPE}")

option(rpcz_build_tests "Build rpcz's tests." OFF)
option(rpcz_build_static "Build static library." ON)
option(rpcz_build_examples "Build rpcz's examples." OFF)

find_package(ProtobufPlugin REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread program_options system)
find_package(ZeroMQ REQUIRED)

include_directories(${ZeroMQ_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/src)

cmake_minimum_required (VERSION 2.8.0)
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Werror")
set(CMAKE_OSX_ARCHITECTURES x86_64)

add_subdirectory(src)

if(rpcz_build_tests)
  enable_testing()
  add_subdirectory(test)
endif(rpcz_build_tests)

if(rpcz_build_examples)
  add_subdirectory(examples/cpp)
endif(rpcz_build_examples)

file(GLOB RPCZ_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/rpcz/*.h*")
if(rpcz_install_headers)
	install(FILES ${RPCZ_PUBLIC_HEADERS} DESTINATION include/rpcz)
endif(rpcz_install_headers)
