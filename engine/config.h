//------------------------------------------------------------------------------
/**
    @file core/config.h
    
	Main configure file for types and OS-specific stuff.
	
	(C) 2015 See the LICENSE file.
*/
#include <stdint.h>
#include <atomic>
#include <xmmintrin.h>
#include <immintrin.h>
#include <memory>
//#include <assert.h>

#define InvalidIndex -1;

typedef uint32_t	uint32;
typedef int32_t		int32;
typedef uint16_t	uint16;
typedef int16_t		int16;
typedef uint8_t		uint8;
typedef int8_t		int8;
typedef uint8_t		uchar;
typedef size_t      index_t;

typedef unsigned int	uint;
typedef unsigned short	ushort;

typedef uint8_t      byte;

typedef uint8_t		ubyte;
typedef float		float32;
typedef double		float64;

#if _MSC_VER
#define VORTEX_ALIGN16 __declspec(align(16))

#elif __GNUC__
#define __forceinline inline
#define VORTEX_ALIGN16 __attribute__((aligned(16)))

#else
#define VORTEX_ALIGN16
#endif

// assert macro
//#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
#define _assert(_Expression, _Msg) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(_Msg), _CRT_WIDE(__FILE__), __LINE__), 0) )

#ifdef NULL
#undef NULL
#define NULL nullptr
#endif
