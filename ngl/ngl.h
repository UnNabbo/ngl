#include "ngl/std/std.h"

#include "ngl/src/base.h"


#ifdef NGL_WINDOW_MODULE
#include "ngl/src/window_module.h"
#endif

#if defined NGL_OPENGL_BACKEND
#include "ngl/src/backends/gl_backend.h"
#elif defined NGL_VULKAN_BACKEND
#include "ngl/src/backends/vk_backend.h"
#endif


#ifdef NGL_IMPLEMENTATION

#include "ngl/src/platform/win32_platform.cpp"

#if defined NGL_OPENGL_BACKEND
#include "ngl/src/backends/gl_backend.cpp"
#elif defined NGL_VULKAN_BACKEND
#include "ngl/src/backends/vk_backend.cpp"
#endif

#endif


#include "ngl/src/rendering_module.h"

