using System.Runtime.InteropServices;
using grabs.Core;
using grabs.Graphics;
using grabs.Graphics.D3D11;
using grabs.Graphics.Exceptions;
using grabs.Graphics.Vulkan;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsCreateInstance")]
    public static Result CreateInstance(InstanceInfo* info, GCHandle* pInstance)
    {
        GrabsLog.LogMessage += (severity, type, message, file, line) => Console.WriteLine(message);

        Backend backendHint = info->BackendHint;
        if (backendHint == Backend.Unknown)
            backendHint = Backend.Vulkan | Backend.D3D11;
        
        if (backendHint.HasFlag(Backend.D3D11))
            Instance.RegisterBackend<D3D11Backend>();
        if (backendHint.HasFlag(Backend.Vulkan))
            Instance.RegisterBackend<VulkanBackend>();

        Graphics.InstanceInfo grabsInfo = new()
        {
            AppName = new string(info->Name),
            Debug = info->Debug
        };

        Instance instance;
        try
        {
            instance = Instance.Create(in grabsInfo);
        }
        catch (NoBackendsException)
        {
            return Result.NoBackends;
        }
        catch (DebugLayersNotFoundException)
        {
            return Result.DebugLayersNotFound;
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        *pInstance = ToHandle(instance);

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsDestroyInstance")]
    public static void DestroyInstance(GCHandle instance)
    {
        Instance gInstance = FromHandle<Instance>(instance);
        gInstance.Dispose();
        instance.Free();
    }

    [UnmanagedCallersOnly(EntryPoint = "gsInstanceGetBackend")]
    public static Backend InstanceGetBackend(GCHandle instance)
    {
        Instance gInstance = FromHandle<Instance>(instance);

        if (gInstance.Backend == VulkanBackend.Name)
            return Backend.Vulkan;
        if (gInstance.Backend == D3D11Backend.Name)
            return Backend.D3D11;

        return Backend.Unknown;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsInstanceEnumerateAdapters")]
    public static Result InstanceEnumerateAdapters(GCHandle instance, uint* numAdapters, Adapter* adapters)
    {
        Instance gInstance = FromHandle<Instance>(instance);

        try
        {
            grabs.Graphics.Adapter[] gAdapters = gInstance.EnumerateAdapters();

            *numAdapters = (uint) gAdapters.Length;

            if (adapters != null)
            {
                for (int i = 0; i < gAdapters.Length; i++)
                {
                    Adapter adapter = new Adapter(gAdapters[i]);

                    adapters[i] = adapter;
                }
            }
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }
        
        return Result.Ok;
    }
    
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
}