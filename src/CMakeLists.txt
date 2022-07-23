set(HEADERS
	Renderer.h
)

set(SOURCES
	Renderer.cpp
)

add_library(rprf STATIC ${SOURCES} ${HEADERS})
target_include_directories(rprf PRIVATE Boost::beast Boost::log)
target_link_libraries(rprf PRIVATE 
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
	Boost::log
)

add_executable(rpr ${SOURCES} ${HEADERS} main.cpp) 
target_link_libraries(rpr rprf)

