#--------------------------------------------------------------------------
# Level Editor
#--------------------------------------------------------------------------

PROJECT(leveleditor)
FILE(GLOB leveleditor_headers code/*.h code/tools/*.h)
FILE(GLOB leveleditor_sources code/*.cpp code/tools/*.cpp)

SET(files_leveleditor ${leveleditor_headers} ${leveleditor_sources})
SOURCE_GROUP("leveleditor" FILES ${files_leveleditor})

ADD_EXECUTABLE(leveleditor ${files_leveleditor})
TARGET_LINK_LIBRARIES(leveleditor core render physics)
ADD_DEPENDENCIES(leveleditor core render physics)

SET_TARGET_PROPERTIES(leveleditor PROPERTIES FOLDER "vortex/toolkit")