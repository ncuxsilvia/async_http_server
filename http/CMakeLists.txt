project(http)

include(cxx11)
include(enumfiles)

set(TARGET http)

enumfiles(HDRS "${PROJECT_SOURCE_DIR}" ".h;.hpp;.hxx")
enumfiles(SRCS "${PROJECT_SOURCE_DIR}" ".cpp")

add_library(
  ${TARGET} STATIC
    ${HDRS}
    ${SRCS}
)

if(UNIX)
  set_target_properties(
    ${TARGET}
    PROPERTIES
      COMPILE_FLAGS "-fPIC"
  )
endif()

enable_cxx11_support(${TARGET})
