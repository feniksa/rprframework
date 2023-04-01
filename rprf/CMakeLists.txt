
set(HEADERS
	CameraType.h
	Camera.h
	Context.h
	ContextUtils.h
	ContextObject.h
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
)

set(SOURCES
	Camera.cpp
	ContextUtils.cpp
	Context.cpp
    ContextObject.cpp
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
)

add_library(rprf STATIC ${SOURCES} ${HEADERS})
target_include_directories(rprf PUBLIC "${CMAKE_SOURCE_DIR}" Boost::log)
target_link_libraries(rprf PUBLIC 
	rprf_math
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
	Boost::log
	RadeonProRenderSDK::RPR
)

add_executable(rpr ${SOURCES} ${HEADERS} main.cpp) 
target_link_libraries(rpr rprf)

if(FORCE_UPDATE_ENUMS)
	add_custom_command(		
		OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/RadeonProRenderEnums.h" 
		#TARGET rprf PRE_BUILD
		COMMAND python "${CMAKE_SOURCE_DIR}/utils/convert.py"
		ARGS -i "${RPR_INCLUDE_DIR}/RadeonProRender.h" -o "${CMAKE_CURRENT_SOURCE_DIR}/RadeonProRenderEnums.h"
		DEPENDS "${RPR_INCLUDE_DIR}/RadeonProRender.h" "${CMAKE_SOURCE_DIR}/utils/convert.py"
	)
endif()

