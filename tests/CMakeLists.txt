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
	GTest::gtest
	GTest::gtest_main
)
target_include_directories(tests_main
	PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}
)

set (RESOURCES
)

configure_file(Environment.h.in ${CMAKE_CURRENT_BINARY_DIR}/Environment.h @ONLY)
configure_file(testconfig.ini.in ${CMAKE_CURRENT_BINARY_DIR}/testconfig.ini @ONLY)

foreach(resource ${RESOURCES})
    configure_file(${resource} ${CMAKE_CURRENT_BINARY_DIR}/${resource} COPYONLY)
endforeach()
