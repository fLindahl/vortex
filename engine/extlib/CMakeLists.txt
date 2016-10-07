#--------------------------------------------------------------------------
# extlibs
# These should be moved to the external libaries!
#--------------------------------------------------------------------------

SET(files_extlib
	stb_image.h
	tinyxml2.h
	tinyxml2.cpp
	${files_extlib_pch}
	)
	
SOURCE_GROUP("extlib" FILES ${files_extlib})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(extlib STATIC ${files_extlib} ${files_pch})
TARGET_PCH(extlib ../)	
ADD_DEPENDENCIES(extlib glew glfw)
TARGET_LINK_LIBRARIES(extlib PUBLIC engine exts glew glfw ${OPENGL_LIBS})