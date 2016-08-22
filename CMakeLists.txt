cmake_minimum_required(VERSION 3.6)
project(RealTimeAnimation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lab001/Camera.h
    lab001/Cone.cpp
    lab001/Cone.h
    lab001/Cube.cpp
    lab001/Cube.hpp
    lab001/CubeMap.cpp
    lab001/CubeMap.hpp
    lab001/FormController.h
    lab001/Geometry.cpp
    lab001/Geometry.hpp
    lab001/main.cpp
    lab001/Mesh.h
    lab001/model.h
    lab001/Plane.h
    lab001/Shader.cpp
    lab001/Shader.hpp
    lab001/ShaderManager.cpp
    lab001/ShaderManager.hpp lab001/WaterFrameBuffers.cpp lab001/WaterFrameBuffers.h)

add_executable(RealTimeAnimation ${SOURCE_FILES})

IF(APPLE)
    INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(GLUT_LIBRARY GLUT )
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
    MARK_AS_ADVANCED (COCOA_LIBRARY
            GLUT_LIBRARY
            OpenGL_LIBRARY)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)
target_link_libraries(RealTimeAnimation ${EXTRA_LIBS})
target_link_libraries(RealTimeAnimation glfw3 assimp SOIL glew AntTweakBar)