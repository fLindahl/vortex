#--------------------------------------------------------------------------
# Level Editor
#--------------------------------------------------------------------------

PROJECT(leveleditor)

#------------------------------
# Base directory
FILE(GLOB leveleditor_headers code/*.h)
FILE(GLOB leveleditor_sources code/*.cpp)
SET(files_base ${leveleditor_headers} ${leveleditor_sources})
SOURCE_GROUP("base" FILES ${files_base})

#------------------------------
# Interfaces directory
FILE(GLOB interface_h code/interfaces/*.h)
FILE(GLOB interface_cpp code/interfaces/*cpp)
SET(files_interfaces ${interface_h} ${interface_cpp})
SOURCE_GROUP("interfaces" FILES ${files_interfaces})



#------------------------------
ADD_EXECUTABLE(leveleditor ${files_base} ${files_interfaces})
TARGET_LINK_LIBRARIES(leveleditor core render)
ADD_DEPENDENCIES(leveleditor core render)

SET_TARGET_PROPERTIES(leveleditor PROPERTIES FOLDER "vortex/toolkit")