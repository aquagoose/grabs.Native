using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public unsafe struct Adapter
{
    public nint Handle;

    public uint Index;

    public sbyte* Name;

    public AdapterType Type;

    public ulong DedicatedMemory;

    public Adapter(grabs.Graphics.Adapter adapter)
    {
        Handle = adapter.Handle;
        Index = adapter.Index;
        Name = (sbyte*) Marshal.StringToCoTaskMemAnsi(adapter.Name);
        Type = adapter.Type;
        DedicatedMemory = adapter.DedicatedMemory;
    }
}