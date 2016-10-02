#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_render
	window.h
	window.cc
	graphicsnode.h
	graphicsnode.cpp
	lightnode.h
	lightnode.cpp
	meshresource.h
	meshresource.cpp
	rasterizer.h
	rasterizer.cpp
	shaderobject.h
	shaderobject.cpp
	stb_image.h
	textureresource.h
	textureresource.cpp
	)
	
SOURCE_GROUP("render" FILES ${files_render_render})


SET(files_render
	${files_render_render}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw ${OPENGL_LIBS})