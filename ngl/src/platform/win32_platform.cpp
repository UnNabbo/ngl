struct window;

#ifdef NGL_WINDOW_MODULE

s64 nglPlatformEventsManager(void* hwnd, u32 msg, u64 wparam, s64 lparam) {
	window * Window = 0;
	if(msg != WM_CREATE){
		Window = (window*)GetWindowLongPtr((HWND)hwnd, GWLP_USERDATA);
	}
	
	switch(msg) {
		case WM_CREATE:{
			CREATESTRUCT *pCreate = (CREATESTRUCT*)(lparam);
			window* userdata = (window*)(pCreate->lpCreateParams);
			SetWindowLongPtr((HWND)hwnd, GWLP_USERDATA, (LONG_PTR)userdata);
		}break;
		case WM_SIZE:{
			event Event = {};
			Event.Flag = NGL_EVENT_CATEGORY_WINDOW | NGL_EVENT_WINDOW_RESIZE;
			Event.Width = LOWORD(lparam);
            Event.Height = HIWORD(lparam);
			nglWindowDispatchEvent(Window, Event);
		}break;
		case WM_CLOSE:{
			event Event = {};
			Event.Flag = NGL_EVENT_CATEGORY_WINDOW | NGL_EVENT_WINDOW_CLOSE;
			nglWindowDispatchEvent(Window, Event);
		}break;
		default: {
			return DefWindowProcA((HWND)hwnd, (UINT)msg, (WPARAM)wparam, (LPARAM)lparam);
		}
	}
	return 0;
}

void* nglPlatformWindowCreate(window * Window) {
	SetProcessDpiAwareness((PROCESS_DPI_AWARENESS)PROCESS_SYSTEM_DPI_AWARE);

	CoInitialize(0);

	WNDCLASSEXA wc = {};
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.lpfnWndProc   = (WNDPROC)nglPlatformEventsManager;
	wc.hInstance     = GetModuleHandleA(null);
	wc.lpszClassName = (LPCSTR)Window->Title;
	wc.hCursor       = LoadCursorA(null, (LPCSTR)32512);
	wc.hbrBackground = CreateSolidBrush((38 << 16) | (38 << 8) | 38);

	RegisterClassExA((WNDCLASSEXA *)&wc);

	// calculate the monitor size and adjust it in the middle of the screen
	s32 monitor_width = GetSystemMetrics(0);
	s32 monitor_height = GetSystemMetrics(1);
	s32 x_pos = (monitor_width - Window->Width) / 2;
	s32 y_pos = (monitor_height - Window->Height) / 2;

	RECT rect = {x_pos, y_pos, Window->Width + x_pos, Window->Height + y_pos};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	HWND win = CreateWindowExA(0, (LPCSTR)Window->Title, (LPCSTR)Window->Title, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, Window);

	if (!win) {
		exit(1);
		return null;
	}

	ShowWindow(win, 5);
	return win;
}


void nglPlatformWindowFetchMessages(void * Hwnd){
	while(1){
		MSG m;
		if(!PeekMessageA(&m, (HWND)Hwnd, 0, 0, 1)) break;

		TranslateMessage(&m);
		DispatchMessageA(&m);
	}
}

#ifdef NGL_GL3W_BACKEND

	void * nglPlatformSetupRenderingSurface(void * Hwnd) {
		HDC Hdc = GetDC((HWND) Hwnd);

		PIXELFORMATDESCRIPTOR pfd;
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = 0x20 | 0x04 | 0x01;
		pfd.iPixelType = 0;
		pfd.cColorBits = 32;
		int pf = ChoosePixelFormat(Hdc, &pfd);
		if(!pf) {
			exit(1);
		}
		if(!SetPixelFormat(Hdc, pf, &pfd)) {
			exit(1);
		}
		HGLRC Hglrc = wglCreateContext(Hdc);
		if(!wglMakeCurrent(Hdc, Hglrc)) {
			exit(1);
		}

		typedef void* wglCreateContextAttribsARB_type(void* hdc, void* hShareContext, const int *attribList);
		typedef s32 wglChoosePixelFormatARB_type(void* hdc, const int *piAttribIList, const float *pfAttribFList, u32 nMaxFormats, int *piFormats, u32 *nNumFormats);
		wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;
		wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

		wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress((LPCSTR)"wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress((LPCSTR)"wglChoosePixelFormatARB");

		if(!wglCreateContextAttribsARB || !wglChoosePixelFormatARB) {
			ReleaseDC((HWND)Hwnd, Hdc);
			exit(1);
		}

		s32 pf_attributes[] = {0x2001, 1, 0x2003, 0x2027, 0x2010, 1, 0x2011, 1, 0x2013, 0x202B, 0x2014, 32, 0x201B, 0, 0x2022, 0, 0x2023, 0, 0x201D, 0, 0x2024, 0, 0x20A9, 1, 0, 0};
		u32 count;
		if(!wglChoosePixelFormatARB(Hdc, pf_attributes, NULL, 1, &pf, &count)) {
			ReleaseDC((HWND)Hwnd, Hdc);
			exit(1);
		}

		PIXELFORMATDESCRIPTOR pfd2;
		DescribePixelFormat(Hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd2);
		SetPixelFormat(Hdc, pf, &pfd2);

		s32 Attribs[8];
		Attribs[0] = 0x2091;
		Attribs[1] = 4;
		Attribs[2] = 0x2092;
		Attribs[3] = 6;
		Attribs[4] = 0x9126;
		Attribs[5] = 1;
		Attribs[6] = 0;
		Attribs[7] = 0;

		HGLRC Hrc = (HGLRC)wglCreateContextAttribsARB(Hdc, null, Attribs);
		if(!wglMakeCurrent(Hdc, Hrc)) {
			ReleaseDC((HWND)Hwnd, Hdc);
			exit(1);
		}


		typedef s32 wglSwapIntervalEXT_type(s32 interval);
		wglSwapIntervalEXT_type* wglSwapIntervalEXT;
		wglSwapIntervalEXT = (wglSwapIntervalEXT_type*)wglGetProcAddress((LPCSTR)"wglSwapIntervalEXT");
		if(wglSwapIntervalEXT) {  // Note(Quattro) this function is an extension, no need to close the app if there isn't
			// 0 -> no vsync, 1 -> yes vsync
			wglSwapIntervalEXT(0);
		}
		return 0;
	}

#elif defined NGL_VULKAN_BACKEND
	
	void nglPlatformSetupRenderingSurface(void * NativeWindow) {	
	
	}

#else
#error unknown backend
#endif


#else

void nglPlatformSetupRenderingContext(void * NativeWindow) {
	
}

#endif
