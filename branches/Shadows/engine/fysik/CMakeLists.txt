#--------------------------------------------------------------------------
# fysik
#--------------------------------------------------------------------------

SET(files_fysik_fysik
	physicsserver.h
	physicsdevice.h
	physicsserver.cpp
	basecollider.h
	surfacecollider.h
	surfacecollider.cpp
        rigidbody.h physicsdevice.cpp rigidbody.cpp)

SOURCE_GROUP("fysik" FILES ${files_fysik})

SET(files_fysik
	${files_fysik_fysik}
	)

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(fysik STATIC ${files_fysik} ${files_pch})
TARGET_PCH(fysik ../)	
ADD_DEPENDENCIES(fysik application foundation glew glfw misc)
TARGET_LINK_LIBRARIES(fysik PUBLIC application foundation exts glew glfw ${OPENGL_LIBS} misc)
