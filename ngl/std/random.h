s32 RandomNumber(s32 Lower, s32 Upper) {
	return (rand() % (Upper - Lower + 1)) + Lower; 
}

f32 RandomFloat() {
	return (float)rand() / RAND_MAX;
}

f32 RandomFloatBetween(f32 x, f32 y) {
	return RandomFloat() * (y - x) + x;
}
