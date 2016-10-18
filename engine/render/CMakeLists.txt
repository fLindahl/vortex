#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_render
	window.h
	window.cc
	shadersemantics.h
	)

SET(files_render_graphics
	graphics/camera.h
	graphics/camera.cpp
	)
	
SET(files_render_animation
	animation/animationclip.h
	animation/animationclip.cpp
	animation/animationparser.h
	animation/animationparser.cpp
	animation/animationresource.h
	animation/animationresource.cpp
	animation/animcurve.h
	animation/animkeybuffer.h
	animation/naxfileformatstructs.h
	animation/nvx2fileformatstructs.h
	)

SET(files_render_character
	character/characterjoint.h
	character/characterjoint.cpp
	character/characterskeleton.h
	character/characterskeleton.cpp
	character/skinnedcharacter.h
	character/skinnedcharacter.cpp	
	)
	
SET(files_render_properties
	properties/graphicsproperty.h
	properties/graphicsproperty.cpp
	)

SET(files_render_resources
	resources/lightnode.h
	resources/lightnode.cpp
	resources/meshresource.h
	resources/meshresource.cpp
		resources/shaderobject.h
		resources/shaderobject.cpp
	resources/textureresource.h
	resources/textureresource.cpp
	resources/vertexcomponent.h
	resources/modelinstance.h
	resources/modelinstance.cpp
		)

SET(files_render_server
	server/graphicsserver.h
	server/graphicsserver.cpp
	server/shaderserver.h
	server/shaderserver.cpp
	server/resourceserver.h
	server/resourceserver.cpp
	server/shaderconstants.h
        server/renderdevice.h server/renderdevice.cpp)
	
SET(files_render_softwarerender
	softwarerender/rasterizer.h
	softwarerender/rasterizer.cpp
	)	
	
SOURCE_GROUP("render" FILES ${files_render_render})

SOURCE_GROUP("graphics" FILES ${files_render_graphics})

SOURCE_GROUP("animation" FILES ${files_render_animation})

SOURCE_GROUP("character" FILES ${files_render_character})

SOURCE_GROUP("properties" FILES ${files_render_properties})

SOURCE_GROUP("resources" FILES ${files_render_resources})

SOURCE_GROUP("server" FILES ${files_render_server})

SOURCE_GROUP("softwarerender" FILES ${files_render_softwarerender})


SET(files_render
	${files_render_render}
	${files_render_graphics}
	${files_render_animation}
	${files_render_character}
	${files_render_properties}
	${files_render_resources}
	${files_render_server}
	${files_render_softwarerender}
	)

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw misc)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw ${OPENGL_LIBS} misc)
