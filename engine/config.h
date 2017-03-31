#pragma once
/**
* @file         config.h
* @brief        Configurations for Vortex
* @date         28-02-2017
* @author       Fredrik Lindahl
* @copyright    See LICENCE file
*/

#ifdef __WIN32__
#undef __WIN32__
#endif
#ifdef WIN32
#define __WIN32__ (1)
#endif

#ifdef __GNUC__
#ifndef __LINUX__
#define __LINUX__ (1)
#endif
#endif

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdint.h>
#include <atomic>
#include <xmmintrin.h>
#include <immintrin.h>
#include <memory>
#include <assert.h>
#include <exception>
#include <iostream>
#include "core/types.h"
#include "foundation/memory/memory.h"
#include <cstdarg>
#include "core/debug.h"

#ifdef __WIN32__
#pragma warning ( disable : 4091)
//#pragma warning ( disable : 10358)     // illegal or unsupported __attribute__
#endif

#ifdef __WIN32__
#define  __attribute__(x) /**/
#endif