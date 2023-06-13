find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)

set(HEADERS
	GLWidget.h
	Renderer.h
	MainWindow.h
)

set(MOC_HEADERS MainWindow.h GLWidget.h Renderer.h)
set(SOURCES
	main.cpp
	GLWidget.cpp
	Renderer.cpp
	MainWindow.cpp        
)

set(UIS MainWindow.ui)
set(RESOURCES main.qrc)

qt6_wrap_cpp(MOC_SRCS ${MOC_HEADERS})
qt6_wrap_ui(UI_HEADERS ${UIS})
qt6_add_resources(RES_RESOURCES ${RESOURCES})

add_executable(viewer ${SOURCES} ${HEADERS} ${UI_HEADERS} ${MOC_SRCS} ${RES_RESOURCES})
target_include_directories(viewer PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(viewer
	PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets rprf
)
