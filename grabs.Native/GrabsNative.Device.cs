using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyDevice")]
    public static void DestroyDevice(GCHandle device)
    {
        Device gDevice = FromHandle<Device>(device);
        gDevice.Dispose();
        device.Free();
    }

    [UnmanagedCallersOnly(EntryPoint = "gsDeviceCreateSwapchain")]
    public static Result DeviceCreateSwapchain(GCHandle device, SwapchainInfo* info, GCHandle* pSurface)
    {
        Device gDevice = FromHandle<Device>(device);

        Graphics.SwapchainInfo gInfo = new()
        {
            Surface = FromHandle<Surface>(info->Surface),
            Size = info->Size,
            Format = info->Format,
            PresentMode = info->PresentMode,
            NumBuffers = info->NumBuffers
        };

        try
        {
            *pSurface = ToHandle(gDevice.CreateSwapchain(in gInfo));
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }
}