# DESCRIPTION:
#     Search for files with a given suffixes
#   in a directory `_path'
# USAGE:
#   enumfiles(
#     "${PATH}"
#     ".ex0;.ex1;ex2"
#     FILELIST
#   )
#   message("${FILELIST}")

function(enumfiles _output _path _suffixes)
  # travers through the list of suffixes
  foreach(suff ${_suffixes})
    # find files with a given suffix
    set(__filelist "__filelist-NOTFOUND")
    file(
      GLOB
        __filelist
        "${_path}/*${suff}"
    )
    if(__filelist)
      # update output file list
      list(
        APPEND
          ${_output}
          ${__filelist}
      )
    endif()
  endforeach()
  # publish function results...
  set(${_output} ${${_output}} PARENT_SCOPE)
endfunction()