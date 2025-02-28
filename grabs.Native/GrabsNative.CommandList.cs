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
}