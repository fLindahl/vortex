project (misc)

set (MISC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${MISC_DIR})

set(MISC_HEAD 
	#${MISC_DIR}/stb_image.h
	${MISC_DIR}/tinyxml2.h)
	
set(MISC_SRC
	${MISC_DIR}/tinyxml2.cpp
)

#If we're running windows, we need to include an external library for dirent functionality
#Linux has dirent natively
if(MSVC)
set(DIRENT
	${MISC_DIR}/dirent.h)
else()
set(DIRENT)
endif()
	
add_library (misc STATIC ${MISC_SRC} ${MISC_HEAD} ${DIRENT})
#target_link_libraries (misc ${MISC_LIBRARIES})
