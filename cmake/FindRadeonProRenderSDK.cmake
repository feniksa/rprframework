find_path(RPR_INCLUDE_DIR 
	NAME RadeonProRender.h
	HINTS /usr /usr/local/include 
	ENV RPR_SDK_ROOT
    	PATH_SUFFIXES RadeonProRender/inc
)

find_library(RPR_LIBRARY 
	NAMES
	RadeonProRender64
	HINTS /usr/lib64 /usr/loca/lib64 
	ENV RPR_SDK_ROOT
	PATH_SUFFIXES RadeonProRender/binUbuntu18 
	              RadeonProRender/libWin64
)

message(STATUS ${RPR_LIBRARY})

macro(process_rpr_version)
	if(RPR_INCLUDE_DIR)
		file(READ "${RPR_INCLUDE_DIR}/RadeonProRender.h" HEADER_CONTENT)
		
		string(REGEX MATCHALL "#define RPR_VERSION_MAJOR ([0-9]+)" VERSION_MAJOR_STR "${HEADER_CONTENT}")
		set(RPR_VERSION_MAJOR ${CMAKE_MATCH_1})

		string(REGEX MATCHALL "#define RPR_VERSION_MINOR ([0-9]+)" VERSION_MINOR_STR "${HEADER_CONTENT}")
		set(RPR_VERSION_MINOR ${CMAKE_MATCH_1})

		string(REGEX MATCHALL "#define RPR_VERSION_REVISION ([0-9]+)" REVISION_MINOR_STR "${HEADER_CONTENT}")
		set(RPR_VERSION_REVISION ${CMAKE_MATCH_1})

		unset(HEADER_CONTENT)
	endif()
	set(RPR_VERSION_STRING "${RPR_VERSION_MAJOR}.${RPR_VERSION_MINOR}.${RPR_VERSION_REVISION}")
endmacro()

macro(find_tahoe)
	find_library(RPR_TAHOE_LIBRARY 
		NAMES
		Tahoe64
		HINTS /usr/lib64 /usr/loca/lib64
		ENV RPR_SDK_ROOT
		PATH_SUFFIXES RadeonProRender/binUbuntu18
	     		      RadeonProRender/libWin64
	)

	if(NOT TARGET RadeonProRenderSDK::tahoe)
    	add_library(RadeonProRenderSDK::tahoe INTERFACE IMPORTED)		
    	set_target_properties(RadeonProRenderSDK::tahoe PROPERTIES
			#INTERFACE_INCLUDE_DIRECTORIES "${RPR_INCLUDE_DIR}"
			INTERFACE_LINK_LIBRARIES "${RPR_TAHOE_LIBRARY}")
    endif()
endmacro()

macro(find_northstar)
	find_library(RPR_NORTHSTAR_LIBRARY 
		NAMES
		Northstar64
		HINTS /usr/lib64 /usr/loca/lib64
		ENV RPR_SDK_ROOT
		PATH_SUFFIXES RadeonProRender/binUbuntu18
	     		      RadeonProRender/libWin64

	)

	if(NOT TARGET RadeonProRenderSDK::northstar)
    	add_library(RadeonProRenderSDK::northstar INTERFACE IMPORTED)		
    	set_target_properties(RadeonProRenderSDK::northstar PROPERTIES
			INTERFACE_LINK_LIBRARIES "${RPR_NORTHSTAR_LIBRARY}")
    endif()
endmacro()
macro(find_hybrid)
	find_library(RPR_HYBRID_LIBRARY 
		NAMES
		Hybrid
		HINTS /usr/lib64 /usr/loca/lib64
		ENV RPR_SDK_ROOT
		PATH_SUFFIXES RadeonProRender/binUbuntu18
	     		      RadeonProRender/libWin64
	)

	if(NOT TARGET RadeonProRenderSDK::hybrid)
    	add_library(RadeonProRenderSDK::hybrid INTERFACE IMPORTED)		
    	set_target_properties(RadeonProRenderSDK::hybrid PROPERTIES
			INTERFACE_LINK_LIBRARIES "${RPR_HYBRID_LIBRARY}")
    endif()
endmacro()

process_rpr_version()


foreach(component IN LISTS RadeonProRenderSDK_FIND_COMPONENTS)
	string(TOUPPER "${component}" component_upcase)

	if (component STREQUAL "tahoe")
		find_tahoe()
	elseif(component STREQUAL "northstar")
		find_northstar()
	elseif(component STREQUAL "hybrid")
		find_hybrid()
	else()
		message(WARNING "${component} is not a valid RadeonProRender component")
		set(RadeonProRenderSDK_${component}_FOUND false)
	endif()

	if(TARGET RadeonProRenderSDK::${component})
 		set(RadeonProRenderSDK_${component}_FOUND TRUE)
	else()
    	set(RadeonProRenderSDK_${component}_FOUND FALSE)
	endif()

endforeach()
unset(component)

find_package_handle_standard_args(RadeonProRenderSDK
	REQUIRED_VARS RPR_LIBRARY RPR_INCLUDE_DIR
    VERSION_VAR RPR_VERSION_STRING
    HANDLE_COMPONENTS)

if(RadeonProRenderSDK_FOUND)
	set(RadeonProRenderSDK_INCLUDE_DIRS ${RPR_INCLUDE_DIR})
	set(RadeonProRenderSDK_LIBRARIES ${RPR_LIBRARY})	
	
	if(NOT TARGET RadeonProRenderSDK::RPR)
		add_library(RadeonProRenderSDK::RPR INTERFACE IMPORTED)
		set_target_properties(RadeonProRenderSDK::RPR PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${RPR_INCLUDE_DIR}"
			INTERFACE_LINK_LIBRARIES "${RPR_LIBRARY}")
	endif()
endif()
