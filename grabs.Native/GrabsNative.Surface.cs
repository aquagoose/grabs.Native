using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsInstanceCreateSurface")]
    public static Result InstanceCreateSurface(GCHandle instance, SurfaceInfo* info, GCHandle* pSurface)
    {
        Instance gInstance = FromHandle<Instance>(instance);

        try
        {
            Surface surface = gInstance.CreateSurface(in *info);
            *pSurface = ToHandle(surface);
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }
    
    [UnmanagedCallersOnly(EntryPoint = "gsDestroySurface")]
    public static void DestroySurface(GCHandle surface)
    {
        Surface gSurface = FromHandle<Surface>(surface);
        gSurface.Dispose();
        surface.Free();
    }
}