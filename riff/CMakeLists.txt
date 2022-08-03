set(HEADERS
	Error.h
	Context.h
	ContextObject.h
	ContextUtils.h
	RadeonImageFilterEnums.h
)

set(SOURCES
	Error.cpp
	Context.cpp
	ContextUtils.cpp
)

add_library(riff STATIC ${SOURCES} ${HEADERS})
target_include_directories(riff PUBLIC "${CMAKE_SOURCE_DIR}")
target_link_libraries(riff PUBLIC
		Boost::system
		Boost::filesystem
		Boost::thread
		Boost::program_options
		Boost::log
		RadeonImageFilter::RIF
)
