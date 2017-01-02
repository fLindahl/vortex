#--------------------------------------------------------------------------
# Level Editor
#--------------------------------------------------------------------------

PROJECT(leveleditor)
FILE(GLOB leveleditor_headers code/*.h)
FILE(GLOB leveleditor_sources code/*.cpp)

SET(files_leveleditor ${leveleditor_headers} ${leveleditor_sources})
SOURCE_GROUP("leveleditor" FILES ${files_leveleditor})

ADD_EXECUTABLE(leveleditor ${files_leveleditor})
TARGET_LINK_LIBRARIES(leveleditor core render fysik)
ADD_DEPENDENCIES(leveleditor core render fysik)

SET_TARGET_PROPERTIES(leveleditor PROPERTIES FOLDER "vortex/toolkit")