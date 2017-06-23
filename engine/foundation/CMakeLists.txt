#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------

SET(files_foundation_math
	math/math.h
	math/matrix4.h	
	math/matrix4.cpp	
	math/vector.h
	math/vector4.h
	math/vector4.cpp
	math/point.h
	math/quaternion.h
	math/plane.cpp
	math/plane.h
	math/line.cpp
	math/line.h
	math/shapes/quad.h
	math/bbox.h
	math/bbox.cpp
	)

SET(files_foundation_util
	util/array.h
	util/fixedarray.h
	util/indexedpriorityqueue.h
	util/variable.h
	util/string.h
	util/string.cpp
	util/hashtable.h
	util/dictionary.h
	util/pair.h
	util/fourcc.h
	util/list.h
	util/threadpool.h
	util/threadpool.cpp
	)

SET(files_foundation_messaging
	messaging/messagehandler.h
	messaging/messagehandler.cpp
	messaging/message.h
	messaging/message.cpp
	messaging/messagetype.h
	)

IF(WIN32)
	SET(files_foundation_memory_OS
		memory/windows/winmemory.h
		memory/windows/winmemoryconfig.h
		memory/windows/winmemoryconfig.cpp
		)
ELSE(WIN32)
	SET(files_foundation_memory_OS
		memory/linux/linuxmemory.h
		memory/linux/linuxmemoryconfig.h
		memory/linux/linuxmemoryconfig.cpp
		)
ENDIF(WIN32)

SET(files_foundation_memory_base
	memory/memory.h
	${files_foundation_memory_OS}
)

SOURCE_GROUP("math" FILES ${files_foundation_math})
SOURCE_GROUP("util" FILES ${files_foundation_util})
SOURCE_GROUP("messaging" FILES ${files_foundation_messaging})
SOURCE_GROUP("memory" FILES ${files_foundation_memory_base})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(foundation STATIC ${files_foundation_math} ${files_foundation_util} ${files_foundation_memory_base} ${files_foundation_messaging} ${files_pch})
TARGET_PCH(foundation ../)
ADD_DEPENDENCIES(foundation glew misc core)
TARGET_LINK_LIBRARIES(foundation PUBLIC engine exts glew misc core)
