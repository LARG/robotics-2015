cmake_minimum_required(VERSION 2.8)

project(vision)

include(../common.cmake)

set(SRCS ${INTERFACE_DIR}/shared_vision/main.cpp)

qi_create_bin(vision ${SRCS})
qi_use_lib(vision core opencv2_core opencv2_highgui opencv2_flann opencv2_features2d opencv2_calib3d opencv2_imgproc opencv2_objdetect)
target_link_libraries(vision ${LINK_LIBS} ${LIBYAML-CPP} ${LIBPYTHONSWIG} ${LIBCORE} ${LIBFFT})
