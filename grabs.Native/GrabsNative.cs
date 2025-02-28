using System.Runtime.InteropServices;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    public static GCHandle CreateHandle<T>(T obj)
    {
        return GCHandle.Alloc(obj);
    }

    public static T FromHandle<T>(GCHandle handle)
    {
        return (T) handle.Target;
    }
}
