#--------------------------------------------------------------------------
# physics
#--------------------------------------------------------------------------

SET(files_physics_physics
	physicsserver.h
	physicsserver.cpp
	physicsdevice.h
	physicsdevice.cpp 
	basecollider.h
	basecollider.cpp
	convexhullcollider.h
	convexhullcollider.cpp
	boxcollider.h
	boxcollider.cpp
    rigidbody.h 
	rigidbody.cpp
	rigidbodyproperty.h
	rigidbodyproperty.cpp
	colliderproperty.h
	colliderproperty.cpp
	bulletvortexconversion.h
	)

SOURCE_GROUP("physics" FILES ${files_physics})

SET(files_physics
	${files_physics_physics}
	)


INCLUDE_DIRECTORIES(${vortex_engine_SOURCE_DIR}/exts/bullet/src)

	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})

ADD_LIBRARY(physics STATIC ${files_physics} ${files_pch})
TARGET_PCH(physics ../)	
ADD_DEPENDENCIES(physics application core foundation glew glfw misc BulletDynamics BulletCollision LinearMath)
TARGET_LINK_LIBRARIES(physics PUBLIC application core foundation exts glew glfw ${OPENGL_LIBS} misc BulletDynamics BulletCollision LinearMath)
