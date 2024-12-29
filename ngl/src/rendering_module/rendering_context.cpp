rendering_context nglRenderingContextCreate(void * WindowHandle, u64 MemoryInBytes){
	nglPlatformSetupRenderingSurface(WindowHandle);

#if defined NGL_GL3W_BACKEND
	gl3wInit();
#elif defined NGL_VULKAN_BACKEND
	
#endif
	SwapBuffers(GetDC((HWND)WindowHandle));
	return {};
}

