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

set(
  CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake-module-ex
)

option(ENABLE_SERVER_TEST "Enable async_http_server test module" OFF)

find_package(Threads REQUIRED)

find_package(
  Boost 
    COMPONENTS 
      system
      program_options
      thread
  REQUIRED
)

include(add_module)

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

set(
  SERVER_SOURCE_PATH
    ${PROJECT_SOURCE_DIR}/source/server
)

#add_subdirectory(${SERVER_SOURCE_PATH})


add_module(
  NAMES
    source/server
  STATUS_SHORT_NAMES
)

add_module(
  CONDITION
    ENABLE_SERVER_TEST
  NAMES
    source/test
  STATUS_SHORT_NAMES
)
