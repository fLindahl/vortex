#--------------------------------------------------------------------------
# Shared Tools
#--------------------------------------------------------------------------

PROJECT(tools)
FILE(GLOB tools_headers *.h)
FILE(GLOB tools_sources *.cpp)

SET(files_tools ${tools_headers} ${tools_sources})
SOURCE_GROUP("tools" FILES ${files_tools})

ADD_EXECUTABLE(tools ${files_tools})
TARGET_LINK_LIBRARIES(tools core render)
ADD_DEPENDENCIES(tools core render)

SET_TARGET_PROPERTIES(tools PROPERTIES FOLDER "vortex/toolkit")