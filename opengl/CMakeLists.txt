cmake_minimum_required(VERSION 3.16)
project(TutorialOpenGL)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    find_package(OpenGL REQUIRED)
	set(DEPENDENCIES ${CMAKE_SOURCE_DIR}/dependecies)
	set(VENDOR ${CMAKE_SOURCE_DIR}/vendor)
	set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
	set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
	set(SRC_FILES 
		${SOURCE_DIR}/main.cpp
		${SOURCE_DIR}/indexBuffer.cpp  
		${SOURCE_DIR}/render.cpp  
		${SOURCE_DIR}/texture.cpp
		${SOURCE_DIR}/vertexBuffer.cpp
		${SOURCE_DIR}/shader.cpp 
		${SOURCE_DIR}/vertexArray.cpp
		)
	set(HEADER
		${INCLUDE_DIR}/indexBuffer.h
		${INCLUDE_DIR}/render.h
		${INCLUDE_DIR}/texture.h
		${INCLUDE_DIR}/vertexBuffer.h
		${INCLUDE_DIR}/shader.h 
		${INCLUDE_DIR}/vertexArray.h
		${INCLUDE_DIR}/vertexBufferLayout.h
		)
	file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/res/*")
	
	add_library(glew STATIC IMPORTED)
	add_library(glfw STATIC IMPORTED)
	set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${DEPENDENCIES}/GLEW/lib/Release/x64/glew32s.lib)
	set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${DEPENDENCIES}/GLFW/lib-vc2019/glfw3.lib)
	
	add_executable(opengl-app ${SRC_FILES} ${HEADER} ${VENDOR}/stb_image/stb_image.cpp ${RESOURCE_FILES})
	target_link_directories(opengl-app PUBLIC ${CMAKE_SOURCE_DIR}/dependecies/GLEW/lib/Release/x64 ${CMAKE_SOURCE_DIR}/dependecies/GLFW/lib-vc2019)
	target_include_directories(opengl-app PUBLIC 
		${INCLUDE_DIR} ${VENDOR}/stb_image 
		${VENDOR}
		${DEPENDENCIES}/GLEW/include
		${DEPENDENCIES}/GLFW/include
		${OPENGL_INCLUDE_DIRS})
	add_compile_definitions(RESOURCE_FOLDER="${CMAKE_SOURCE_DIR}/res/")
	source_group("Resource Files" FILES ${RESOURCE_FILEs})
	target_link_libraries(opengl-app ${OPENGL_LIBRARIES} glew glfw)
else()
	message(STATUS "Only Windows supported")
endif(WIN32)