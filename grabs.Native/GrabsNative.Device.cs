using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using grabs.Graphics;
using Buffer = grabs.Graphics.Buffer;

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

    [UnmanagedCallersOnly(EntryPoint = "gsCreateTexture")]
    public static Result CreateTexture(GCHandle device, TextureInfo* pInfo, void* pData, GCHandle* pTexture)
    {
        Device gDevice = FromHandle<Device>(device);

        try
        {
            *pTexture = ToHandle(gDevice.CreateTexture(*pInfo, pData));
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

        DescriptorLayout[] layouts = new DescriptorLayout[pInfo->NumDescriptors];
        for (int i = 0; i < pInfo->NumDescriptors; i++)
            layouts[i] = FromHandle<DescriptorLayout>(pInfo->Descriptors[i]);
        
        Graphics.PipelineInfo info = new Graphics.PipelineInfo()
        {
            VertexShader = FromHandle<ShaderModule>(pInfo->VertexShader),
            PixelShader = FromHandle<ShaderModule>(pInfo->PixelShader),
            ColorAttachmentFormats =
                new ReadOnlySpan<Format>(pInfo->ColorAttachmentFormats, (int) pInfo->NumColorAttachmentFormats),
            VertexBuffers = new ReadOnlySpan<VertexBufferInfo>(pInfo->VertexBuffers, (int) pInfo->NumVertexBuffers),
            InputLayout = new ReadOnlySpan<InputElement>(pInfo->InputLayout, (int) pInfo->NumInputElements),
            Descriptors = layouts
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

    [UnmanagedCallersOnly(EntryPoint = "gsCreateDescriptorLayout")]
    public static Result CreateDescriptorLayout(GCHandle device, DescriptorLayoutInfo* pInfo, GCHandle* pLayout)
    {
        Device gDevice = FromHandle<Device>(device);

        Graphics.DescriptorLayoutInfo info = new()
        {
            Bindings = new ReadOnlySpan<DescriptorBinding>(pInfo->Bindings, (int) pInfo->NumBindings)
        };

        try
        {
            *pLayout = ToHandle(gDevice.CreateDescriptorLayout(in info));
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

    [UnmanagedCallersOnly(EntryPoint = "gsMapBuffer")]
    public static Result MapBuffer(GCHandle device, GCHandle buffer, MapMode mode, MappedData* pData)
    {
        Device gDevice = FromHandle<Device>(device);
        Buffer gBuffer = FromHandle<Buffer>(buffer);

        try
        {
            *pData = gDevice.MapResource(gBuffer, mode);
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsUnmapBuffer")]
    public static void UnmapBuffer(GCHandle device, GCHandle buffer)
    {
        Device gDevice = FromHandle<Device>(device);
        Buffer gBuffer = FromHandle<Buffer>(buffer);
        
        gDevice.UnmapResource(gBuffer);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsWaitForIdle")]
    public static void WaitForIdle(GCHandle device)
    {
        Device gDevice = FromHandle<Device>(device);
        gDevice.WaitForIdle();
    }
}