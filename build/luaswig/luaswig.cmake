cmake_minimum_required(VERSION 2.8)

set(SWIG_LIST_FILE "${SRC_DIR}/lua/SwigFiles.txt")
set(SWIG_CPP_DIR "${NAO_HOME}/build/build/swig/luaswig/cpp")
set(SWIG_I_DIR "${NAO_HOME}/build/build/swig/luaswig/i")
file(READ ${SWIG_LIST_FILE} SWIG_ORIG_FILES_STR)
string(REPLACE "\n" ";" SWIG_ORIG_FILES ${SWIG_ORIG_FILES_STR})
foreach(header ${SWIG_ORIG_FILES})
  list(APPEND HEADERS "${NAO_HOME}/core/${header}")
endforeach(header ${SWIG_ORIG_FILES})


set(GENERATE_SWIG ${CMAKE_SOURCE_DIR}/../luaswig/generateSwig.py)

EXECUTE_PROCESS(
  COMMAND ${GENERATE_SWIG} --getSourceNames ${SWIG_LIST_FILE}
  OUTPUT_VARIABLE SWIG_NAMES_STR
)
string(REPLACE "\n" ";" SWIG_NAMES ${SWIG_NAMES_STR})

MAKE_DIRECTORY(${SWIG_CPP_DIR})
MAKE_DIRECTORY(${SWIG_I_DIR})

MACRO(STRING_TRIM var str)
  SET(${var} "")
  IF(${var} STREQUAL "")
    SET(_var_1 "${str}")
    STRING(REGEX REPLACE  "^[ \t\r\n]+" "" _var_2 "${_var_1}" )
    STRING(REGEX REPLACE  "[ \t\r\n]+$" "" _var_3 "${_var_2}" )
    SET(${var} "${_var_3}")
  ENDIF(${var} STREQUAL "")
ENDMACRO(STRING_TRIM var str)

foreach(item ${SWIG_ORIG_FILES})
  STRING_TRIM(item ${item}) 
  string(REPLACE "/" "_" SWIG_NAME ${item})
  string(REPLACE ".h" "" SWIG_NAME ${SWIG_NAME})
  string(REPLACE ".i" "" SWIG_NAME ${SWIG_NAME})
  set(I_FILE ${SWIG_I_DIR}/lua_${SWIG_NAME}.i)
  list(APPEND I_FILES ${I_FILE})
endforeach(item ${SWIG_ORIG_FILES})

foreach(item ${SWIG_ORIG_FILES})
  STRING_TRIM(item ${item}) 

  string(REPLACE "/" "_" SWIG_NAME ${item})
  string(REPLACE ".h" "" SWIG_NAME ${SWIG_NAME})
  string(REPLACE ".i" "" SWIG_NAME ${SWIG_NAME})

  set(I_FILE ${SWIG_I_DIR}/lua_${SWIG_NAME}.i)
  set(CPP_FILE ${SWIG_CPP_DIR}/lua_${SWIG_NAME}_wrap.cpp)
  list(APPEND SWIG_SRCS ${CPP_FILE})

  ADD_CUSTOM_COMMAND(
    OUTPUT ${I_FILE}
    COMMAND ${GENERATE_SWIG} --generateI ${SWIG_NAME} ${NAO_HOME}/core/${item} ${I_FILE} ${SWIG_NAMES} 
    DEPENDS ${SWIG_LIST_FILE} ${GENERATE_SWIG} ${HEADERS}
  )
  
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CPP_FILE}
    COMMAND ${GENERATE_SWIG} --generateCpp ${SWIG_NAME} ${I_FILE} ${CPP_FILE}
    IMPLICIT_DEPENDS CXX ${I_FILE}
    DEPENDS ${SWIG_LIST_FILE} ${I_FILES} ${GENERATE_SWIG} ${HEADERS}
  )
endforeach(item ${SWIG_ORIG_FILES})

list(APPEND SWIG_TEMPLATES "Vector2<float>")
list(APPEND SWIG_TEMPLATES "Vector3<float>")
list(APPEND SWIG_TEMPLATES "std::vector<float>")
list(APPEND SWIG_TEMPLATES "std::vector<double>")
list(APPEND SWIG_TEMPLATES "std::vector<int>")

foreach(item ${SWIG_TEMPLATES})
  string(REPLACE "<" "_" SWIG_NAME ${item})
  string(REPLACE ">" "" SWIG_NAME ${SWIG_NAME})
  string(REPLACE "std::" "" SWIG_NAME ${SWIG_NAME})
  string(REPLACE "V" "v" SWIG_NAME ${SWIG_NAME})
  list(APPEND SWIG_TEMPLATE_NAMES ${SWIG_NAME})

  set(I_FILE ${SWIG_I_DIR}/lua_${SWIG_NAME}.i)
  set(CPP_FILE ${SWIG_CPP_DIR}/lua_${SWIG_NAME}_wrap.cpp)
  list(APPEND SWIG_SRCS ${CPP_FILE})
  
  ADD_CUSTOM_COMMAND(
    OUTPUT ${I_FILE}
    COMMAND ${GENERATE_SWIG} --generateTemplateI ${SWIG_NAME} \"${item}\" ${I_FILE}
    DEPENDS ${SWIG_LIST_FILE} ${GENERATE_SWIG}
  )

  ADD_CUSTOM_COMMAND(
    OUTPUT ${CPP_FILE}
    COMMAND ${GENERATE_SWIG} --generateCpp ${SWIG_NAME} ${I_FILE} ${CPP_FILE}
    IMPLICIT_DEPENDS CXX ${I_FILE}
    DEPENDS ${SWIG_LIST_FILE} ${GENERATE_SWIG}
  )
endforeach(item ${SWIG_TEMPLATES})

ADD_CUSTOM_COMMAND(
  OUTPUT ${SWIG_CPP_DIR}/openLuaLibs.cpp
  COMMAND ${GENERATE_SWIG} --generateOpenLuaLibs ${SWIG_CPP_DIR}/openLuaLibs.cpp ${SWIG_NAMES} ${SWIG_TEMPLATE_NAMES}
  DEPENDS ${GENERATE_SWIG}
)
list(APPEND SWIG_SRCS "${SWIG_CPP_DIR}/openLuaLibs.cpp")
