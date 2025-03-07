namespace grabs.Native;

public enum Result
{
    Ok,
    
    UnknownError,
    NoBackends,
    DebugLayersNotFound,
    FailedToPresent,
    ShaderCompilationFailed
}