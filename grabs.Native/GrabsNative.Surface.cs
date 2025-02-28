using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroySurface")]
    public static void DestroySurface(GCHandle surface)
    {
        Surface gSurface = FromHandle<Surface>(surface);
        gSurface.Dispose();
        surface.Free();
    }
}