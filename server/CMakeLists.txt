project(server)

include(cxx11)
include(enumfiles)

find_package(Threads)

set(TARGET server)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

enumfiles(HDRS "${PROJECT_SOURCE_DIR}" ".h;.hpp;.hxx")
enumfiles(SRCS "${PROJECT_SOURCE_DIR}" ".cpp")

include_directories (${SOURCE_INCLUDE})

add_executable(
  ${TARGET}
    ${HDRS}
    ${SRCS}
)

target_link_libraries(
    ${TARGET}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        -lstdc++
        http-header
)

#if(UNIX)
#  set_target_properties(
#    ${TARGET}
#    PROPERTIES
#      COMPILE_FLAGS "-std=c++11"
#  )
#endif()

enable_cxx11_support(${TARGET})

