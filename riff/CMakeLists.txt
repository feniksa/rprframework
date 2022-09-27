set(HEADERS
	Error.h
	Context.h
	ContextObject.h
	ContextUtils.h
	RadeonImageFilterEnums.h
	Image.h ContextQueue.cpp ContextQueue.h ImageFilter.cpp ImageFilter.h ImageFilterType.h ImageDescription.h MemoryMap.cpp MemoryMap.h)

set(SOURCES
	Error.cpp
	Context.cpp
	ContextUtils.cpp
	Image.cpp ContextQueue.cpp ContextQueue.h ImageFilter.cpp ImageFilter.h MemoryMap.cpp MemoryMap.h)

add_library(riff STATIC ${SOURCES} ${HEADERS})
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
