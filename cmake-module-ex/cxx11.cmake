include(CheckCXXCompilerFlag)

function(enable_cxx11_support _target)
  if(UNIX)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" __compiler_supports_cxx11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" __compiler_supports_cxx0x)
    if(__compiler_supports_cxx11)
      target_compile_options(
        ${_target}
        PRIVATE
          "-std=c++11"
      )
    elseif(__compiler_supports_cxx0x)
      target_compile_options(
        ${_target}
        PRIVATE
          "-std=c++0x"
      )
    else()
      message(
        FATAL_ERROR
          "The compiler \"${CMAKE_CXX_COMPILER}\" has no C++11 support. Please use a different C++ compiler."
      )
    endif()
  endif()
endfunction(enable_cxx11_support)
