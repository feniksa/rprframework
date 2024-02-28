add_subdirectory(math)

set(HEADERS
	CastContainer.hpp
	CameraType.h
	Camera.h
	Context.h
	ContextUtils.h
	ContextObject.h
	GlobalFunctions.h
	Object.h
	Error.h
	Renderer.h
	Plugin.h
	Scene.h
	Shape.h
	LightDirectional.h  
	LightEnvironment.h  
	Light.h  
	LightPoint.h  
	LightSky.h  
	LightSpot.h
	FrameBuffer.h
	FrameBufferSysmem.h
	MaterialSystem.h
	MaterialNode.h
	MaterialNodeInput.h
	Image.h
	RadeonProRenderEnums.h
	Color4.h
)

set(SOURCES
	Camera.cpp
	ContextUtils.cpp
	ContextObject.cpp
	Context.cpp
	GlobalFunctions.cpp
	Error.cpp
	Object.cpp
	Renderer.cpp
	Plugin.cpp
	Scene.cpp
	Shape.cpp
	Light.cpp  
	LightDirectional.cpp  
	LightEnvironment.cpp  
	LightPoint.cpp
	LightSky.cpp  
	LightSpot.cpp
	FrameBuffer.cpp
	FrameBufferSysmem.cpp
	MaterialSystem.cpp
	MaterialNode.cpp
	MaterialNodeInput.cpp
	Image.cpp
	Color4.cpp
)

add_library(rprf SHARED ${SOURCES} ${HEADERS} ${VERSION_RC})
if(RPRF_EXPORT_API)
	target_compile_definitions(rprf PRIVATE RPRF_EXPORT_API)
endif()

target_include_directories(rprf PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(rprf PUBLIC 
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
	Boost::log
	RadeonProRenderSDK::RPR
	rprf_math
	stbimpl
)

file(GLOB PUBLIC_HEADERS "*.h")
set_target_properties(rprf PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

if(FORCE_UPDATE_ENUMS)
	add_custom_command(		
		OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/RadeonProRenderEnums.h" 
		#TARGET rprf PRE_BUILD
		COMMAND python "${CMAKE_SOURCE_DIR}/utils/convert.py"
		ARGS -i "${RPR_INCLUDE_DIR}/RadeonProRender.h" -o "${CMAKE_CURRENT_SOURCE_DIR}/RadeonProRenderEnums.h"
		DEPENDS "${RPR_INCLUDE_DIR}/RadeonProRender.h" "${CMAKE_SOURCE_DIR}/utils/convert.py"
	)
endif()
include(GNUInstallDirs)
install(TARGETS rprf RUNTIME PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rprf)

