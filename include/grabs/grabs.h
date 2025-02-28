#ifndef GS_GRABS
#define GS_GRABS

#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux__
#include <X11/Xlib-xcb.h>
#endif

#define GS_APIFUNC(Name, RetType, Params...) typedef RetType (*PfnGs##Name##Func)(Params);\
    extern PfnGs##Name##Func gs##Name;

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        GS_RESULT_OK,
        GS_RESULT_UNKNOWN_ERROR,
        GS_RESULT_NO_BACKENDS,
        GS_RESULT_DEBUG_LAYERS_NOT_FOUND
    } GsResult;

    typedef enum
    {
        GS_ADAPTER_TYPE_OTHER,
        GS_ADAPTER_TYPE_SOFTWARE,
        GS_ADAPTER_TYPE_INTEGRATED,
        GS_ADAPTER_TYPE_DEDICATED,
    } GsAdapterType;

    typedef enum
    {
        GS_SURFACE_TYPE_WINDOWS,
        GS_SURFACE_TYPE_XLIB,
        GS_SURFACE_TYPE_XCB,
        GS_SURFACE_TYPE_WAYLAND
    } GsSurfaceType;

    typedef struct
    {
        const char* name;
        bool debug;
    } GsInstanceInfo;

    typedef struct
    {
        void* handle;
        uint32_t index;
        const char* name;
        GsAdapterType type;
        uint64_t dedicatedMemory;
    } GsAdapter;

    typedef struct
    {
        GsSurfaceType type;

        union
        {
#ifdef _WIN32
            HINSTANCE windows;
#endif
#ifdef __linux__
            Display* xlib;
            xcb_connection_t* xcb;
            struct wl_display* wayland;
#endif
        } display;

        union
        {
#ifdef _WIN32
            HWND windows;
#endif
#ifdef __linux__
            Window xlib;
            xcb_window_t xcb;
            struct wl_surface* wayland;
#endif
        } window;
    } GsSurfaceInfo;

    typedef void* GsInstance;
    typedef void* GsSurface;
    typedef void* GsDevice;

    extern void gsInit();

    GS_APIFUNC(CreateInstance, GsResult, GsInstanceInfo *pInfo, GsInstance *pInstance)
    GS_APIFUNC(DestroyInstance, void, GsInstance instance);
    GS_APIFUNC(InstanceGetBackend, GsResult, GsInstance instance, const char **pBackend)
    GS_APIFUNC(InstanceEnumerateAdapters, GsResult, GsInstance instance, uint32_t *pNumAdapters, GsAdapter *pAdapters);
    GS_APIFUNC(InstanceCreateSurface, GsResult, GsInstance instance, GsSurfaceInfo *pInfo, GsSurface *pSurface);
    GS_APIFUNC(DestroySurface, void, GsSurface surface);
    GS_APIFUNC(InstanceCreateDevice, GsResult, GsInstance instance, GsSurface surface, GsAdapter *pAdapter, GsDevice *pDevice);
    GS_APIFUNC(DestroyDevice, void, GsDevice device);

#ifdef __cplusplus
}
#endif
#endif