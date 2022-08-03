find_path(RIF_INCLUDE_DIR 
	NAME RadeonImageFilters.h
	HINTS /usr /usr/local/include 
	"${RIF_SDK_ROOT}"
	ENV RIF_SDK_ROOT
    PATH_SUFFIXES include
)

find_library(RIF_LIBRARY 
	NAMES
	RadeonImageFilters
	HINTS /usr/lib64 /usr/loca/lib64 
	"${RIF_SDK_ROOT}"
	ENV RIF_SDK_ROOT
	PATH_SUFFIXES 	Ubuntu20/Dynamic/
			Windows/Dynamic-MT
)

set(RadeonImageFilter_DLLS "")
if (WIN32)
	find_file(RadeonImageFilter_rif_dll 
		NAMES "RadeonImageFilters.dll"
		HINTS
		"${RIF_SDK_ROOT}"
		ENV RIF_SDK_ROOT
		PATH_SUFFIXES Windows/Dynamic-MT)

	if (RadeonImageFilter_rif_dll)
		list(APPEND RadeonImageFilter_DLLS ${RadeonImageFilter_rif_dll})
	endif()

	find_file(RadeonImageFilter_rif_ml_dll
			NAMES "RadeonML.dll"
			HINTS
			"${RIF_SDK_ROOT}"
			ENV RIF_SDK_ROOT
			PATH_SUFFIXES Windows/Dynamic-MT)

	if (RadeonImageFilter_rif_ml_dll)
		list(APPEND RadeonImageFilter_DLLS ${RadeonImageFilter_rif_ml_dll})
	endif()
endif()

function(process_rif_version)
	if(RIF_INCLUDE_DIR)
		file(READ "${RIF_INCLUDE_DIR}/RadeonImageFilters_version.h" HEADER_CONTENT)
		
		string(REGEX MATCHALL "#define RIF_VERSION_MAJOR[ \t\r\n]+([0-9]+)" VERSION_MAJOR_STR "${HEADER_CONTENT}")
		set(RIF_VERSION_MAJOR ${CMAKE_MATCH_1})

		string(REGEX MATCHALL "#define RIF_VERSION_MINOR[ \t\r\n]+([0-9]+)" VERSION_MINOR_STR "${HEADER_CONTENT}")
		set(RIF_VERSION_MINOR ${CMAKE_MATCH_1})

		string(REGEX MATCHALL "#define RIF_VERSION_REVISION[ \t\r\n]+([0-9]+)" REVISION_MINOR_STR "${HEADER_CONTENT}")
		set(RIF_VERSION_REVISION ${CMAKE_MATCH_1})

		unset(HEADER_CONTENT)
	endif()
	set(RIF_VERSION_STRING "${RIF_VERSION_MAJOR}.${RIF_VERSION_MINOR}.${RIF_VERSION_REVISION}" PARENT_SCOPE)
endfunction()

process_rif_version()

find_package_handle_standard_args(RadeonImageFilter
	REQUIRED_VARS RIF_LIBRARY RIF_INCLUDE_DIR
    VERSION_VAR RIF_VERSION_STRING
)

if(RadeonImageFilter_FOUND)
	set(RadeonImageFilter_INCLUDE_DIRS ${RIF_INCLUDE_DIR})
	set(RadeonImageFilter_LIBRARIES ${RIF_LIBRARY})	

	if(NOT TARGET RadeonImageFillter::RIF)
		add_library(RadeonImageFilter::RIF INTERFACE IMPORTED)
		set_target_properties(RadeonImageFilter::RIF PROPERTIES
				IMPORTED_LINK_INTERFACE_LANGUAGES "C"
				INTERFACE_LINK_LIBRARIES "${RIF_LIBRARY}"
				INTERFACE_INCLUDE_DIRECTORIES "${RIF_INCLUDE_DIR}"
				)
	endif()
endif()
