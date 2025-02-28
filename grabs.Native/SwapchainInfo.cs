using System.Runtime.InteropServices;
using grabs.Core;
using grabs.Graphics;

namespace grabs.Native;

public struct SwapchainInfo
{
    public GCHandle Surface;
    public Size2D Size;
    public Format Format;
    public PresentMode PresentMode;
    public uint NumBuffers;
}