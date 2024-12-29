
struct window{
	const char * Title;
	u32 Width, Height;
	void *Handle;
	
	ring<event> Events;
};

window * nglWindowCreate(const char * Title, u32 Width, u32 Height);
void nglWindowDestroy(window * Window);

void nglWindowFetchMessages(window * Window);
void nglWindowDispatchEvent(window * Window, event Event);
bool nglWindowFetchNextEvent(window * Window, event * Event);
