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
            "-Winit-self"
            "-Wstrict-aliasing")
endif()

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()
