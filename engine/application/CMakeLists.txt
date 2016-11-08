#--------------------------------------------------------------------------
# application
#--------------------------------------------------------------------------

SET(files_application_basegamefeature
	basegamefeature/managers/entitymanager.h
	basegamefeature/managers/entitymanager.cpp
	basegamefeature/keyhandler.h
	basegamefeature/keyhandler.cpp
	)

SET(files_application_game
	game/entity.h
	game/entity.cpp
	)
	
SET(files_application_properties
	properties/freecamera.h
	properties/freecamera.cpp
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
ADD_DEPENDENCIES(application glew misc)
TARGET_LINK_LIBRARIES(application PUBLIC engine foundation exts glew misc)
