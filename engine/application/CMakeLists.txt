#--------------------------------------------------------------------------
# application
#--------------------------------------------------------------------------

SET(files_application_basegamefeature
	basegamefeature/managers/entitymanager.h
	basegamefeature/managers/entitymanager.cpp
	basegamefeature/managers/scenemanager.h
	basegamefeature/managers/scenemanager.cpp
	basegamefeature/managers/envmanager.h
	basegamefeature/managers/envmanager.cpp	
	basegamefeature/keyhandler.h
	basegamefeature/keyhandler.cpp
	basegamefeature/iniparser.h
	)

SET(files_application_game
	game/entity.h
	game/entity.cpp
	game/entitymessages.h
	game/entitymessages.cpp
	game/modelentity.h
	game/modelentity.cpp
	game/baseproperty.h
	game/baseproperty.cpp
	game/rigidbodyentity.h
	game/staticentity.h
	game/rigidbodyentity.cpp
	game/physicsentity.h
	game/staticentity.cpp 
	game/physicsentity.cpp
	game/cubemapentity.h
	game/cubemapentity.cpp
	game/modelentityWithSpotlight.h
	game/modelentityWithSpotlight.cpp
	game/particleentity.h
	game/particleentity.cpp
	game/staticentity.cpp 
	game/physicsentity.cpp
	game/geometryproxyentity.h
	game/geometryproxyentity.cpp
	game/pointlightentity.cpp 
	game/pointlightentity.h
	game/attribute.h
	game/attribute.cpp
	)
	
SET(files_application_properties
	properties/freecamera.h
	properties/freecamera.cpp
	properties/particleemitter.h
	properties/particleemitter.cpp
	)
	
	
SOURCE_GROUP("basegamefeature" FILES ${files_application_basegamefeature})

SOURCE_GROUP("game" FILES ${files_application_game})

SOURCE_GROUP("properties" FILES ${files_application_properties})

SET(files_application
	${files_application_basegamefeature}
	${files_application_game}
	${files_application_properties}
	)

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(application STATIC ${files_application} ${files_pch})
TARGET_PCH(application ../)	
ADD_DEPENDENCIES(application core render glew misc)
TARGET_LINK_LIBRARIES(application PUBLIC engine core foundation render exts glew misc)
