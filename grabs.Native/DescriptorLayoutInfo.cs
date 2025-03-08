using grabs.Graphics;

namespace grabs.Native;

public unsafe struct DescriptorLayoutInfo
{
    public uint NumBindings;
    public DescriptorBinding* Bindings;
}