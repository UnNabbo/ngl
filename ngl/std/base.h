typedef signed char        s8;
typedef short              s16;
typedef int                s32;
typedef long long          s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

#define MIN_U8  0
#define MIN_U16 0
#define MIN_U32 0
#define MIN_U64 0
#define MAX_U8  0xFF
#define MAX_U16 0xFFFF
#define MAX_U32 0xFFFFFFFF
#define MAX_U64 0xFFFFFFFFFFFFFFFF
#define MIN_S8  0x80
#define MIN_S16 0x8000
#define MIN_S32 0x80000000
#define MIN_S64 0x8000000000000000
#define MAX_S8  0x7F
#define MAX_S16 0x7FFF
#define MAX_S32 0x7FFFFFFF
#define MAX_S64 0x7FFFFFFFFFFFFFFF

#ifndef INFINITY
#define INFINITY   ((float)(1e+300 * 1e+300))
#endif
#ifndef NAN
#define NAN        (-(float)(INFINITY * 0.0F))
#endif

#define null 0

#define INLINE __forceinline
