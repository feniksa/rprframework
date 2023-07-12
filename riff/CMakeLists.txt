set(HEADERS
	Error.h
	Context.h
	ContextObject.h
	ContextUtils.h
	RadeonImageFilterEnums.h
	Image.h
	ContextQueue.h
	ImageFilter.h ImageFilterType.h
	ImageDescription.h
	MemoryMap.h
)

set(SOURCES
	Error.cpp
	Context.cpp
	ContextUtils.cpp
	Image.cpp
	ContextQueue.cpp
	ImageFilter.cpp
	MemoryMap.cpp
	RadeonImageFilterEnums.cpp
)

add_library(riff STATIC ${SOURCES} ${HEADERS} ${VERSION_RC})
target_include_directories(riff PUBLIC "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/ThirdParty")
target_link_libraries(riff PUBLIC
		Boost::system
		Boost::filesystem
		Boost::thread
		Boost::program_options
		Boost::log
		RadeonImageFilter::RIF
		tinyexrimpl
		stbimpl
)
