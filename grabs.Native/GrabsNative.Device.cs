using System.Runtime.CompilerServices;
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

    [UnmanagedCallersOnly(EntryPoint = "gsCreateSwapchain")]
    public static Result CreateSwapchain(GCHandle device, SwapchainInfo* info, GCHandle* pSurface)
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

    [UnmanagedCallersOnly(EntryPoint = "gsCreateCommandList")]
    public static Result CreateCommandList(GCHandle device, GCHandle* pCommandList)
    {
        Device gDevice = FromHandle<Device>(device);

        try
        {
            *pCommandList = ToHandle(gDevice.CreateCommandList());
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsCreateShaderModule")]
    public static Result CreateShaderModule(GCHandle device, ShaderStage stage, nuint spirvLength, byte* pSpirv, sbyte* pEntryPoint, GCHandle* pModule)
    {
        Device gDevice = FromHandle<Device>(device);

        byte[] spirv = new byte[spirvLength];
        fixed (byte* pManagedSpirv = spirv)
            Unsafe.CopyBlock(pManagedSpirv, pSpirv, (uint) spirvLength);

        try
        {
            *pModule = ToHandle(gDevice.CreateShaderModule(stage, spirv, new string(pEntryPoint)));
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsCreateBuffer")]
    public static Result CreateBuffer(GCHandle device, BufferInfo* pInfo, void* pData, GCHandle* pBuffer)
    {
        Device gDevice = FromHandle<Device>(device);

        try
        {
            *pBuffer = ToHandle(gDevice.CreateBuffer(*pInfo, pData));
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsCreatePipeline")]
    public static Result CreatePipeline(GCHandle device, PipelineInfo* pInfo, GCHandle* pPipeline)
    {
        Device gDevice = FromHandle<Device>(device);
        
        Graphics.PipelineInfo info = new Graphics.PipelineInfo()
        {
            VertexShader = FromHandle<ShaderModule>(pInfo->VertexShader),
            PixelShader = FromHandle<ShaderModule>(pInfo->PixelShader),
            ColorAttachmentFormats =
                new ReadOnlySpan<Format>(pInfo->ColorAttachmentFormats, (int) pInfo->NumColorAttachmentFormats),
            VertexBuffers = new ReadOnlySpan<VertexBufferInfo>(pInfo->VertexBuffers, (int) pInfo->NumVertexBuffers),
            InputLayout = new ReadOnlySpan<InputElement>(pInfo->InputLayout, (int) pInfo->NumInputElements)
        };

        try
        {
            *pPipeline = ToHandle(gDevice.CreatePipeline(in info));
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsExecuteCommandList")]
    public static Result DeviceExecuteCommandList(GCHandle device, GCHandle commandList)
    {
        Device gDevice = FromHandle<Device>(device);
        CommandList gList = FromHandle<CommandList>(commandList);

        try
        {
            gDevice.ExecuteCommandList(gList);
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }
}