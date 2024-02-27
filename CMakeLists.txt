cmake_minimum_required (VERSION 3.18)
project (rprframework VERSION 2.2.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
option(FORCE_UPDATE_ENUMS "Update Contants.h file" FALSE)
option(ENABLE_TESTING "Enable testing" TRUE)
option(BUILD_UTILS "Build utils" TRUE)

include("${CMAKE_SOURCE_DIR}/cmake/CompileOptions.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/WindowsFileVersion.cmake")

#add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.83 REQUIRED COMPONENTS thread system filesystem
	program_options log)
find_package(RadeonProRenderSDK 3.1.5 REQUIRED COMPONENTS northstar tahoe hybrid hybridpro)
find_package(RadeonImageFilter REQUIRED)

add_subdirectory(ThirdParty/miniz)
add_subdirectory(tinyexrimpl)
add_subdirectory(stbimpl)
add_subdirectory(math)
add_subdirectory(rprf)
add_subdirectory(riff)
if (BUILD_UTILS)
	add_subdirectory(utils)
endif()

if (ENABLE_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()
