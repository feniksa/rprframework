set(HEADERS
	CubeData.h
)
set(SOURCES
	CubeData.cpp
	rprprobe.cpp
)

add_executable(rpr_probe ${HEADERS} ${SOURCES} ${VERSION_RC})
target_link_libraries(rpr_probe
	rprf
	Boost::log
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
)

add_custom_command(TARGET rpr_probe POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:rpr_probe> $<TARGET_RUNTIME_DLLS:rpr_probe>
  COMMAND_EXPAND_LISTS
)
populate_rprsdk_kernels(rpr_probe)

