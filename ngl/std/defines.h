#define INLINE __forceinline
#define STALL while(true){}

#define TEMP
#define UNUSED
#define UNIMPLEMENTED
#define INTERNAL
#define EXTERNAL

#define for_range(var, amt) for(s32 var = 0; var < amt; var++)

#define RETURN_IF_NULL(x, ...) if(!x){return __VA_ARGS__; }

#define ZeroData(x) memset(x, 0, sizeof(*x))
#define ZeroArray(x) memset(x, 0, sizeof(x))

#define SAFE_EXEC(x, ...) if(x){ x(__VA_ARGS__); }

#define Kilobytes(amt) ((amt) * 1024)
#define Megabytes(amt) (Kilobytes(amt) * 1024)
#define Gigabytes(amt) (Megabytes(amt) * 1024)

#define FN(x) (*x)

#define BIT(i) (1 << i)
#define GET_BIT(Key, Bit) (((Key) & (BIT((Bit)))) >> (Bit))

#define S_TO_NS(amt) (amt * 1000000000)

#define ASSERT_BOUNDS(var, start, length) Assert(((var) >= (start)) && ((var) < ((start) + (length))), "OUT OF BOUNDS! expected between % and % but was %", (start), ((start) + (length)), (var))

#define RICHGREY   color4{38,  38,  38,  255}
#define LIGHTGREY  color4{200, 200, 200, 255}
#define GREY       color4{130, 130, 130, 255}
#define DARKGREY   color4{80,  80,  80,  255}
#define YELLOW     color4{253, 249, 0,   255}
#define GOLD       color4{255, 203, 0,   255}
#define ORANGE     color4{255, 161, 0,   255}
#define PINK       color4{255, 109, 194, 255}
#define RED        color4{230, 41,  55,  255}
#define MAROON     color4{190, 33,  55,  255}
#define GREEN      color4{0,   228, 48,  255}
#define LIME       color4{0,   158, 47,  255}
#define DARKGREEN  color4{0,   117, 44,  255}
#define SKYBLUE    color4{102, 191, 255, 255}
#define BLUE       color4{0,   121, 241, 255}
#define DARKBLUE   color4{0,   82,  172, 255}
#define PURPLE     color4{200, 122, 255, 255}
#define VIOLET     color4{135, 60,  190, 255}
#define DARKPURPLE color4{112, 31,  126, 255}
#define BEIGE      color4{211, 176, 131, 255}
#define BROWN      color4{127, 106, 79,  255}
#define DARKBROWN  color4{76,  63,  47,  255}
#define WHITE      color4{255, 255, 255, 255}
#define BLACK      color4{0,   0,   0,   255}
#define BLANK      color4{0,   0,   0,   0}
#define MAGENTA    color4{255, 0,   255, 255}
#define RICHGRAY   RICHGREY
#define DARKGRAY   DARKGREY
#define GRAY       GREY
#define LIGHTGRAY  LIGHTGREY

#define VEC2_EXPAND(v) v.x, v.y
#define VEC3_EXPAND(v) v.x, v.y, v.z
#define VEC4_EXPAND(v) v.x, v.y, v.z, v.w
