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

#include <stdint.h>
#include <atomic>
#include <xmmintrin.h>
#include <immintrin.h>
#include <memory>
#include <assert.h>
#include <exception>
#include <iostream>
#include "core/types.h"
//#include "core/debug.h"


// assert macro
//#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#ifdef __GNUC__
#define _assert(expr, msg) ((expr) ? __ASSERT_VOID_CAST(0) : __assert_fail(#msg, __FILE__, __LINE__, __ASSERT_FUNCTION))
#else
//#define _assert(_Expression, _Msg) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(_Msg), _CRT_WIDE(__FILE__), __LINE__), 0) )
#define _assert(condition, message)                                            \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__   \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate();                                                  \
        }                                                                      \
    } while (false)
#endif
