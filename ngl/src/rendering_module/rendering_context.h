struct rendering_context{
	memory_arena Memory;
};

rendering_context nglRenderingContextCreate(void * NativeWindow, u64 MemoryInBytes);

