using System.Runtime.InteropServices;
using grabs.Graphics;
using Buffer = grabs.Graphics.Buffer;

namespace grabs.Native;

public unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyBuffer")]
    public static void DestroyBuffer(GCHandle buffer)
    {
        Buffer gBuffer = FromHandle<Buffer>(buffer);
        gBuffer.Dispose();
        buffer.Free();
    }
}