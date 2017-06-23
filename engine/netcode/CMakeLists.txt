#--------------------------------------------------------------------------
# netcode
#--------------------------------------------------------------------------


IF(WIN32)
	SET(files_netcode_OS
		windows/winsockethandler.h
		windows/winsockethandler.cpp
		)
ELSE(WIN32)
	SET(files_netcode_OS
		linux/linuxsockethandler.h
		linux/linuxsockethandler.cpp
		)
ENDIF(WIN32)

SET(files_netcode_global
	constants.h
	${files_netcode_OS}
)

SOURCE_GROUP("netcode" FILES ${files_netcode_global})

SET(files_netcode
	${files_netcode_global}
	)

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(netcode STATIC ${files_netcode} ${files_pch})
TARGET_PCH(netcode ../)	
ADD_DEPENDENCIES(netcode application core foundation glew glfw misc)
TARGET_LINK_LIBRARIES(netcode PUBLIC application core foundation exts glew glfw ${OPENGL_LIBS} misc)
