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

    typedef enum
    {
        GS_ADAPTER_TYPE_OTHER,
        GS_ADAPTER_TYPE_SOFTWARE,
        GS_ADAPTER_TYPE_INTEGRATED,
        GS_ADAPTER_TYPE_DEDICATED,
    } GsAdapterType;

    typedef struct
    {
        const char* name;
        bool debug;
    } GsInstanceInfo;

    typedef struct
    {
        void* handle;
        uint32_t index;
        const char* name;
        GsAdapterType type;
        uint64_t dedicatedMemory;
    } GsAdapter;

    typedef void* GsInstance;

    extern void gsInit();

    GS_APIFUNC(CreateInstance, GsResult, GsInstanceInfo *pInfo, GsInstance *pInstance)
    GS_APIFUNC(InstanceGetBackend, GsResult, GsInstance instance, const char **pBackend)
    GS_APIFUNC(InstanceEnumerateAdapters, GsResult, GsInstance instance, uint32_t *pNumAdapters, GsAdapter *pAdapters);

#ifdef __cplusplus
}
#endif
#endif