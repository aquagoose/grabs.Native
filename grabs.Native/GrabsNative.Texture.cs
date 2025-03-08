using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyTexture")]
    public static void DestroyTexture(GCHandle texture)
    {
        Texture gTexture = FromHandle<Texture>(texture);
        gTexture.Dispose();
        texture.Free();
    }
}