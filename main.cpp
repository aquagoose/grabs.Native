#include <iostream>
#include <vector>

#include "grabs/grabs.h"

int main(int argc, char* argv[])
{
    gsInit();

    GsInstanceInfo info
    {
        "Test",
        true
    };

    GsInstance instance;
    GsResult result = gsCreateInstance(&info, &instance);

    const char* backend;
    gsInstanceGetBackend(instance, &backend);

    std::cout << backend << std::endl;

    uint32_t numAdapters;
    gsInstanceEnumerateAdapters(instance, &numAdapters, nullptr);
    std::vector<GsAdapter> adapters(numAdapters);
    gsInstanceEnumerateAdapters(instance, &numAdapters, adapters.data());

    for (const auto& adapter : adapters)
    {
        std::cout << adapter.name << std::endl;
    }

    return 0;
}