##
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 目标名
SET(TARGET_NAME "zbar_qrdecode")

# 头文件路径添加
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlib/include)

# 库路径添加
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlib/lib)

# 源码文件列表添加
#AUX_SOURCE_DIRECTORY(. BIN_SRC_LIST)
SET(BIN_SRC_LIST ${TARGET_NAME}.c)

# 执行文件生成
ADD_EXECUTABLE(${TARGET_NAME} ${BIN_SRC_LIST})

target_link_libraries(${TARGET_NAME} -lMagickWand-6.Q16 -lzbar -liconv -lrt)

# Note
# lMagickWand-6.Q16 静态库链接是报错