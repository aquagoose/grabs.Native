using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsInstanceCreateDevice")]
    public static Result InstanceCreateDevice(GCHandle instance, GCHandle surface, Adapter* adapter, GCHandle* pDevice)
    {
        Instance gInstance = FromHandle<Instance>(instance);
        Surface gSurface = FromHandle<Surface>(surface);

        try
        {
            Graphics.Adapter? gAdapter = adapter == null
                ? null
                : new Graphics.Adapter(adapter->Handle, 0, null, AdapterType.Other, 0);

            *pDevice = ToHandle(gInstance.CreateDevice(gSurface, gAdapter));
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsDestroyDevice")]
    public static void DestroyDevice(GCHandle device)
    {
        Device gDevice = FromHandle<Device>(device);
        gDevice.Dispose();
        device.Free();
    }
}