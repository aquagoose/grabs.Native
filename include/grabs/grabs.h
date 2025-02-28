#ifndef GS_GRABS
#define GS_GRABS

#include <stdbool.h>
#include <stdint.h>

#define GS_APIFUNC(Name, RetType, Params...) typedef RetType (*PfnGs##Name##Func)(Params);\
    extern PfnGs##Name##Func gs##Name;

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        GS_RESULT_OK,
        GS_RESULT_UNKNOWN_ERROR,
        GS_RESULT_NO_BACKENDS,
        GS_RESULT_DEBUG_LAYERS_NOT_FOUND
    } GsResult;

    typedef struct
    {
        const char* name;
        bool debug;
    } GsInstanceInfo;

    typedef void* GsInstance;

    extern void gsInit();

    GS_APIFUNC(CreateInstance, GsResult, GsInstanceInfo *pInfo, GsInstance *pInstance)
    GS_APIFUNC(InstanceGetBackend, GsResult, GsInstance instance, const char** pBackend)

#ifdef __cplusplus
}
#endif
#endif