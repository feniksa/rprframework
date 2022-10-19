cmake_minimum_required (VERSION 3.18)
project (rprframework CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
option(FORCE_UPDATE_ENUMS "Update Contants.h file" FALSE)

include("${CMAKE_SOURCE_DIR}/cmake/CompileOptions.cmake")

#add_definitions(-DBOOST_LOG_DYN_LINK)
SET(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.79 REQUIRED COMPONENTS thread system filesystem
	program_options log)
find_package(RadeonProRenderSDK REQUIRED COMPONENTS northstar tahoe)
find_package(RadeonImageFilter REQUIRED)

add_subdirectory(ThirdParty/miniz)
add_subdirectory(tinyexrimpl)
add_subdirectory(stbimpl)
add_subdirectory(math)
add_subdirectory(rprf)
add_subdirectory(riff)
add_subdirectory(utils)

enable_testing()
add_subdirectory(tests)
