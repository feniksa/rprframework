add_executable(rpr_probe rprprobe.cpp CubeData.cpp)
target_link_libraries(rpr_probe
	rprf
	Boost::log
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
)

add_executable(riff_probe riffprobe.cpp)
target_link_libraries(riff_probe
	riff
	Boost::log
	Boost::system
	Boost::filesystem
	Boost::thread
	Boost::program_options
)

foreach(dll ${RadeonProRender_DLLS})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET rpr_probe POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_CURRENT_BINARY_DIR}/${file_name}"
                COMMENT "Copying rpr dll's: ${dll}")
endforeach()

foreach(dll ${RadeonImageFilter_DLLS})
	get_filename_component(file_name "${resource}" NAME)
	add_custom_command(
                TARGET riff_probe POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_CURRENT_BINARY_DIR}/${file_name}"
                COMMENT "Copying rif dll's: ${dll}")
endforeach()

file(GLOB hipkernels "${RadeonProRenderSDK_HIP_KERNELS_DIRS}/*")
foreach(hipkernel ${hipkernels})
	get_filename_component(file_name "${hipkernel}" NAME)
	add_custom_command(
	       TARGET rpr_probe POST_BUILD
	       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hipkernel}" "${CMAKE_CURRENT_BINARY_DIR}/hipbin/${file_name}"
	       COMMENT "Copying hip kernel: ${hipkernel}")
endforeach()



