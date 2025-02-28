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

PfnGsCreateInstanceFunc gsCreateInstance = NULL;
PfnGsInstanceGetBackendFunc gsInstanceGetBackend = NULL;

void gsInit()
{
    void* handle = open("/home/aqua/Documents/C#/grabs.Native/grabs.Native/bin/Release/net9.0/linux-x64/publish/grabs.Native.so");
    gsCreateInstance = (PfnGsCreateInstanceFunc) load(handle, "gsCreateInstance");
    gsInstanceGetBackend = (PfnGsInstanceGetBackendFunc) load(handle, "gsInstanceGetBackend");
}
