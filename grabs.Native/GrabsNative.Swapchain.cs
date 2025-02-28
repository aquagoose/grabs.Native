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
}