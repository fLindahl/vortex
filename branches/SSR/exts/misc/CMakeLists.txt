project (misc)

set (MISC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${MISC_DIR})

set(MISC_HEAD 
	#${MISC_DIR}/stb_image.h
	${MISC_DIR}/tinyxml2.h)

set(MISC_SRC
	${MISC_DIR}/tinyxml2.cpp
)

add_library (misc STATIC ${MISC_SRC} ${MISC_HEAD})
#target_link_libraries (misc ${MISC_LIBRARIES})
