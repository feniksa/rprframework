set(HEADERS tinyexrimpl.h)
set(SOURCES tinyexrimpl.cpp)

add_library(tinyexrimpl STATIC ${SOURCES} ${HEADERS})
target_include_directories(tinyexrimpl PUBLIC "${CMAKE_SOURCE_DIR}/ThirdParty" "${CMAKE_SOURCE_DIR}/ThirdParty/miniz" "${CMAKE_BINARY_DIR}/ThirdParty/miniz")
target_link_libraries(tinyexrimpl PUBLIC miniz)
