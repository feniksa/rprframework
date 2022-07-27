cmake_minimum_required (VERSION 3.18)
project (rprframework CXX)

#option(HTTP_SERVER "Enable build of http server" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(
		"-fno-elide-constructors"
                "-Wall"
 		# fix for math/matrix.h ugly anonymous union
 		#"-pedantic-errors"
		"-Wextra"
                "-Winit-self"
                "-Wold-style-cast"
                "-Woverloaded-virtual"
                "-Wuninitialized"
                "-Wmissing-declarations"
                "-Winit-self")
 endif()

add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.79 REQUIRED COMPONENTS thread system filesystem
	program_options log)
find_package(RadeonProRenderSDK REQUIRED COMPONENTS northstar tahoe)

add_subdirectory(math)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
