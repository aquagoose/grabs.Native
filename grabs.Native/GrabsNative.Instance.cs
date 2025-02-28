using System.Runtime.InteropServices;
using grabs.Core;
using grabs.Graphics;
using grabs.Graphics.D3D11;
using grabs.Graphics.Exceptions;
using grabs.Graphics.Vulkan;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    private static Dictionary<nint, Instance> _instanceHandles = new Dictionary<nint, Instance>();
    
    [UnmanagedCallersOnly(EntryPoint = "gsCreateInstance")]
    public static Result CreateInstance(InstanceInfo* info, nint* pInstance)
    {
        GrabsLog.LogMessage += (severity, type, message, file, line) => Console.WriteLine(message);
        
        Instance.RegisterBackend<D3D11Backend>();
        Instance.RegisterBackend<VulkanBackend>();

        Graphics.InstanceInfo grabsInfo = new()
        {
            AppName = new string(info->Name),
            Debug = info->Debug
        };

        Instance gInstance;
        try
        {
            gInstance = Instance.Create(in grabsInfo);
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
        
        *pInstance = CreateHandle(_instanceHandles, gInstance);

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsInstanceGetBackend")]
    public static Result InstanceGetBackend(nint instance, nint* pBackend)
    {
        if (!TryGetHandle(_instanceHandles, instance, out Instance gInstance))
            return Result.InvalidHandle;

        *pBackend = Marshal.StringToCoTaskMemAnsi(gInstance.Backend);

        return Result.Ok;
    }
}