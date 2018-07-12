##
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## ָ��cmakeģ���·��
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

## ƽ̨�������������� (��Ҫ�޸�)
#INCLUDE(${CMAKE_MODULE_PATH}/fh-toolchain.cmake)
#INCLUDE(${CMAKE_MODULE_PATH}/vatics-m3c-toolchain.cmake)
#INCLUDE(${CMAKE_MODULE_PATH}/amba-toolchain.cmake)
#INCLUDE(${CMAKE_MODULE_PATH}/hisi-toolchain.cmake)

## ����ѡ��
SET(BUILD_DEBUG_TYPE ON)	# ON=debug, OFF=release
IF (BUILD_DEBUG_TYPE)
	SET(FLAGS_ARGS "-O0 -Wall -g -ggdb")
ELSE (BUILD_DEBUG_TYPE)
	SET(FLAGS_ARGS "-O3 -Wall")
ENDIF (BUILD_DEBUG_TYPE)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS_ARGS} -std=gnu99")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_ARGS} -std=c++11")

## ��̬��or��̬��
SET(BUILD_SHARED_LIBS OFF)	# ON=��̬��, OFF=��̬��

## �������·������ (�����޸�)
# ���·��(bin lib inc)
SET(COMMON_OUTPUT_PATH ${PROJECT_BINARY_DIR})	# ./build
#SET(COMMON_OUTPUT_PATH ${PROJECT_SOURCE_DIR})	# ./
# ִ���ļ����·��
SET(EXECUTABLE_OUTPUT_PATH ${COMMON_OUTPUT_PATH}/bin)
# ���ļ����·��
SET(LIBRARY_OUTPUT_PATH ${COMMON_OUTPUT_PATH}/lib)
