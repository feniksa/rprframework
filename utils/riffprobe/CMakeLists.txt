set(HEADERS
)
set(SOURCES
	riffprobe.cpp
)	

add_executable(riff_probe ${HEADERS} ${SOURCES} ${VERSION_RC})
target_link_libraries(riff_probe
	riff
	Boost::log
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
)

add_custom_command(TARGET riff_probe POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:riff_probe> $<TARGET_RUNTIME_DLLS:riff_probe>
  COMMAND_EXPAND_LISTS
)
populate_rprsdk_kernels(riff_probe)

