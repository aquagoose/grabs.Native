using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyDescriptorLayout")]
    public static void DestroyDescriptorLayout(GCHandle layout)
    {
        DescriptorLayout gLayout = FromHandle<DescriptorLayout>(layout);
        gLayout.Dispose();
        layout.Free();
    }
}