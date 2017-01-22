#--------------------------------------------------------------------------
# Physics
#--------------------------------------------------------------------------

SET(files_physics_server
	server/physicsserver.h
	server/physicsserver.cpp
	server/physicsdevice.h
	server/physicsdevice.cpp
	)

SET(files_physics_dynamics
	dynamics/rigidbody.h
	dynamics/rigidbody.cpp
	)
	
SOURCE_GROUP("server" FILES ${files_physics_server})
SOURCE_GROUP("dynamics" FILES ${files_physics_dynamics})

SET(files_physics
	${files_physics_server}
	${files_physics_dynamics}
	)

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(physics STATIC ${files_physics} ${files_pch})
TARGET_PCH(physics ../)	
ADD_DEPENDENCIES(physics application foundation glew glfw misc BulletCollision BulletDynamics LinearMath)
TARGET_LINK_LIBRARIES(physics PUBLIC application foundation exts glew glfw ${OPENGL_LIBS} misc BulletCollision BulletDynamics LinearMath)
