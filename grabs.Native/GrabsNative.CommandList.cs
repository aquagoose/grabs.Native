using System.Runtime.InteropServices;
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

    [UnmanagedCallersOnly(EntryPoint = "gsCommandListBegin")]
    public static Result CommandListBegin(GCHandle commandList)
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

    [UnmanagedCallersOnly(EntryPoint = "gsCommandListEnd")]
    public static void CommandListEnd(GCHandle commandList)
    {
        CommandList gList = FromHandle<CommandList>(commandList);
        gList.End();
    }
}