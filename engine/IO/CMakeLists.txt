#--------------------------------------------------------------------------
# IO - input output
#--------------------------------------------------------------------------

SET(files_IO
	console.h
	console.cpp
	callbacks.h
	serialization.cpp
	serialization.h
	)

SOURCE_GROUP("IO" FILES ${files_IO})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(IO STATIC ${files_IO} ${files_pch})
TARGET_PCH(IO ../)
ADD_DEPENDENCIES(IO glew glfw misc foundation )
TARGET_LINK_LIBRARIES(IO PUBLIC engine exts glew glfw misc core foundation)
