namespace grabs.Native;

[Flags]
public enum Backend
{
    Unknown = 0,
    
    Vulkan = 1 << 0,
    D3D11 = 1 << 1
}