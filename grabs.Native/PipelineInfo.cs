using System.Runtime.InteropServices;
using grabs.Graphics;

namespace grabs.Native;

public unsafe struct PipelineInfo
{
    public GCHandle VertexShader;
    public GCHandle PixelShader;
    public uint NumColorAttachmentFormats;
    public Format* ColorAttachmentFormats;
    public uint NumVertexBuffers;
    public VertexBufferInfo* VertexBuffers;
    public uint NumInputElements;
    public InputElement* InputLayout;
    public uint NumDescriptors;
    public GCHandle* Descriptors;
}