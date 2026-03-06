#include "src/app/app_runtime.hpp"

int main(int argc, char *argv[])
{
    AppRuntime runtime(argc, argv);
    return runtime.run();
}
