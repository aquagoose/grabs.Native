using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroySwapchain")]
    public static void DestroySwapchain(GCHandle swapchain)
    {
        Swapchain gSwapchain = FromHandle<Swapchain>(swapchain);
        gSwapchain.Dispose();
        swapchain.Free();
    }

    [UnmanagedCallersOnly(EntryPoint = "gsSwapchainGetNextTexture")]
    public static Result SwapchainGetNextTexture(GCHandle swapchain, GCHandle* pTexture)
    {
        Swapchain gSwapchain = FromHandle<Swapchain>(swapchain);

        try
        {
            *pTexture = ToHandle(gSwapchain.GetNextTexture());
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsSwapchainPresent")]
    public static Result SwapchainPresent(GCHandle swapchain)
    {
        Swapchain gSwapchain = FromHandle<Swapchain>(swapchain);

        try
        {
            gSwapchain.Present();
        }
        catch (Exception e)
        {
            return Result.FailedToPresent;
        }

        return Result.Ok;
    }
}