#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "grabs/grabs.h"

#define CHECK_RESULT(Operation) {\
    GsResult res = Operation;\
    if (res != GS_RESULT_OK)\
        throw std::runtime_error("grabs operation failed: " + std::string(gsResultToString(res)));\
}

const char* ShaderCode = R"(
struct VSInput
{
    float3 Position: POSITION0;
    float2 TexCoord: TEXCOORD0;
};

struct VSOutput
{
    float4 Position: SV_Position;
    float2 TexCoord: TEXCOORD0;
};

struct PSOutput
{
    float4 Color: SV_Target0;
};

cbuffer Matrices : register(b0)
{
    float4x4 Matrix;
}

VSOutput VSMain(const in VSInput input)
{
    VSOutput output;

    output.Position = mul(Matrix, float4(input.Position, 1.0));
    output.TexCoord = input.TexCoord;

    return output;
}

PSOutput PSMain(const in VSOutput input)
{
    PSOutput output;

    output.Color = float4(input.TexCoord, 0.0, 1.0);

    return output;
}
)";

int main(int argc, char* argv[])
{
    gsInit();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Native GRABS test in C/++!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    if (!window)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
    }

    GsInstanceInfo info
    {
        .name = "Test",
        .backendHint = GS_BACKEND_UNKNOWN,
        .debug = true
    };

    GsInstance instance;
    CHECK_RESULT(gsCreateInstance(&info, &instance));

    const GsBackend backend = gsGetCurrentBackend(instance);

    std::cout << gsBackendToString(backend) << std::endl;

    uint32_t numAdapters;
    CHECK_RESULT(gsEnumerateAdapters(instance, &numAdapters, nullptr));
    std::vector<GsAdapter> adapters(numAdapters);
    CHECK_RESULT(gsEnumerateAdapters(instance, &numAdapters, adapters.data()));

    for (const auto& adapter : adapters)
        std::cout << adapter.name << std::endl;

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

    GsSurfaceInfo surfaceInfo;

    switch (wmInfo.subsystem)
    {
#ifdef _WIN32
        case SDL_SYSWM_WINDOWS:
            surfaceInfo.type = GS_SURFACE_TYPE_WINDOWS;
            surfaceInfo.display.windows = wmInfo.info.win.hinstance;
            surfaceInfo.window.windows = wmInfo.info.win.window;
            break;
#endif
#ifdef __linux__
        case SDL_SYSWM_X11:
            surfaceInfo.type = GS_SURFACE_TYPE_XLIB;
            surfaceInfo.display.xlib = wmInfo.info.x11.display;
            surfaceInfo.window.xlib = wmInfo.info.x11.window;
            break;
        case SDL_SYSWM_WAYLAND:
            surfaceInfo.type = GS_SURFACE_TYPE_WAYLAND;
            surfaceInfo.display.wayland = wmInfo.info.wl.display;
            surfaceInfo.window.wayland = wmInfo.info.wl.surface;
            break;
#endif
    }

    GsSurface surface;
    CHECK_RESULT(gsCreateSurface(instance, &surfaceInfo, &surface));

    GsDevice device;
    CHECK_RESULT(gsCreateDevice(instance, surface, nullptr, &device));

    GsSwapchainInfo swapchainInfo
    {
        .surface = surface,
        .size = { 1280, 720 },
        .format = GS_FORMAT_B8G8R8A8_UNORM,
        .presentMode = GS_PRESENT_MODE_FIFO,
        .numBuffers = 2
    };
    GsSwapchain swapchain;
    CHECK_RESULT(gsCreateSwapchain(device, &swapchainInfo, &swapchain));

    GsCommandList cl;
    CHECK_RESULT(gsCreateCommandList(device, &cl));

    float vertices[]
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, +0.5f, 0.0f, 0.0f, 0.0f,
        +0.5f, +0.5f, 0.0f, 1.0f, 0.0f,
        +0.5f, -0.5f, 0.0f, 1.0f, 1.0f
    };

    uint16_t indices[]
    {
        0, 1, 3,
        1, 2, 3
    };

    float matrix[][4]
    {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f, },
        { 0.0f, 0.0f, 1.0f, 0.0f, },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    GsBufferInfo bufferInfo
    {
        .type = GS_BUFFER_TYPE_VERTEX,
        .size = sizeof(vertices),
        .usage = GS_BUFFER_USAGE_DEFAULT
    };

    GsBuffer vertexBuffer;
    CHECK_RESULT(gsCreateBuffer(device, &bufferInfo, vertices, &vertexBuffer));

    bufferInfo.type = GS_BUFFER_TYPE_INDEX;
    bufferInfo.size = sizeof(indices);

    GsBuffer indexBuffer;
    CHECK_RESULT(gsCreateBuffer(device, &bufferInfo, indices, &indexBuffer));

    bufferInfo.type = GS_BUFFER_TYPE_CONSTANT;
    bufferInfo.size = sizeof(matrix);
    bufferInfo.usage = GS_BUFFER_USAGE_DYNAMIC;

    GsBuffer constantBuffer;
    CHECK_RESULT(gsCreateBuffer(device, &bufferInfo, matrix, &constantBuffer));

    GsMappedData mappedData;
    CHECK_RESULT(gsMapBuffer(device, constantBuffer, GS_MAP_MODE_WRITE, &mappedData));

    GsDescriptorBinding bindings[]
    {
        { .binding = 0, .type = GS_DESCRIPTOR_TYPE_CONSTANT_BUFFER, .stages = GS_SHADER_STAGE_VERTEX }
    };

    GsDescriptorLayoutInfo layoutInfo
    {
        .numBindings = 1,
        .pBindings = bindings
    };

    GsDescriptorLayout layout;
    CHECK_RESULT(gsCreateDescriptorLayout(device, &layoutInfo, &layout));

    size_t spirvLength;
    uint8_t* spirv;

    CHECK_RESULT(gsCompileHLSL(GS_SHADER_STAGE_VERTEX, ShaderCode, "VSMain", &spirvLength, &spirv));
    GsShaderModule vertexModule;
    CHECK_RESULT(gsCreateShaderModule(device, GS_SHADER_STAGE_VERTEX, spirvLength, spirv, "VSMain", &vertexModule));
    gsFreeCompiledSpirv(spirv);

    CHECK_RESULT(gsCompileHLSL(GS_SHADER_STAGE_PIXEL, ShaderCode, "PSMain", &spirvLength, &spirv));
    GsShaderModule pixelModule;
    CHECK_RESULT(gsCreateShaderModule(device, GS_SHADER_STAGE_PIXEL, spirvLength, spirv, "PSMain", &pixelModule));
    gsFreeCompiledSpirv(spirv);

    GsFormat format = GS_FORMAT_B8G8R8A8_UNORM;

    GsVertexBufferInfo vertexBufferInfo
    {
        .binding = 0,
        .stride = 5 * sizeof(float)
    };

    GsInputElement inputLayout[]
    {
        { .format = GS_FORMAT_R32G32B32_FLOAT, .offset = 0, .slot = 0 },
        { .format = GS_FORMAT_R32G32_FLOAT, .offset = 12, .slot = 0 }
    };

    GsPipelineInfo pipelineInfo
    {
        .vertexShader = vertexModule,
        .pixelShader = pixelModule,
        .numColorAttachmentFormats = 1,
        .pColorAttachmentFormats = &format,
        .numVertexBuffers = 1,
        .pVertexBuffers = &vertexBufferInfo,
        .numInputElements = 2,
        .pInputLayout = inputLayout,
        .numDescriptors = 1,
        .pDescriptors = &layout
    };

    GsPipeline pipeline;
    CHECK_RESULT(gsCreatePipeline(device, &pipelineInfo, &pipeline));

    gsDestroyShaderModule(pixelModule);
    gsDestroyShaderModule(vertexModule);

    float h = 0;

    bool alive = true;
    while (alive)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            alive = false;
                            break;
                    }
                    break;
                }
            }
        }

        h += 0.05f;
        matrix[3][0] = sin(h);
        memcpy(mappedData.pData, matrix, sizeof(matrix));

        GsTexture texture;
        CHECK_RESULT(gsGetNextSwapchainTexture(swapchain, &texture));

        CHECK_RESULT(gsBeginCommandList(cl));

        GsColorAttachmentInfo colorAttachment
        {
            .texture = texture,
            .clearColor = { 1.0f, 0.5f, 0.25f, 1.0f },
            .loadOp = GS_LOAD_OP_CLEAR
        };

        GsRenderPassInfo passInfo
        {
            .numColorAttachments = 1,
            .pColorAttachments = &colorAttachment
        };

        gsBeginRenderPass(cl, &passInfo);

        GsViewport viewport
        {
            .x = 0,
            .y = 0,
            .width = 1280,
            .height = 720,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
        gsSetViewport(cl, &viewport);

        GsDescriptor descriptors[]
        {
            { .slot = 0, .type = GS_DESCRIPTOR_TYPE_CONSTANT_BUFFER, .buffer = constantBuffer }
        };
        gsPushDescriptors(cl, 0, pipeline, 1, descriptors);

        gsSetPipeline(cl, pipeline);
        gsSetVertexBuffer(cl, 0, vertexBuffer, 0);
        gsSetIndexBuffer(cl, indexBuffer, GS_FORMAT_R16_UINT, 0);
        gsDrawIndexed(cl, 6);

        gsEndRenderPass(cl);

        gsEndCommandList(cl);

        CHECK_RESULT(gsExecuteCommandList(device, cl));
        CHECK_RESULT(gsSwapchainPresent(swapchain));
    }

    gsWaitForIdle(device);

    gsDestroyPipeline(pipeline);
    gsDestroyDescriptorLayout(layout);
    gsDestroyBuffer(constantBuffer);
    gsDestroyBuffer(indexBuffer);
    gsDestroyBuffer(vertexBuffer);
    gsDestroyCommandList(cl);
    gsDestroySwapchain(swapchain);
    gsDestroyDevice(device);
    gsDestroySurface(surface);
    gsDestroyInstance(instance);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}