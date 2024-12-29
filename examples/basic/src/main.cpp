#include <windows.h>
#include <shellscalingapi.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VULKAN

#ifdef OPENGL
#include "GL/gl3w.h"
#define NGL_OPENGL_BACKEND
#endif

#ifdef VULKAN
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_win32.h"
#define NGL_VULKAN_BACKEND
#endif

#define NGL_IMPLEMENTATION
#define NGL_WINDOW_MODULE
#include "ngl/ngl.h"

void HandleEvent(event Event, bool * IsOpen){
	switch(Event.Flag & NGL_EVENT_CATEGORY_MASK){
		case NGL_EVENT_CATEGORY_WINDOW:{
			switch(Event.Flag & ~NGL_EVENT_CATEGORY_MASK){
				case NGL_EVENT_WINDOW_CLOSE:{
					*IsOpen = false;
				}break;
				case NGL_EVENT_WINDOW_RESIZE:{
					print("Resize: %, %", Event.Width, Event.Height);
				}break;
			}
		}break;
	}
}

int main() 
{
	window *Window = nglWindowCreate("Title", 800, 600);
	rendering_context Context = nglRenderingContextCreate(Window->Handle, Megabytes(256));
	bool IsOpen = true;
	
	while(IsOpen){
		nglWindowFetchMessages(Window);
		event Event = {};
		while(nglWindowFetchNextEvent(Window, &Event)){
			HandleEvent(Event, &IsOpen);
		}
	}

	nglWindowDestroy(Window);
	
	return 0;
}
