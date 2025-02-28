using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public unsafe struct ColorAttachmentInfo
{
    public GCHandle Texture;
    public fixed float ClearColor[4];
    public LoadOp LoadOp;
}