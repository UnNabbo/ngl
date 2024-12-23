#if !DEBUG_MEMORY

#define MemAlloc(Size) calloc(1, Size)
#define MemRealloc(Ptr, Size) realloc(Ptr, Size)
#define MemCopy(Destination, Source, Size) memcpy(Destination, Source, Size)
#define MemFree(ptr) free(ptr)

#else

u64 AllocCounter = 0;
u64 AllocSize = 0;


INLINE void * DebugMemAlloc(u64 Amount, u64 Size,string8 File, u32 Line){
	//Log("%, %",File, Line);
	AllocCounter++;
	return calloc(Amount, Size);
}

INLINE void DebugMemFree(void * Pointer){
	AllocCounter--;
	free(Pointer);
}


#define MemAlloc(Size) DebugMemAlloc(1, Size, __FILE__, __LINE__)
#define MemRealloc(Ptr, Size) realloc(Ptr, Size)
#define MemCopy(Destination, Source, Size) memcpy(Destination, Source, Size)
#define MemFree(ptr) DebugMemFree(ptr)

#endif


struct memory_arena {
    u64 Size;
    u8* Base;
	u64 Used;
};

INLINE memory_arena ArenaAllocate(u32 Size) {
	u8 * Data = (u8*)MemAlloc(Size);
	return {Size, Data, 0};
}

bool IsPowerOfTwo(u64 x) {
	return (x & (x-1)) == 0;
}

u64 AlignForward(u64 ptr, u64 align) {
	u64 p, a, modulo;
	
	//Assert(IsPowerOfTwo(align), "Allignment is not a power of 2");
	
	p = ptr;
	a = (u64)align;
	// Same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);
	
	if (modulo != 0) {
		// If 'p' address is not aligned, push the address to the
		// next value which is aligned
		p += a - modulo;
	}
	return p;
}

#define DEFAULT_ALIGNMENT (2*sizeof(void *))

INLINE void * ArenaBump(memory_arena* Arena, u64 Size, u64 Alignment = DEFAULT_ALIGNMENT){
	// Align 'curr_offset' forward to the specified alignment
	u64 curr_ptr = (u64)Arena->Base + Arena->Used;
	u64 offset = AlignForward(curr_ptr, Alignment);
	offset -= (u64)Arena->Base; // ChArenange to relative offset
	
	// Check to see if the backing memory has space left
	if (offset + Size <= Arena->Size) {
		void *ptr = &Arena->Base[offset];
		Arena->Used = offset + Size;
		
		memset(ptr, 0, Size);
		return ptr;
	}
	// Return NULL if the arena is out of memory (or handle differently)
	return 0;
}

INLINE void ArenaClean(memory_arena* Arena){
	Arena->Used = 0;
}

INLINE void ArenaFree(memory_arena* Arena){
	Arena->Size = 0;
	Arena->Used =0;
	
	free(Arena->Base);
	Arena->Base = 0;
}
