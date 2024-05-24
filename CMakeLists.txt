cmake_minimum_required(VERSION 3.20)
project(VisionDemos)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ASAN "enable asan/ubsan")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wvla -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unknown-pragmas")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	if (ASAN)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined")
	endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

add_executable(vision Image.cpp
                      lib/imgui-docking/backends/imgui_impl_glfw.cpp
                      lib/imgui-docking/backends/imgui_impl_opengl3.cpp
                      lib/imgui-docking/imgui.cpp
                      lib/imgui-docking/imgui_draw.cpp
                      lib/imgui-docking/imgui_tables.cpp
                      lib/imgui-docking/imgui_widgets.cpp
                      lib/imgui-docking/misc/cpp/imgui_stdlib.cpp
                      lib/imnodes-master/imnodes.cpp
                      MRU.cpp
                      NodeList.cpp
                      nodes/Node.cpp
                      nodes/NodeAdd.cpp
                      nodes/NodeAnd.cpp
                      nodes/NodeCameraStream.cpp
                      nodes/NodeCanny.cpp
                      nodes/NodeContours.cpp
                      nodes/NodeConvolve.cpp
                      nodes/NodeCrop.cpp
                      nodes/NodeDilate.cpp
                      nodes/NodeDistanceTransform.cpp
                      nodes/NodeErode.cpp
                      nodes/NodeGaussianBlur.cpp
                      nodes/NodeHistogram.cpp
                      nodes/NodeImageLoad.cpp
                      nodes/NodeInRange.cpp
                      nodes/NodeMultiply.cpp
                      nodes/NodeNormalize.cpp
                      nodes/NodeOutput.cpp
                      nodes/NodeConvertColor.cpp
                      nodes/NodeResize.cpp
                      nodes/NodeSubtract.cpp
                      nodes/NodeThreshold.cpp
                      Pin.cpp
                      VisionDemos.cpp)

target_include_directories(vision PUBLIC "${CMAKE_SOURCE_DIR}/lib/imgui-docking/"
                                         "${CMAKE_SOURCE_DIR}/lib/imgui-docking/backends/"
                                         "${CMAKE_SOURCE_DIR}/lib/json-develop/single_include/nlohmann/"
                                         "${CMAKE_SOURCE_DIR}/lib/imnodes-master/") 

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET opencv4 glfw3 opengl)
target_link_libraries(vision PUBLIC PkgConfig::deps)
