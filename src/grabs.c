#include "grabs/grabs.h"

#include <stddef.h>

#ifdef _WIN32
#include <windows.h>
#define open LoadLibraryA
#define load GetProcAddress
#else
#include <dlfcn.h>
#define open(name) dlopen(name, RTLD_LAZY)
#define load dlsym
#endif

#define GS_IMPLFUNC(Name) PfnGs##Name##Func gs##Name = NULL;
#define GS_LOADFUNC(Name) gs##Name = (PfnGs##Name##Func) load(handle, "gs"#Name)

GS_IMPLFUNC(CreateInstance)
GS_IMPLFUNC(DestroyInstance)
GS_IMPLFUNC(InstanceGetBackend)
GS_IMPLFUNC(InstanceEnumerateAdapters)
GS_IMPLFUNC(InstanceCreateSurface)
GS_IMPLFUNC(InstanceCreateDevice)
GS_IMPLFUNC(DestroySurface)
GS_IMPLFUNC(DestroyDevice)
GS_IMPLFUNC(DeviceCreateSwapchain)
GS_IMPLFUNC(DestroySwapchain)
GS_IMPLFUNC(SwapchainPresent);

void gsInit()
{
    void* handle = open("/home/aqua/Documents/C#/grabs.Native/grabs.Native/bin/Release/net9.0/linux-x64/publish/grabs.Native.so");
    GS_LOADFUNC(CreateInstance);
    GS_LOADFUNC(DestroyInstance);
    GS_LOADFUNC(InstanceGetBackend);
    GS_LOADFUNC(InstanceEnumerateAdapters);
    GS_LOADFUNC(InstanceCreateSurface);
    GS_LOADFUNC(InstanceCreateDevice);
    GS_LOADFUNC(DestroySurface);
    GS_LOADFUNC(DestroyDevice);
    GS_LOADFUNC(DeviceCreateSwapchain);
    GS_LOADFUNC(DestroySwapchain);
    GS_LOADFUNC(SwapchainPresent);
}
