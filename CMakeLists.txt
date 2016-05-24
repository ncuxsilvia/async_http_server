cmake_minimum_required(VERSION 2.8.12)

set(
  CMAKE_USER_MAKE_RULES_OVERRIDE
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-overrides/c_flag.cmake
)

set(
  CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-overrides/cxx_flag.cmake
)

project(async_http_server)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS thread REQUIRED)

set(
  CMAKE_ARCHIVE_OUTPUT_DIRECTORY  
    ${PROJECTS_BINARY_DIR}/lib
  CACHE
    INTERNAL "Directory where all .lib|.a files are dumped."
)

set(
  CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib
  CACHE
    PATH "Directory where all .so files are dumped (if necessary)"
)

set(
  CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
  CACHE
    PATH "Directory where all executable files are dumped."
)

if(NOT Boost_FOUND)
    message(SEND_ERROR "Failed to find boost::thread.")
  return()
else()
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Boost found.")
endif()

message("CMAKE_BINARY_DIR: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_executable(main main.cpp)
