#--------------------------------------------------------------------------
# engine
#--------------------------------------------------------------------------

ADD_LIBRARY(engine INTERFACE)
TARGET_INCLUDE_DIRECTORIES(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(engine INTERFACE ${OPENGL_LIBS})
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(render)
ADD_SUBDIRECTORY(foundation)
ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(IO)
ADD_SUBDIRECTORY(physics)
ADD_SUBDIRECTORY(netcode)
ADD_SUBDIRECTORY(toolkit)

SET_TARGET_PROPERTIES(core PROPERTIES FOLDER "vortex/engine")
SET_TARGET_PROPERTIES(render PROPERTIES FOLDER "vortex/engine")
SET_TARGET_PROPERTIES(foundation PROPERTIES FOLDER "vortex/engine")
SET_TARGET_PROPERTIES(application PROPERTIES FOLDER "vortex/engine")
SET_TARGET_PROPERTIES(IO PROPERTIES FOLDER "vortex/engine")
SET_TARGET_PROPERTIES(netcode PROPERTIES FOLDER "vortex/engine")
SET_TARGET_PROPERTIES(physics PROPERTIES FOLDER "vortex/engine")
