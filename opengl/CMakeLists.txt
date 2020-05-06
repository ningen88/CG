cmake_minimum_required(VERSION 3.16)
project(TutorialOpenGL)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
	set(DEPENDENCIES ${CMAKE_SOURCE_DIR}/dependecies)
	set(VENDOR ${CMAKE_SOURCE_DIR}/vendor)
	set(RESOURCE ${CMAKE_SOURCE_DIR}/res)
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
	
	add_library(glew STATIC IMPORTED)
	add_library(glfw STATIC IMPORTED)
	set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${DEPENDENCIES}/GLEW/lib/Release/x64/glew32.lib)
	set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${DEPENDENCIES}/GLFW/lib-vc2019/glfw3.lib)
	
	add_executable(opengl-app ${SRC_FILES} ${VENDOR}/stb_image/stb_image.cpp)
	target_include_directories(opengl-app PUBLIC 
		${INCLUDE_DIR} ${VENDOR}/stb_image 
		${VENDOR}
		${DEPENDENCIES}/GLEW/include
		${DEPENDENCIES}/GLFW/include)
	target_link_libraries(opengl-app glew glfw)
else()
	message(STATUS "Only Windows supported")
endif(WIN32)