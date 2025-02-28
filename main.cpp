#include <iostream>
#include <vector>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "grabs/grabs.h"

#define CHECK_RESULT(Operation) {\
    GsResult res = Operation;\
    if (res != GS_RESULT_OK)\
        throw std::runtime_error("grabs operation failed.");\
}

int main(int argc, char* argv[])
{
    gsInit();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Native GRABS test in C/++!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
    }

    GsInstanceInfo info
    {
        .name = "Test",
        .debug = true
    };

    GsInstance instance;
    CHECK_RESULT(gsCreateInstance(&info, &instance));

    const char* backend;
    CHECK_RESULT(gsInstanceGetBackend(instance, &backend));

    std::cout << backend << std::endl;

    uint32_t numAdapters;
    CHECK_RESULT(gsInstanceEnumerateAdapters(instance, &numAdapters, nullptr));
    std::vector<GsAdapter> adapters(numAdapters);
    CHECK_RESULT(gsInstanceEnumerateAdapters(instance, &numAdapters, adapters.data()));

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
            surfaceInfo.window.windows = wmInfo.info.win.hwnd;
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
    CHECK_RESULT(gsInstanceCreateSurface(instance, &surfaceInfo, &surface));

    GsDevice device;
    CHECK_RESULT(gsInstanceCreateDevice(instance, surface, nullptr, &device));

    GsSwapchainInfo swapchainInfo
    {
        .surface = surface,
        .size = { 800, 600 },
        .format = GS_FORMAT_B8G8R8A8_UNORM,
        .presentMode = GS_PRESENT_MODE_FIFO,
        .numBuffers = 2
    };
    GsSwapchain swapchain;
    CHECK_RESULT(gsDeviceCreateSwapchain(device, &swapchainInfo, &swapchain));

    GsCommandList cl;
    CHECK_RESULT(gsDeviceCreateCommandList(device, &cl));

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

        GsTexture texture;
        CHECK_RESULT(gsSwapchainGetNextTexture(swapchain, &texture));

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
        gsEndRenderPass(cl);

        gsEndCommandList(cl);

        CHECK_RESULT(gsDeviceExecuteCommandList(device, cl));
        CHECK_RESULT(gsSwapchainPresent(swapchain));
    }

    gsDestroyCommandList(cl);
    gsDestroySwapchain(swapchain);
    gsDestroyDevice(device);
    gsDestroySurface(surface);
    gsDestroyInstance(instance);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}