#include <iostream>
#include <dlfcn.h>

struct InstanceInfo
{
    const char* Name;
    bool Debug;
};

enum class Result
{
    Ok
};

typedef void* Instance;

typedef Result (*PfnCreateInstance)(InstanceInfo* info, Instance* instance);
typedef Result (*PfnInstanceGetBackend)(Instance instance, char** backend);

PfnCreateInstance gsCreateInstance;
PfnInstanceGetBackend gsInstanceGetBackend;

int main(int argc, char* argv[])
{
    void* handle = dlopen("/home/aqua/Documents/C#/grabs.Native/grabs.Native/bin/Release/net9.0/linux-x64/publish/grabs.Native.so", RTLD_LAZY);
    gsCreateInstance = (PfnCreateInstance) dlsym(handle, "gsCreateInstance");
    gsInstanceGetBackend = (PfnInstanceGetBackend) dlsym(handle, "gsInstanceGetBackend");

    InstanceInfo info
    {
        "Test",
        true
    };

    Instance instance;
    Result result = gsCreateInstance(&info, &instance);

    char* backend;
    gsInstanceGetBackend(instance, &backend);

    std::cout << backend << std::endl;

    return 0;
}