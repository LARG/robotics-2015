cmake_minimum_required(VERSION 2.8)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# You need this to find the QiBuild CMake framework
find_package(qibuild QUIET)
include(qibuild/general)


# some base variables
set(NAO_HOME $ENV{NAO_HOME})
set(CMAKE_C_COMPILER  ${NAO_HOME}/naoqi/crosstoolchain/atom/cross/bin/i686-aldebaran-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER  ${NAO_HOME}/naoqi/crosstoolchain/atom/cross/bin/i686-aldebaran-linux-gnu-g++)
set(SRC_DIR ${NAO_HOME}/core)
set(INTERFACE_DIR ${NAO_HOME}/interfaces)

set(BHWALK2011_DIR ${NAO_HOME}/core/motion/bhwalk2011)
set(LIBBHWALK2011 ${CMAKE_CURRENT_BINARY_DIR}/../../bhwalk2011/build-atom/bhwalk2011/libbhwalk2011.a)

set(BHWALK2013_DIR ${NAO_HOME}/core/motion/bhwalk2013)
set(LIBBHWALK2013 ${CMAKE_CURRENT_BINARY_DIR}/../../bhwalk2013/build-atom/bhwalk2013/libbhwalk2013.a)
set(LIBSNAPPY ${BHWALK2013_DIR}/Util/snappy/lib/linux_x86/libsnappy.a)

# Adding this for the runswift walk - Josiah
set(RSWALK2014_DIR ${NAO_HOME}/core/motion/rswalk2014)
set(LIBRSWALK2014 ${CMAKE_CURRENT_BINARY_DIR}/../../rswalk2014/build-atom/rswalk2014/librswalk2014.a)
set(LIBRSSOCCER ${CMAKE_CURRENT_BINARY_DIR}/../../rswalk2014/build-atom/rswalk2014/librssoccer.a)

set(WALK_LIBS ${LIBRSWALK2014} ${LIBRSSOCCER})
set(WALK_DIRS ${BHWALK2011_DIR} ${BHWALK2013_DIR} ${RSWALK2014_DIR})

set(LINK_LIBS -L${NAO_HOME}/naoqi/crosstoolchain/atom/boost/lib boost_thread-mt-1_55 boost_system-mt-1_55)
set(NAOQI_LINK_LIBS ${NAO_HOME}/naoqi/link_libs)
set(NAOQI_INCLUDES ${NAO_HOME}/naoqi/includes)
set(EIGEN_DIR ${NAOQI_INCLUDES}/eigen3)
set(LUA_INCLUDE ${NAO_HOME}/lib/lua/src)
set(LIBLUA ${NAO_HOME}/lib/lua/liblua.a)
set(YAML_INCLUDE ${NAO_HOME}/lib/yaml-cpp/include)
set(LIBYAML-CPP ${NAO_HOME}/lib/yaml-cpp/libyaml-cpp.a)
set(NAOQI_ROOT ${NAO_HOME}/naoqi/crosstoolchain/atom/sysroot)
set(NAOQI_LIB ${NAOQI_ROOT}/usr/lib)
set(NAOQI_CROSS_LIB ${NAO_HOME}/naoqi/crosstoolchain/atom/cross/i686-aldebaran-linux-gnu/sysroot/lib)
set(PYTHON_INCLUDE ${NAOQI_ROOT}/usr/include/python2.7)
set(LIBPYTHONSWIG ${NAO_HOME}/build/pythonswig/lib_pythonswig_module.so -lutil)
set(LIBCORE ${CMAKE_CURRENT_BINARY_DIR}/../../core/build-atom/sdk/lib/libcore.a)
SET(QT_DIR ${NAOQI_INCLUDES}/qwt-qt4)

SET(FFT_INCLUDE ${NAO_HOME}/lib/fft/include)
SET(LIBFFT ${NAO_HOME}/lib/fft/libfftw3.a)


if($ENV{USER} STREQUAL "sbarrett") # for lab machines
  set(OPENCV2_CORE_INCLUDE_DIRS /usr/include/opencv-2.3.1/opencv2)
  set(OPENCV2_HIGHGUI_INCLUDE_DIRS /usr/include/opencv-2.3.1/opencv2/highgui)
  set(OPENCV_DIR /usr/include/opencv-2.3.1)
endif($ENV{USER} STREQUAL "sbarrett") # for lab machines

# set C++ compiler flags
# TODO: Why do we need cache string force?
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=1" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y" CACHE STRING "" FORCE)
if(TOOL_BUILD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--dynamic-linker=${NAO_HOME}/naoqi/link_libs/ld-linux.so" CACHE STRING "" FORCE)
endif(TOOL_BUILD)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-enum-compare" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cpp" CACHE STRING "" FORCE)
set(CMAKE_SYSROOT ${NAO_HOME}/naoqi/crosstoolchain/atom/sysroot)

message("C Compiler: " ${CMAKE_C_COMPILER})
message("C++ Compiler: " ${CMAKE_CXX_COMPILER})

# set the include directory, the base dir, lua, bhuman's walk, and lib eigen
INCLUDE_DIRECTORIES(${SRC_DIR} ${LUA_INCLUDE} ${EIGEN_DIR} ${YAML_INCLUDE} ${OPENCV_DIR} ${PYTHON_INCLUDE} ${FFT_INCLUDE} ${NAO_HOME}/build/include ${NAO_HOME}/naoqi/include ${NAO_HOME}/naoqi/includes ${NAO_HOME}/naoqi/crosstoolchain/atom/zlib/include)

SET(ALGLIB_SRC ${NAO_HOME}/lib/alglib)
SET(ALGLIB ${ALGLIB_SRC}/alglib.a)
IF(NOT EXISTS ${ALGLIB})
  EXECUTE_PROCESS(COMMAND ${ALGLIB_SRC}/compile)
ENDIF()
SET(USER_DEFINE "USER_$ENV{USER}")
ADD_DEFINITIONS(-D"${USER_DEFINE}")
