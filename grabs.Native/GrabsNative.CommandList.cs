using System.Runtime.InteropServices;
using grabs.Core;
using grabs.Graphics;

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
}