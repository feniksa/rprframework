find_package(GTest REQUIRED)

set(HEADERS
	StreamRedirector.h
	CommonData.h
    Utils.h)

set(SOURCES
	CommonData.cpp
	Utils.cpp)

set (TESTS
	Dummy
	Renderer
	Northstar
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
	#configure_file(${dll} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_CURRENT_BINARY_DIR}/${file_name}"
                COMMENT "Copying rpr dll's: ${dll}")    
endforeach()

foreach(dll ${RadeonImageFilter_DLLS})
	#configure_file(${dll} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET tests_main POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_CURRENT_BINARY_DIR}/${file_name}"
                COMMENT "Copying rif dll's: ${dll}")    
endforeach()



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
