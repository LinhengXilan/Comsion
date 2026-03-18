cmake_minimum_required(VERSION 4.2)
project(Comsion)

set(CMAKE_CXX_STANDARD 20)

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release/")

# 设置编译选项
set(CMAKE_CXX_FLAGS -fPIC)
set(CMAKE_AUTOMOC on)

# 查询包
find_package(OpenCV 4.13 REQUIRED)
find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets Gui)

# 项目1：主程序
add_executable(Comsion
	Core/main.cpp
	Core/Image.cpp
	Core/Processor.cpp
	Window/Dialog.cpp
)
# 项目1：主程序.编译
target_include_directories(Comsion PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(Comsion PRIVATE /usr/include/opencv4)
target_include_directories(Comsion PRIVATE /usr/include/qt6)
target_precompile_headers(Comsion PRIVATE Core/pch.h)
# 项目1：主程序.链接
target_link_libraries(Comsion PRIVATE
	libopencv_core.so
	libopencv_imgcodecs.so
	libopencv_highgui.so
	libopencv_imgproc.so
)
target_link_libraries(Comsion PRIVATE
	libQt6Core.so
	libQt6Widgets.so
	libQt6Gui.so
)
if(CMAKE_BUILD_TYPE EQUAL Debug)
	target_compile_definitions(Comsion PRIVATE
		DEBUG
	)
endif()