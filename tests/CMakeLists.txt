find_package(GTest REQUIRED)

set(HEADERS
	StreamRedirect.h
	CommonData.h
	Utils.h
)

set(SOURCES
	StreamRedirect.cpp
	CommonData.cpp
	Utils.cpp
)

set (TESTS
	Dummy
	Renderer
	Northstar
	HybridPro
	Riff
	Utils
)

foreach(test ${TESTS})
	add_test(
			NAME ${test}
			COMMAND tests_main --gtest_filter=Test${test}.*
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		)
		set (TEST_SOURCES ${TEST_SOURCES} Test${test}.cpp)
endforeach()

add_executable(tests_main ${HEADERS} ${SOURCES} ${TEST_SOURCES})
target_link_libraries(tests_main 
	rprf
	riff
	GTest::gtest
	GTest::gtest_main
)
target_include_directories(tests_main
	PRIVATE
	"${CMAKE_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
)

configure_file(Environment.h.in ${CMAKE_CURRENT_BINARY_DIR}/Environment.h @ONLY)
configure_file(testconfig.ini.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.ini @ONLY)

# automatically copy all dll's deps.
add_custom_command(TARGET tests_main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:tests_main> $<TARGET_RUNTIME_DLLS:tests_main>
  COMMAND_EXPAND_LISTS
)

# copy hip kernels
populate_rprsdk_kernels(tests_main)

file(GLOB RESOURCES_textures resources/textures/*)
foreach(resource ${RESOURCES_textures})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resource}" "${CMAKE_CURRENT_BINARY_DIR}/resources/textures/${file_name}"
                COMMENT "Copying resource file: ${resource}")    
endforeach()

file(GLOB RESOURCES_meshes resources/meshes/*)
foreach(resource ${RESOURCES_meshes})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resource}" "${CMAKE_CURRENT_BINARY_DIR}/resources/meshes/${file_name}"
                COMMENT "Copying resource file: ${resource}")    
endforeach()

file(GLOB RESOURCES_images resources/images/*)
foreach(resource ${RESOURCES_images})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
			TARGET tests_main POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resource}" "${CMAKE_CURRENT_BINARY_DIR}/resources/images/${file_name}"
			COMMENT "Copying resource file: ${resource}")
endforeach()
