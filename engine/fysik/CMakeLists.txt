#--------------------------------------------------------------------------
# fysik
#--------------------------------------------------------------------------

SET(files_fysik_fysik
	physicsserver.h
	physicsserver.cpp
	physicsdevice.h
	physicsdevice.cpp 
	basecollider.h
	basecollider.cpp
	surfacecollider.h
	surfacecollider.cpp
	boxcollider.h
	boxcollider.cpp
    rigidbody.h 
	rigidbody.cpp
	rigidbodyproperty.h
	rigidbodyproperty.cpp
	colliderproperty.h
	colliderproperty.cpp
	)

SOURCE_GROUP("fysik" FILES ${files_fysik})

SET(files_fysik
	${files_fysik_fysik}
	)

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(fysik STATIC ${files_fysik} ${files_pch})
TARGET_PCH(fysik ../)	
ADD_DEPENDENCIES(fysik application core foundation glew glfw misc)
TARGET_LINK_LIBRARIES(fysik PUBLIC application core foundation exts glew glfw ${OPENGL_LIBS} misc)
