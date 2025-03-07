using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyPipeline")]
    public static void DestroyPipeline(GCHandle pipeline)
    {
        Pipeline gPipeline = FromHandle<Pipeline>(pipeline);
        gPipeline.Dispose();
        pipeline.Free();
    }
}