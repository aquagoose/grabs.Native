using System.Runtime.InteropServices;
using grabs.Graphics;
using grabs.ShaderCompiler;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsCompileHLSL")]
    public static Result CompileHLSL(ShaderStage stage, sbyte* pHlsl, sbyte* pEntryPoint, GCHandle* pSpirv)
    {
        try
        {
            byte[] result = Compiler.CompileHlsl(stage, new string(pHlsl), new string(pEntryPoint));
            *pSpirv = GCHandle.Alloc(result, GCHandleType.Pinned);
        }
        catch (CompilationException e)
        {
            Console.WriteLine(e);
            return Result.ShaderCompilationFailed;
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            return Result.UnknownError;
        }

        return Result.Ok;
    }

    [UnmanagedCallersOnly(EntryPoint = "gsFreeCompiledSpirv")]
    public static void FreeCompiledSpirv(GCHandle spirv)
    {
        spirv.Free();
    }
}