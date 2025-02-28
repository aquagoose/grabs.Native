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
        
        Instance.RegisterBackend<D3D11Backend>();
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
    public static Result InstanceGetBackend(GCHandle instance, nint* pBackend)
    {
        Instance gInstance = FromHandle<Instance>(instance);
        *pBackend = Marshal.StringToCoTaskMemAnsi(gInstance.Backend);

        return Result.Ok;
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
}