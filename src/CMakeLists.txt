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
	MaterialNodeType.h
)

set(SOURCES
	Camera.cpp
	ContextUtils.cpp
	Context.cpp
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
)

add_library(rprf STATIC ${SOURCES} ${HEADERS})
target_include_directories(rprf PUBLIC "${CMAKE_SOURCE_DIR}" Boost::beast Boost::log)
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
