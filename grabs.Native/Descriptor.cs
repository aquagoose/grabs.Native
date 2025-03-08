using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public struct Descriptor
{
    public uint Slot;
    public DescriptorType Type;
    public GCHandle Buffer;
    public GCHandle Texture;
}