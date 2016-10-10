//------------------------------------------------------------------------------
/**
    @file core/config.h
    
	Main configure file for types and OS-specific stuff.
	
	(C) 2015 See the LICENSE file.
*/
#ifdef __WIN32__
#include "win32/pch.h"
#endif

#include <stdint.h>
#include <atomic>
#include <xmmintrin.h>
#include <memory>
#include <assert.h>

#define InvalidIndex -1;

typedef uint32_t	uint32;
typedef int32_t		int32;
typedef uint16_t	uint16;
typedef int16_t		int16;
typedef uint8_t		uint8;
typedef int8_t		int8;
typedef uint8_t		uchar;

typedef uint32_t	uint;
typedef uint16_t ushort;

// eh, windows already defines byte, so don't redefine byte if we are running windows
#ifndef __WIN32__
typedef uint8_t      byte;
#endif

typedef uint8_t		ubyte;
typedef float		float32;
typedef double		float64;

// hmm, this is questionable, we actually want SSE vectors to operate as classes
//typedef __m128		vec4;
//typedef __m128i		ivec4;
//typedef __m128d		dvec4;

// assert macro
#define _assert(_Expression, _Msg) (void)( (!!(_Expression)) || (_wassert(_Msg, _CRT_WIDE(__FILE__), __LINE__), 0) )

#ifdef NULL
#undef NULL
#define NULL nullptr
#endif
