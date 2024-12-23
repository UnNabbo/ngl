#define FNV_64_PRIME 0x100000001b3
#define FNV_64_OFFSET_BIAS 0xcbf29ce484222325

#define XXHASH_64_PRIME1 11400714785074694791ULL
#define XXHASH_64_PRIME2 14029467366897019727ULL
#define XXHASH_64_PRIME3 1609587929392839161ULL
#define XXHASH_64_PRIME4 9650029242287828579ULL
#define XXHASH_64_PRIME5 2870177450012600261ULL

inline u64 FNVHash(u64 Value, u64 H){
	return (H ^ (Value)) * FNV_64_PRIME;
}

u64 FNVHash(void * Data, u64 Size) {
	u64 H = FNV_64_OFFSET_BIAS;
    for (int i = 0; i < Size; i++) {
		H = FNVHash(((u8*)Data)[i], H);
    }
    return H;
}

template<typename T>
u64 FNVHash(T Data) {
    return FNVHash(&Data, sizeof(T));
}

u64 FNVHash(string S) {
    return FNVHash(S.Data, S.Size);
}


static inline u64 XXHashRotateLeft(u64 x, unsigned char bits)
{
	return (x << bits) | (x >> (64 - bits));
}
/// process a single 64 bit value
static inline u64 XXHashProcessSingle(u64 previous, u64 input)
{
	return XXHashRotateLeft(previous + input * XXHASH_64_PRIME2, 31) * XXHASH_64_PRIME1;
}
/// process a block of 4x4 bytes, this is the main part of the XXHash32 algorithm
static inline void XXHashProcess(const void* data, u64& state0, u64& state1, u64& state2, u64& state3)
{
	const u64* block = (const u64*) data;
	state0 = XXHashProcessSingle(state0, block[0]);
	state1 = XXHashProcessSingle(state1, block[1]);
	state2 = XXHashProcessSingle(state2, block[2]);
	state3 = XXHashProcessSingle(state3, block[3]);
}

u64 XXHashHash(void * Data, u64 Size){
	const u64 MaxBufferSize = 32;
	
	u64 BufferSize = 0;
	u64 TotalSize = 0;
	u64 State[4];
	u8 Buffer[32];
	
	u64 Seed = 0;
	
	State[0] = Seed + XXHASH_64_PRIME1 + XXHASH_64_PRIME2;
	State[1] = Seed + XXHASH_64_PRIME2;
	State[2] = Seed;
	State[3] = Seed - XXHASH_64_PRIME1;
	
	if (!Data || Size == 0){
		//ASSETY
	}
	
    TotalSize += Size;
    // byte-wise access
    u8* Data8 = (u8*)Data;
    // unprocessed old Data8 plus new Data8 still fit in temporary buffer ?
    if (BufferSize + Size < MaxBufferSize)
    {
		// just add new Data8
		while (Size-- > 0)
			Buffer[BufferSize++] = *Data8++;
    }
    // point beyond last byte
    u8* Stop      = Data8 + Size;
    u8* StopBlock = Stop - MaxBufferSize;
    // some Data8 left from previous update ?
    if (BufferSize > 0)
    {
		// make sure temporary Buffer is full (16 bytes)
		while (BufferSize < MaxBufferSize)
			Buffer[BufferSize++] = *Data8++;
		// process these 16 bytes (4x4)
		XXHashProcess(Buffer, State[0], State[1], State[2], State[3]);
    }
    // copying State to local variables helps optimizer A LOT
    u64 s0 = State[0], s1 = State[1], s2 = State[2], s3 = State[3];
    // 16 bytes at once
    while (Data8 <= StopBlock)
    {
		// local variables s0..s3 instead of State[0]..State[3] are much faster
		XXHashProcess(Data8, s0, s1, s2, s3);
		Data8 += 16;
    }
    // copy back
    State[0] = s0; State[1] = s1; State[2] = s2; State[3] = s3;
    // copy remainder to temporary Buffer
    BufferSize = Stop - Data8;
    for (unsigned int i = 0; i < BufferSize; i++)
		Buffer[i] = Data8[i];
	
	
	u64 Result = 0;
	if (TotalSize>= MaxBufferSize)
	{
		Result = XXHashRotateLeft(State[0],  1) +
			XXHashRotateLeft(State[1],  7) +
			XXHashRotateLeft(State[2], 12) +
			XXHashRotateLeft(State[3], 18);
		Result = (Result ^ XXHashProcessSingle(0, State[0])) * XXHASH_64_PRIME1 + XXHASH_64_PRIME4;
		Result = (Result ^ XXHashProcessSingle(0, State[1])) * XXHASH_64_PRIME1 + XXHASH_64_PRIME4;
		Result = (Result ^ XXHashProcessSingle(0, State[2])) * XXHASH_64_PRIME1 + XXHASH_64_PRIME4;
		Result = (Result ^ XXHashProcessSingle(0, State[3])) * XXHASH_64_PRIME1 + XXHASH_64_PRIME4;
	}
	else
	{
		Result = State[2] + XXHASH_64_PRIME5;
	}
	Result += TotalSize;
	// XXHashProcess remaining bytes in temporary Buffer
	Data8 = Buffer;
	// point beyond last byte
	Stop = Data8 + BufferSize;
	// at least 8 bytes left ? => eat 8 bytes per step
	for (; Data8 + 8 <= Stop; Data8 += 8)
		Result = XXHashRotateLeft(Result ^ XXHashProcessSingle(0, *(u64*)Data8), 27) * XXHASH_64_PRIME1 + XXHASH_64_PRIME4;
	// 4 bytes left ? => eat those
	if (Data8 + 4 <= Stop)
	{
		Result = XXHashRotateLeft(Result ^ (*(u32*)Data8) * XXHASH_64_PRIME1,   23) * XXHASH_64_PRIME2 + XXHASH_64_PRIME3;
		Data8  += 4;
	}
	// take care of remaining 0..3 bytes, eat 1 byte per step
	while (Data8 != Stop)
		Result = XXHashRotateLeft(Result ^ (*Data8++) * XXHASH_64_PRIME5,            11) * XXHASH_64_PRIME1;
	// mix bits
	Result ^= Result >> 33;
	Result *= XXHASH_64_PRIME2;
	Result ^= Result >> 29;
	Result *= XXHASH_64_PRIME3;
	Result ^= Result >> 32;
	return Result;
}

template<typename T>
u64 XXHashHash(T Data) {
    return XXHashHash(&Data, sizeof(T));
}

u64 XXHashHash(string S) {
    return XXHashHash(S.Data, S.Size);
}
