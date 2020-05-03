cmake_minimum_required(VERSION 3.12)
project(project_360_visual)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/project_360_visual.cpp
    ${CMAKE_SOURCE_DIR}/src/projection.cpp)
set(INCLUDE_FILES
    ${CMAKE_SOURCE_DIR}/include/project_360_visual.h
    ${CMAKE_SOURCE_DIR}/include/projection.h)

#Creating executable targets
add_executable(project_360_visual ${SOURCE_FILES} ${INCLUDE_FILES})
set_target_properties(project_360_visual PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

message("Output directory : ")
message("${RUNTIME_OUTPUT_DIRECTORY}")

#Importing shared libraries, copying and linking with the executable
set(SHARED_LIBS_NAME 
		opencv_tracking430 
		opencv_highgui430
		opencv_videoio430
		opencv_imgproc430
		opencv_core430
		opencv_video430
		opencv_dnn430
		opencv_imgcodecs430
		opencv_calib3d430
		opencv_features2d430
		opencv_flann430)

foreach(SHARED_LIB_NAME ${SHARED_LIBS_NAME})

	#Creating library targets
	add_library(${SHARED_LIB_NAME} SHARED IMPORTED GLOBAL)
	set_target_properties(${SHARED_LIB_NAME} PROPERTIES
		IMPORTED_LOCATION "c:/opencv/build/bin/Release/${SHARED_LIB_NAME}.dll"
		IMPORTED_IMPLIB "c:/opencv/build/lib/Release/${SHARED_LIB_NAME}.lib"
	)

	#CopyingDLLs
	add_custom_command(TARGET project_360_visual POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			c:/opencv/build/bin/Release/${SHARED_LIB_NAME}.dll
			$<TARGET_FILE_DIR:project_360_visual>)  

	#Linking with target executable
	target_link_libraries(project_360_visual PUBLIC ${SHARED_LIB_NAME})

endforeach()

target_include_directories(project_360_visual PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(project_360_visual PUBLIC ${OpenCV_LIBS})