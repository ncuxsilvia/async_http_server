project(server)

include(cxx11)
include(enumfiles)

find_package(Threads)

set(TARGET server)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

enumfiles(HDRS "${SERVER_SOURCE_PATH}" ".h;.hpp;.hxx")
enumfiles(SRCS "${SERVER_SOURCE_PATH}" ".cpp")

add_executable(
  ${TARGET}
    ${HDRS}
    ${SRCS}
)

target_link_libraries(${TARGET} ${Boost_LIBRARIES})
#target_link_libraries(${TARGET} pthread)

enable_cxx11_support(${TARGET})

