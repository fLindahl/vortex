#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------

SET(files_foundation_math
	math/matrix3.h
	math/matrix4.h	
	math/vector2.h
	math/vector3.h
	math/vector4.h
	)

SET(files_foundation_util
	util/array.h
	)
	
SOURCE_GROUP("math" FILES ${files_foundation_math})
SOURCE_GROUP("util" FILES ${files_foundation_util})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(foundation STATIC ${files_foundation_math} ${files_foundation_util} ${files_pch})
TARGET_PCH(foundation ../)
ADD_DEPENDENCIES(foundation glew)
TARGET_LINK_LIBRARIES(foundation PUBLIC engine exts glew)
