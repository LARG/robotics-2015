cmake_minimum_required(VERSION 2.8)

project(core)

include(../common.cmake)
include_directories(${BHWALK2013_DIR} ${BHWALK2011_DIR} ${RSWALK2014_DIR})

SET(OPERATIONS_H ${NAO_HOME}/core/memory/MemoryBlockOperations.h)
SET(OPERATIONS_PY ${NAO_HOME}/build/core/generate_block_operations.py)
ADD_CUSTOM_COMMAND(
  OUTPUT ${OPERATIONS_H}
  COMMAND ${OPERATIONS_PY} ${OPERATIONS_H}
  DEPENDS ${OPERATIONS_PY}
)
ADD_CUSTOM_TARGET(operations DEPENDS ${OPERATIONS_H})

set(IGNORE_REGEXS 
  "python/PythonInterface.*"
  "lua/*"
  ".*backup.*"
  ".*bak.*"
  "${BHWALK2011_DIR}/*"
  "${BHWALK2013_DIR}/*"
  "${RSWALK2014_DIR}/*"
)

# collect all the files into a temp var
file(GLOB_RECURSE TEMP_SRCS 
  ${SRC_DIR}/*.cpp
  ${SRC_DIR}/*.h
)

SET(SRCS ${SWIG_SRCS})

# remove files from the excluded dirs and fill out the SRCS var
FOREACH(item ${TEMP_SRCS})
  set(excluded False)
  FOREACH(ignoreRegex ${IGNORE_REGEXS})
    STRING(REGEX MATCH ${ignoreRegex} excludedTemp ${item})
    IF (excludedTemp)
      set(excluded True)
      #message("IGNORE_REGEX: " ${ignoreRegex} " ignoring " ${item})
    ENDIF(excludedTemp)
  ENDFOREACH(ignoreRegex ${IGNORE_REGEXS})
  IF(NOT excluded)
    list(APPEND SRCS ${item})
  ENDIF(NOT excluded)
ENDFOREACH(item ${TEMP_SRCS})

qi_create_lib(core STATIC ${SRCS})
ADD_DEPENDENCIES(core operations)
qi_stage_lib(core)
