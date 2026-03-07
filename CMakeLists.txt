cmake_minimum_required(VERSION 4.2)
project(Comsion)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release/")

set(CMAKE_CXX_FLAGS -fPIC)

set(CMAKE_AUTOMOC on)

find_package(OpenCV 4.13.0 REQUIRED)
find_package(Qt6 6.10.2 REQUIRED COMPONENTS Widgets Gui)

add_executable(Comsion
	main.cpp
	Core/Image.cpp
	Window/Dialog.cpp
)
target_include_directories(Comsion PRIVATE ${CMAKE_SOURCE_DIR})
target_precompile_headers(Comsion PRIVATE pch.h)
target_include_directories(Comsion PRIVATE /usr/include/opencv4)
target_link_libraries(Comsion PRIVATE
	libopencv_core.so
	libopencv_imgcodecs.so
	libopencv_highgui.so
)
target_include_directories(Comsion PRIVATE /usr/include/qt6)
target_link_libraries(Comsion PRIVATE
	libQt6Core.so
	libQt6Widgets.so
	libQt6Gui.so
)