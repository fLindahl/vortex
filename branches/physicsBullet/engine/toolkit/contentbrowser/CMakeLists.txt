#--------------------------------------------------------------------------
# Content Browser
#--------------------------------------------------------------------------

PROJECT(contentbrowser)
FILE(GLOB contentbrowser_headers code/*.h code/tools/*.h)
FILE(GLOB contentbrowser_sources code/*.cpp code/tools/*.cpp)

SET(files_contentbrowser ${contentbrowser_headers} ${contentbrowser_sources})
SOURCE_GROUP("contentbrowser" FILES ${files_contentbrowser})

ADD_EXECUTABLE(contentbrowser ${files_contentbrowser})
TARGET_LINK_LIBRARIES(contentbrowser core render physics nativefiledialog)
ADD_DEPENDENCIES(contentbrowser core render physics nativefiledialog)

SET_TARGET_PROPERTIES(contentbrowser PROPERTIES FOLDER "vortex/toolkit")
