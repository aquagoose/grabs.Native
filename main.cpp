#include <iostream>

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

    return 0;
}