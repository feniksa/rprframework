find_package(GTest REQUIRED)

set(HEADERS
	StreamRedirector.h
	CommonData.h
)
set(SOURCES
	CommonData.cpp
)

set (TESTS
	Dummy
	Renderer
	Northstar
	Riff
)

foreach(test ${TESTS})
	add_test(
			NAME ${test}
			COMMAND tests_main --gtest_filter=Test${test}.*
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		)
		set (TEST_SOURCES ${TEST_SOURCES} Test${test}.cpp)
endforeach()

add_executable(tests_main ${TEST_SOURCES} ${HEADERS} ${SOURCES})
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

foreach(dll ${RadeonProRender_DLLS})	
	configure_file(${dll} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_CURRENT_BINARY_DIR}/${file_name}"
                COMMENT "Copying rpr dll's: ${dll}")    
endforeach()

foreach(dll ${RadeonProRender_DLLS})
	configure_file(${dll} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_CURRENT_BINARY_DIR}/${file_name}"
                COMMENT "Copying rif dll's: ${dll}")    
endforeach()



file(GLOB RESOURCES_textures Resources/Textures/*)
foreach(resource ${RESOURCES_textures})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resource}" "${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/${file_name}"
                COMMENT "Copying resource file: ${resource}")    
endforeach()

file(GLOB RESOURCES_meshes Resources/Meshes/*)
foreach(resource ${RESOURCES_meshes})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resource}" "${CMAKE_CURRENT_BINARY_DIR}/Resources/Meshes/${file_name}"
                COMMENT "Copying resource file: ${resource}")    
endforeach()

