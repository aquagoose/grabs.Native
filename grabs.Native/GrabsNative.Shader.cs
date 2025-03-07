using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using grabs.Graphics;
using grabs.ShaderCompiler;

namespace grabs.Native;

public static unsafe partial class GrabsNative
{
    [UnmanagedCallersOnly(EntryPoint = "gsDestroyShaderModule")]
    public static void DestroyShaderModule(GCHandle module)
    {
        ShaderModule gModule = FromHandle<ShaderModule>(module);
        gModule.Dispose();
        module.Free();
    }
    
    [UnmanagedCallersOnly(EntryPoint = "gsCompileHLSL")]
    public static Result CompileHLSL(ShaderStage stage, sbyte* pHlsl, sbyte* pEntryPoint, nuint* pSpirvLength, byte** pSpirv)
    {
        try
        {
            byte[] result = Compiler.CompileHlsl(stage, new string(pHlsl), new string(pEntryPoint));
            *pSpirv = (byte*) NativeMemory.Alloc((nuint) (result.Length * sizeof(byte*)));
            
            fixed (void* pResult = result)
                Unsafe.CopyBlock(*pSpirv, pResult, (uint) result.Length);
            
            *pSpirvLength = (nuint) result.Length;
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
    public static void FreeCompiledSpirv(byte* spirv)
    {
        NativeMemory.Free(spirv);
    }
}