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

#define GS_APIFUNC(Name, RetType, ...) typedef RetType (*PfnGs##Name##Func)(__VA_ARGS__);\
    extern PfnGs##Name##Func gs##Name;

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* GsInstance;
    typedef void* GsSurface;
    typedef void* GsDevice;
    typedef void* GsBuffer;
    typedef void* GsSwapchain;
    typedef void* GsTexture;
    typedef void* GsCommandList;
    typedef void* GsShaderModule;
    typedef void* GsPipeline;
    typedef void* GsDescriptorLayout;

    typedef enum
    {
        GS_RESULT_OK,
        GS_RESULT_UNKNOWN_ERROR,
        GS_RESULT_NO_BACKENDS,
        GS_RESULT_DEBUG_LAYERS_NOT_FOUND,
        GS_RESULT_FAILED_TO_PRESENT,
        GS_RESULT_SHADER_COMPILATION_FAILED
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
        GS_BUFFER_TYPE_VERTEX,
        GS_BUFFER_TYPE_INDEX,
        GS_BUFFER_TYPE_CONSTANT
    } GsBufferType;

    typedef enum
    {
        GS_BUFFER_USAGE_DEFAULT,
        GS_BUFFER_USAGE_DYNAMIC
    } GsBufferUsage;

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

    typedef enum
    {
        GS_SHADER_STAGE_NONE = 0,
        GS_SHADER_STAGE_VERTEX,
        GS_SHADER_STAGE_PIXEL
    } GsShaderStage;

    typedef enum
    {
        GS_MAP_MODE_WRITE,
        GS_MAP_MODE_READ,
        GS_MAP_MODE_READ_AND_WRITE
    } GsMapMode;

    typedef enum
    {
        GS_DESCRIPTOR_TYPE_CONSTANT_BUFFER,
        GS_DESCRIPTOR_TYPE_TEXTURE
    } GsDescriptorType;

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
        GsBufferType type;
        uint32_t size;
        GsBufferUsage usage;
    } GsBufferInfo;

    typedef struct
    {
        float x;
        float y;
        float width;
        float height;
        float minDepth;
        float maxDepth;
    } GsViewport;

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

    typedef struct
    {
        uint32_t binding;
        uint32_t stride;
    } GsVertexBufferInfo;

    typedef struct
    {
        GsFormat format;
        uint32_t offset;
        uint32_t slot;
    } GsInputElement;

    typedef struct
    {
        GsShaderModule vertexShader;
        GsShaderModule pixelShader;
        uint32_t numColorAttachmentFormats;
        GsFormat* pColorAttachmentFormats;
        uint32_t numVertexBuffers;
        GsVertexBufferInfo* pVertexBuffers;
        uint32_t numInputElements;
        GsInputElement* pInputLayout;
        uint32_t numDescriptors;
        GsDescriptorLayout* pDescriptors;
    } GsPipelineInfo;

    typedef struct
    {
        void* pData;
    } GsMappedData;

    typedef struct
    {
        uint32_t binding;
        GsDescriptorType type;
        GsShaderStage stages;
    } GsDescriptorBinding;

    typedef struct
    {
        uint32_t numBindings;
        GsDescriptorBinding* pBindings;
    } GsDescriptorLayoutInfo;

    typedef struct
    {
        uint32_t slot;
        GsDescriptorType type;
        GsBuffer buffer;
        GsTexture texture;
    } GsDescriptor;

    extern void gsInit();
    extern const char* gsResultToString(GsResult result);
    extern const char* gsBackendToString(GsBackend backend);

    GS_APIFUNC(CreateInstance, GsResult, GsInstanceInfo *pInfo, GsInstance *pInstance)
    GS_APIFUNC(CreateSurface, GsResult, GsInstance instance, GsSurfaceInfo *pInfo, GsSurface *pSurface)
    GS_APIFUNC(CreateDevice, GsResult, GsInstance instance, GsSurface surface, GsAdapter *pAdapter, GsDevice *pDevice)
    GS_APIFUNC(CreateBuffer, GsResult, GsDevice device, GsBufferInfo *pBufferInfo, void* pData, GsBuffer *pBuffer)
    GS_APIFUNC(CreateSwapchain, GsResult, GsDevice device, GsSwapchainInfo *pInfo, GsSwapchain *pSwapchain)
    GS_APIFUNC(CreateCommandList, GsResult, GsDevice device, GsCommandList *pCommandList)
    GS_APIFUNC(CreateShaderModule, GsResult, GsDevice device, GsShaderStage stage, size_t spirvLength, uint8_t *pSpirv, const char *pEntryPoint, GsShaderModule *pModule)
    GS_APIFUNC(CreatePipeline, GsResult, GsDevice device, GsPipelineInfo *pInfo, GsPipeline *pPipeline)
    GS_APIFUNC(CreateDescriptorLayout, GsResult, GsDevice device, GsDescriptorLayoutInfo *pInfo, GsDescriptorLayout *pLayout)

    GS_APIFUNC(DestroyInstance, void, GsInstance instance)
    GS_APIFUNC(DestroySurface, void, GsSurface surface)
    GS_APIFUNC(DestroyDevice, void, GsDevice device)
    GS_APIFUNC(DestroyBuffer, void, GsBuffer buffer)
    GS_APIFUNC(DestroySwapchain, void, GsSwapchain swapchain)
    GS_APIFUNC(DestroyCommandList, void, GsCommandList commandList)
    GS_APIFUNC(DestroyShaderModule, void, GsShaderModule module)
    GS_APIFUNC(DestroyPipeline, void, GsPipeline pipeline)
    GS_APIFUNC(DestroyDescriptorLayout, void, GsDescriptorLayout layout)

    GS_APIFUNC(GetCurrentBackend, GsBackend, GsInstance instance)
    GS_APIFUNC(EnumerateAdapters, GsResult, GsInstance instance, uint32_t *pNumAdapters, GsAdapter *pAdapters)

    GS_APIFUNC(ExecuteCommandList, GsResult, GsDevice device, GsCommandList commandList)
    GS_APIFUNC(WaitForIdle, void, GsDevice device)
    GS_APIFUNC(MapBuffer, GsResult, GsDevice device, GsBuffer buffer, GsMapMode mode, GsMappedData *pData)
    GS_APIFUNC(UnmapBuffer, void, GsDevice device, GsBuffer buffer);

    GS_APIFUNC(GetNextSwapchainTexture, GsResult, GsSwapchain swapchain, GsTexture *pTexture)
    GS_APIFUNC(SwapchainPresent, GsResult, GsSwapchain swapchain)

    GS_APIFUNC(BeginCommandList, GsResult, GsCommandList commandList)
    GS_APIFUNC(EndCommandList, void, GsCommandList commandList)
    GS_APIFUNC(BeginRenderPass, void, GsCommandList commandList, GsRenderPassInfo *info)
    GS_APIFUNC(EndRenderPass, void, GsCommandList commandList)
    GS_APIFUNC(SetViewport, void, GsCommandList commandList, GsViewport *pViewport)
    GS_APIFUNC(SetPipeline, void, GsCommandList commandList, GsPipeline pipeline)
    GS_APIFUNC(SetVertexBuffer, void, GsCommandList commandList, uint32_t slot, GsBuffer buffer, uint32_t offset)
    GS_APIFUNC(SetIndexBuffer, void, GsCommandList commandList, GsBuffer buffer, GsFormat format, uint32_t offset)
    GS_APIFUNC(PushDescriptors, void, GsCommandList commandList, uint32_t slot, GsPipeline pipeline, uint32_t numDescriptors, GsDescriptor *pDescriptors)
    GS_APIFUNC(DrawIndexed, void, GsCommandList commandList, uint32_t numIndices)

    GS_APIFUNC(CompileHLSL, GsResult, GsShaderStage stage, const char *pHlsl, const char *pEntryPoint, size_t *pSpirvLength, uint8_t **ppSpirv)
    GS_APIFUNC(FreeCompiledSpirv, void, uint8_t *pSpirv)

#ifdef __cplusplus
}
#endif
#endif