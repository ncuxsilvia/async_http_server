project(parser-test)

include(enumfiles)

set(TARGET parser-test)

enumfiles(HDRS "${PROJECT_SOURCE_DIR}" ".h;.hpp;.hxx")
enumfiles(SRCS "${PROJECT_SOURCE_DIR}" ".cpp")

include_directories(${SOURCE_INCLUDE})

add_executable(
  ${TARGET}
    ${HDRS}
    ${SRCS}
)

target_link_libraries(
  ${TARGET}
    -lstdc++
    http-header
)

enable_cxx11_support(${TARGET})
