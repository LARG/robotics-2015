cmake_minimum_required(VERSION 2.8)

PROJECT(pythonswig)
INCLUDE(../common.cmake)

SET(PYTHONSWIG_DIR ${CMAKE_SOURCE_DIR}/../pythonswig)
SET(SWIG_LIST_FILE "${SRC_DIR}/python/SwigFiles.txt")
FILE(READ ${SWIG_LIST_FILE} SWIG_ORIG_FILES_STR)
STRING(REPLACE "\n" ";" SWIG_ORIG_FILES ${SWIG_ORIG_FILES_STR})
FOREACH(header ${SWIG_ORIG_FILES})
  LIST(APPEND HEADERS "${NAO_HOME}/core/${header}")
ENDFOREACH(header ${SWIG_ORIG_FILES})

SET(GENERATE_SWIG ${PYTHONSWIG_DIR}/generateSwig.py)
SET(SWIG_CPP_DIR "${NAO_HOME}/build/build/swig/pythonswig/cpp")
set(SWIG_I_DIR "${NAO_HOME}/build/build/swig/pythonswig/i")
MAKE_DIRECTORY(${SWIG_CPP_DIR})
MAKE_DIRECTORY(${SWIG_I_DIR})

SET(CPP_FILE ${SWIG_CPP_DIR}/pythonswig_module.cpp)
SET(I_FILE ${SWIG_I_DIR}/pythonswig_module.i) 
ADD_CUSTOM_COMMAND(
  OUTPUT ${CPP_FILE}
  COMMAND ${GENERATE_SWIG} ${SWIG_LIST_FILE} ${NAO_HOME}/core ${I_FILE} ${CPP_FILE}
  DEPENDS ${SWIG_LIST_FILE} ${GENERATE_SWIG} ${HEADERS}
)
ADD_CUSTOM_TARGET(pythonswig_module_wrap DEPENDS ${CPP_FILE})

INCLUDE_DIRECTORIES(${PYTHONSWIG_DIR} ${PYTHON_INCLUDE})
ADD_LIBRARY(_pythonswig_module SHARED ${CPP_FILE} ${PYTHONSWIG_DIR}/PythonInterface.cpp)
TARGET_LINK_LIBRARIES(_pythonswig_module ${NAOQI_LIB}/libpython2.7.a)
ADD_DEPENDENCIES(_pythonswig_module pythonswig_module_wrap)

EXECUTE_PROCESS(
  COMMAND ln -sf ${CMAKE_CURRENT_BINARY_DIR}/lib_pythonswig_module.so ${NAO_HOME}/build/pythonswig/_pythonswig_module.so
)
EXECUTE_PROCESS(
  COMMAND ln -sf ${NAO_HOME}/build/build/swig/pythonswig/cpp/pythonswig_module.py ${NAO_HOME}/build/pythonswig/pythonswig_module.py
)
