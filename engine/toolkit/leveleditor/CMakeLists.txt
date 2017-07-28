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
# Inspectors directory
FILE(GLOB inspectors_h code/inspectors/*.h)
FILE(GLOB inspectors_cpp code/inspectors/*cpp)
SET(files_inspectors ${inspectors_h} ${inspectors_cpp})
SOURCE_GROUP("inspectors" FILES ${files_inspectors})

#------------------------------
ADD_EXECUTABLE(leveleditor ${files_base} ${files_interfaces} ${files_inspectors})
TARGET_LINK_LIBRARIES(leveleditor core render json)
ADD_DEPENDENCIES(leveleditor core render json)

SET_TARGET_PROPERTIES(leveleditor PROPERTIES FOLDER "vortex/toolkit")