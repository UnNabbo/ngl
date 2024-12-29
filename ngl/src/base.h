#define INLINE __forceinline
#define STALL while(true){}

#define TEMP
#define UNUSED
#define UNIMPLEMENTED
#define INTERNAL
#define EXTERNAL

#define for_range(var, base, amt) for(s32 var = base; var < amt; var += (base < amt) ? 1 : -1)

#define Kilobytes(amt) ((amt) * 1024)
#define Megabytes(amt) (Kilobytes(amt) * 1024)
#define Gigabytes(amt) (Megabytes(amt) * 1024)

#define BIT(i) (1 << i)
#define GET_BIT(Key, Bit) (((Key) & (BIT((Bit)))) >> (Bit))
