using System.Runtime.InteropServices;
using grabs.Core;
using grabs.Graphics;
using Buffer = grabs.Graphics.Buffer;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyCommandList")]
    public static void DestroyCommandList(GCHandle commandList)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        gList.Dispose();
        commandList.Free();
    }

    [UnmanagedCallersOnly(EntryPoint = "gsBeginCommandList")]
    public static Result BeginCommandList(GCHandle commandList)
    {
        CommandList gList = FromHandle<CommandList>(commandList);

        try
        {
            gList.Begin();
        }
        catch (Exception)
        {
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsEndCommandList")]
    public static void EndCommandList(GCHandle commandList)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        gList.End();
    }

    [UnmanagedCallersOnly(EntryPoint = "gsBeginRenderPass")]
    public static void BeginRenderPass(GCHandle commandList, RenderPassInfo* info)
    {
        CommandList gList = FromHandle<CommandList>(commandList);

        // TODO: Make this more efficient.
        Graphics.ColorAttachmentInfo[] colorAttachments = new Graphics.ColorAttachmentInfo[info->NumColorAttachments];

        for (uint i = 0; i < info->NumColorAttachments; i++)
        {
            ColorAttachmentInfo* attachmentInfo = &info->ColorAttachments[i];
            
            colorAttachments[i] = new Graphics.ColorAttachmentInfo()
            {
                Texture = FromHandle<Texture>(attachmentInfo->Texture),
                ClearColor = new ColorF()
                {
                    R = attachmentInfo->ClearColor[0],
                    G = attachmentInfo->ClearColor[1],
                    B = attachmentInfo->ClearColor[2],
                    A = attachmentInfo->ClearColor[3],
                },
                LoadOp = attachmentInfo->LoadOp
            };
        }

        Graphics.RenderPassInfo passInfo = new Graphics.RenderPassInfo(colorAttachments);
        
        gList.BeginRenderPass(passInfo);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsEndRenderPass")]
    public static void EndRenderPass(GCHandle commandList)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        gList.EndRenderPass();
    }

    [UnmanagedCallersOnly(EntryPoint = "gsSetViewport")]
    public static void SetViewport(GCHandle commandList, Viewport* viewport)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        gList.SetViewport(*viewport);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsSetPipeline")]
    public static void SetPipeline(GCHandle commandList, GCHandle pipeline)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        Pipeline gPipeline = FromHandle<Pipeline>(pipeline);
        
        gList.SetPipeline(gPipeline);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsSetVertexBuffer")]
    public static void SetVertexBuffer(GCHandle commandList, uint slot, GCHandle buffer, uint offset)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        Buffer gBuffer = FromHandle<Buffer>(buffer);
        
        gList.SetVertexBuffer(slot, gBuffer, offset);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsSetIndexBuffer")]
    public static void SetIndexBuffer(GCHandle commandList, GCHandle buffer, Format format, uint offset)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        Buffer gBuffer = FromHandle<Buffer>(buffer);
        
        gList.SetIndexBuffer(gBuffer, format, offset);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsPushDescriptors")]
    public static void PushDescriptors(GCHandle commandList, uint slot, GCHandle pipeline, uint numDescriptors,
        Descriptor* pDescriptors)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        Pipeline gPipeline = FromHandle<Pipeline>(pipeline);

        Graphics.Descriptor[] descriptors = new Graphics.Descriptor[numDescriptors];
        for (int i = 0; i < numDescriptors; i++)
        {
            Descriptor* descriptor = &pDescriptors[i];

            descriptors[i] = new Graphics.Descriptor()
            {
                Slot = descriptor->Slot,
                Type = descriptor->Type,
                Buffer = GCHandle.ToIntPtr(descriptor->Buffer) == IntPtr.Zero
                    ? null
                    : FromHandle<Buffer>(descriptor->Buffer),
                Texture = GCHandle.ToIntPtr(descriptor->Texture) == IntPtr.Zero
                    ? null
                    : FromHandle<Texture>(descriptor->Texture)
            };
        }
        
        gList.PushDescriptors(slot, gPipeline, descriptors);
    }

    [UnmanagedCallersOnly(EntryPoint = "gsDrawIndexed")]
    public static void DrawIndexed(GCHandle commandList, uint numIndices)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        gList.DrawIndexed(numIndices);
    }
}