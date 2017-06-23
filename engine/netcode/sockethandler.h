#pragma once
/**
	@file       sockethandler.h
	@class      Netcode::SocketHandler
	
	@author     Fredrik Lindahl
	@copyright  See LICENSE file

	@brief      Berkeley TCP Socket Handler
*/
#include "config.h"
#include "core/types.h"

#if (__WIN32__)
    #include "windows/winsockethandler.h"
#elif (__LINUX__)
#include "linux/linuxsockethandler.h"
#else
    #error "UNKNOWN PLATFORM"
#endif