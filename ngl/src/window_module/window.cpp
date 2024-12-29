
void* nglPlatformWindowCreate(window * Window);
window* nglWindowCreate(const char * Title, u32 Width, u32 Height){
	window * Window = (window* )MemAlloc(sizeof(window));
	*Window = {Title, Width, Height, 0, RingCreate<event>(256)};
	Window->Handle = nglPlatformWindowCreate(Window);
	return Window;
}

void nglPlatformWindowFetchMessages(void * Hwnd);
void nglWindowFetchMessages(window * Window){
	nglPlatformWindowFetchMessages(Window->Handle);
}

bool nglWindowFetchNextEvent(window * Window, event * Event){
	if(Window->Events[-1].Flag != NGL_EVENT_NULL){
		*Event = Window->Events[-1];
		Window->Events.Offset++;
		return true;
	}
	return false;
}

void nglWindowDispatchEvent(window * Window, event Event){
	RingAppend(&Window->Events, Event);
}

void nglWindowDestroy(window * Window){
	MemFree(Window);
}
