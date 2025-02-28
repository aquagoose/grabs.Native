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

    typedef void* GsInstance;
    typedef void* GsSurface;
    typedef void* GsDevice;
    typedef void* GsSwapchain;
    typedef void* GsTexture;
    typedef void* GsCommandList;

    typedef enum
    {
        GS_RESULT_OK,
        GS_RESULT_UNKNOWN_ERROR,
        GS_RESULT_NO_BACKENDS,
        GS_RESULT_DEBUG_LAYERS_NOT_FOUND,
        GS_RESULT_FAILED_TO_PRESENT
    } GsResult;

    typedef enum
    {
        GS_BACKEND_UNKNOWN = 0,
        GS_BACKEND_VULKAN = 1 << 0,
        GS_BACKEND_D3D11 = 1 << 1
    } GsBackend;

    typedef enum
    {
        GS_FORMAT_UNKNOWN,
        GS_FORMAT_B5G6R5_UNORM,
        GS_FORMAT_B5G5R5A1_UNORM,
        GS_FORMAT_R8_UNORM,
        GS_FORMAT_R8_UINT,
        GS_FORMAT_R8_SNORM,
        GS_FORMAT_R8_SINT,
        GS_FORMAT_A8_UNORM,
        GS_FORMAT_R8G8_UNORM,
        GS_FORMAT_R8G8_UINT,
        GS_FORMAT_R8G8_SNORM,
        GS_FORMAT_R8G8_SINT,
        GS_FORMAT_R8G8B8A8_UNORM,
        GS_FORMAT_R8G8B8A8_UNORM_SRGB,
        GS_FORMAT_R8G8B8A8_UINT,
        GS_FORMAT_R8G8B8A8_SNORM,
        GS_FORMAT_R8G8B8A8_SINT,
        GS_FORMAT_B8G8R8A8_UNORM,
        GS_FORMAT_B8G8R8A8_UNORM_SRGB,
        GS_FORMAT_R16_FLOAT,
        GS_FORMAT_D16_UNORM,
        GS_FORMAT_R16_UNORM,
        GS_FORMAT_R16_UINT,
        GS_FORMAT_R16_SNORM,
        GS_FORMAT_R16_SINT,
        GS_FORMAT_R16G16_FLOAT,
        GS_FORMAT_R16G16_UNORM,
        GS_FORMAT_R16G16_UINT,
        GS_FORMAT_R16G16_SNORM,
        GS_FORMAT_R16G16_SINT,
        GS_FORMAT_R16G16B16A16_FLOAT,
        GS_FORMAT_R16G16B16A16_UNORM,
        GS_FORMAT_R16G16B16A16_UINT,
        GS_FORMAT_R16G16B16A16_SNORM,
        GS_FORMAT_R16G16B16A16_SINT,
        GS_FORMAT_R32_FLOAT,
        GS_FORMAT_R32_UINT,
        GS_FORMAT_R32_SINT,
        GS_FORMAT_R32G32_FLOAT,
        GS_FORMAT_R32G32_UINT,
        GS_FORMAT_R32G32_SINT,
        GS_FORMAT_R32G32B32_FLOAT,
        GS_FORMAT_R32G32B32_UINT,
        GS_FORMAT_R32G32B32_SINT,
        GS_FORMAT_R32G32B32A32_FLOAT,
        GS_FORMAT_R32G32B32A32_UINT,
        GS_FORMAT_R32G32B32A32_SINT,
        GS_FORMAT_D24_UNORM_S8_UINT,
        GS_FORMAT_D32_FLOAT,
        GS_FORMAT_BC1_UNORM,
        GS_FORMAT_BC1_UNORM_SRGB,
        GS_FORMAT_BC2_UNORM,
        GS_FORMAT_BC2_UNORM_SRGB,
        GS_FORMAT_BC3_UNORM,
        GS_FORMAT_BC3_UNORM_SRGB,
        GS_FORMAT_BC4_UNORM,
        GS_FORMAT_BC4_SNORM,
        GS_FORMAT_BC5_UNORM,
        GS_FORMAT_BC5_SNORM,
        GS_FORMAT_BC6H_UF16,
        GS_FORMAT_BC6H_SF16,
        GS_FORMAT_BC7_UNORM,
        GS_FORMAT_BC7_UNORM_SRGB
    } GsFormat;

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

    typedef enum
    {
        GS_PRESENT_MODE_IMMEDIATE,
        GS_PRESENT_MODE_FIFO,
        GS_PRESENT_MODE_FIFO_RELAXED,
        GS_PRESENT_MODE_MAILBOX
    } GsPresentMode;

    typedef enum
    {
        GS_LOAD_OP_CLEAR,
        GS_LOAD_OP_LOAD
    } GsLoadOp;

    typedef struct
    {
        uint32_t width;
        uint32_t height;
    } GsSize2D;

    typedef struct
    {
        const char* name;
        GsBackend backendHint;
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

    typedef struct
    {
        GsSurface surface;
        GsSize2D size;
        GsFormat format;
        GsPresentMode presentMode;
        uint32_t numBuffers;
    } GsSwapchainInfo;

    typedef struct
    {
        GsTexture texture;
        float clearColor[4];
        GsLoadOp loadOp;
    } GsColorAttachmentInfo;

    typedef struct
    {
        uint32_t numColorAttachments;
        GsColorAttachmentInfo* pColorAttachments;
    } GsRenderPassInfo;

    extern void gsInit();
    extern const char* gsResultToString(GsResult result);
    extern const char* gsBackendToString(GsBackend backend);

    GS_APIFUNC(CreateInstance, GsResult, GsInstanceInfo *pInfo, GsInstance *pInstance)
    GS_APIFUNC(DestroyInstance, void, GsInstance instance)

    GS_APIFUNC(InstanceGetBackend, GsBackend, GsInstance instance)
    GS_APIFUNC(InstanceEnumerateAdapters, GsResult, GsInstance instance, uint32_t *pNumAdapters, GsAdapter *pAdapters)
    GS_APIFUNC(InstanceCreateSurface, GsResult, GsInstance instance, GsSurfaceInfo *pInfo, GsSurface *pSurface)
    GS_APIFUNC(InstanceCreateDevice, GsResult, GsInstance instance, GsSurface surface, GsAdapter *pAdapter, GsDevice *pDevice)

    GS_APIFUNC(DestroySurface, void, GsSurface surface)

    GS_APIFUNC(DestroyDevice, void, GsDevice device)
    GS_APIFUNC(DeviceCreateSwapchain, GsResult, GsDevice device, GsSwapchainInfo *pInfo, GsSwapchain *pSwapchain)
    GS_APIFUNC(DeviceCreateCommandList, GsResult, GsDevice device, GsCommandList *pCommandList)
    GS_APIFUNC(DeviceExecuteCommandList, GsResult, GsDevice device, GsCommandList commandList)

    GS_APIFUNC(DestroySwapchain, void, GsSwapchain swapchain)
    GS_APIFUNC(SwapchainGetNextTexture, GsResult, GsSwapchain swapchain, GsTexture *pTexture)
    GS_APIFUNC(SwapchainPresent, GsResult, GsSwapchain swapchain)

    GS_APIFUNC(DestroyCommandList, void, GsCommandList commandList)
    GS_APIFUNC(BeginCommandList, GsResult, GsCommandList commandList)
    GS_APIFUNC(EndCommandList, void, GsCommandList commandList)
    GS_APIFUNC(BeginRenderPass, void, GsCommandList commandList, GsRenderPassInfo *info)
    GS_APIFUNC(EndRenderPass, void, GsCommandList commandList)

#ifdef __cplusplus
}
#endif
#endif