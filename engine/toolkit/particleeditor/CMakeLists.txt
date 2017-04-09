#--------------------------------------------------------------------------
# Particle Editor
# Maintained by Anton Grönberg
#--------------------------------------------------------------------------

PROJECT(particleeditor)
FILE(GLOB particleeditor_headers code/*.h code/tools/*.h)
FILE(GLOB particleeditor_sources code/*.cpp code/tools/*.cpp)

SET(files_particleeditor ${particleeditor_headers} ${particleeditor_sources} code/particleeditor.rc)
SOURCE_GROUP("particleeditor" FILES ${files_particleeditor})

ADD_EXECUTABLE(particleeditor ${files_particleeditor})
TARGET_LINK_LIBRARIES(particleeditor core render)
ADD_DEPENDENCIES(particleeditor core render)

SET_TARGET_PROPERTIES(particleeditor PROPERTIES FOLDER "vortex/toolkit")