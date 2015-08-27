cmake_minimum_required(VERSION 2.8)

project(motion)

include(../common.cmake)

set(SRCS ${INTERFACE_DIR}/shared_motion/main.cpp)

qi_create_bin(motion ${SRCS})
qi_use_lib(motion core opencv2_core opencv2_highgui)
target_link_libraries(motion ${LIBCORE} ${LIBYAML-CPP} ${WALK_LIBS} ${ALGLIB} ${LINK_LIBS})
