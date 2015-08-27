cmake_minimum_required(VERSION 2.8)

project(core)

include(../common.cmake)
include(luaswig.cmake)

foreach(item ${SWIG_SRCS})
  list(APPEND SWIG_HEADERS "${item}.h")
  ADD_CUSTOM_COMMAND(
    OUTPUT ${item}.h
    COMMAND touch "${item}.h"
    DEPENDS ${item}
  )
endforeach(item ${SWIG_SRCS})

#qi_create_lib(swig STATIC ${SWIG_SRCS})
#qi_stage_lib(swig)
add_library(luaswig STATIC ${SWIG_HEADERS})
set_target_properties(luaswig PROPERTIES LINKER_LANGUAGE CXX)
