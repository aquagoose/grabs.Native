namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    private static nint _handleIndex;

    private static nint CreateHandle<T>(Dictionary<nint, T> dict, T obj)
    {
        nint handle = ++_handleIndex;
        dict.Add(handle, obj);
        return handle;
    }

    private static bool TryGetHandle<T>(Dictionary<nint, T> dict, nint handle, out T obj)
    {
        if (!dict.TryGetValue(handle, out obj))
            return false;

        return true;
    }
}
