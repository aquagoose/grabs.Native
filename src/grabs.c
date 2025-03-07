#include "grabs/grabs.h"

#include <stddef.h>

#ifdef _WIN32
#include <windows.h>
#define open LoadLibraryA
#define load GetProcAddress
#define LIB_NAME "grabs.Native.dll"
#else
#include <dlfcn.h>
#define open(name) dlopen(name, RTLD_LAZY)
#define load dlsym
#define LIB_NAME "./grabs.Native.so"
#endif

#define GS_IMPLFUNC(Name) PfnGs##Name##Func gs##Name = NULL;
#define GS_LOADFUNC(Name) gs##Name = (PfnGs##Name##Func) load(handle, "gs"#Name)

GS_IMPLFUNC(CreateInstance)
GS_IMPLFUNC(DestroyInstance)
GS_IMPLFUNC(GetCurrentBackend)
GS_IMPLFUNC(EnumerateAdapters)
GS_IMPLFUNC(CreateSurface)
GS_IMPLFUNC(CreateDevice)
GS_IMPLFUNC(DestroySurface)
GS_IMPLFUNC(DestroyDevice)
GS_IMPLFUNC(CreateBuffer)
GS_IMPLFUNC(CreateSwapchain)
GS_IMPLFUNC(CreateCommandList)
GS_IMPLFUNC(ExecuteCommandList)
GS_IMPLFUNC(DestroySwapchain)
GS_IMPLFUNC(GetNextSwapchainTexture)
GS_IMPLFUNC(SwapchainPresent)
GS_IMPLFUNC(DestroyCommandList)
GS_IMPLFUNC(BeginCommandList)
GS_IMPLFUNC(EndCommandList)
GS_IMPLFUNC(BeginRenderPass)
GS_IMPLFUNC(EndRenderPass)
GS_IMPLFUNC(DestroyBuffer)
GS_IMPLFUNC(CompileHLSL)
GS_IMPLFUNC(FreeCompiledSpirv)
GS_IMPLFUNC(CreateShaderModule)
GS_IMPLFUNC(DestroyShaderModule)

void gsInit()
{
    void* handle = open(LIB_NAME);
    GS_LOADFUNC(CreateInstance);
    GS_LOADFUNC(DestroyInstance);
    GS_LOADFUNC(GetCurrentBackend);
    GS_LOADFUNC(EnumerateAdapters);
    GS_LOADFUNC(CreateSurface);
    GS_LOADFUNC(CreateDevice);
    GS_LOADFUNC(DestroySurface);
    GS_LOADFUNC(DestroyDevice);
    GS_LOADFUNC(CreateBuffer);
    GS_LOADFUNC(CreateSwapchain);
    GS_LOADFUNC(CreateCommandList);
    GS_LOADFUNC(ExecuteCommandList);
    GS_LOADFUNC(DestroySwapchain);
    GS_LOADFUNC(GetNextSwapchainTexture);
    GS_LOADFUNC(SwapchainPresent);
    GS_LOADFUNC(DestroyCommandList);
    GS_LOADFUNC(BeginCommandList);
    GS_LOADFUNC(EndCommandList);
    GS_LOADFUNC(BeginRenderPass);
    GS_LOADFUNC(EndRenderPass);
    GS_LOADFUNC(DestroyBuffer);
    GS_LOADFUNC(CompileHLSL);
    GS_LOADFUNC(FreeCompiledSpirv);
    GS_LOADFUNC(CreateShaderModule);
    GS_LOADFUNC(DestroyShaderModule);
}

const char* gsResultToString(const GsResult result)
{
#define STR(x) case GS_RESULT_##x: return #x;

    switch (result)
    {
        STR(OK)
        STR(UNKNOWN_ERROR)
        STR(NO_BACKENDS)
        STR(DEBUG_LAYERS_NOT_FOUND)
        STR(FAILED_TO_PRESENT)
        STR(SHADER_COMPILATION_FAILED)

        default:
            return "UNKNOWN";
    }

#undef STR
}

const char* gsBackendToString(const GsBackend backend)
{
    switch (backend)
    {
        case GS_BACKEND_VULKAN:
            return "Vulkan";
        case GS_BACKEND_D3D11:
            return "D3D11";
        default:
            return "Unknown";
    }
}
