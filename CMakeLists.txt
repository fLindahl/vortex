#--------------------------------------------------------------------------
# VORTEX ENGINE
#--------------------------------------------------------------------------
PROJECT(vortex_engine)
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

MACRO(TARGET_PCH target path)
IF(WIN32)
	IF(MSVC)
		SET_TARGET_PROPERTIES(${target}
			PROPERTIES 
			COMPILE_FLAGS "/Yuconfig.h /Fp${CMAKE_CFG_INTDIR}/${target}.pch")
		SET_SOURCE_FILES_PROPERTIES(${path}config.cc
            PROPERTIES
            COMPILE_FLAGS "/Ycconfig.h")
	ENDIF()
ENDIF()
ENDMACRO(TARGET_PCH)

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -mavx -std=c++11")
ENDIF()

IF(MSVC)
    SET(OPENGL_LIBS opengl32.lib)
ELSE()
    SET(OPENGL_LIBS GL GLU X11 Xxf86vm pthread Xrandr Xi Xinerama Xcursor)
ENDIF()

SET(VORTEX_ENGINE_ROOT ${CMAKE_CURRENT_DIR})

SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS GLEW_STATIC)
ADD_SUBDIRECTORY(exts)
ADD_SUBDIRECTORY(engine)
ADD_SUBDIRECTORY(projects)

