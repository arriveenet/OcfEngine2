// SPDX - License - Identifier : MIT
#include "AppDelegate.h"
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#   include <cstdlib>
#ifdef _WIN32
#   include <crtdbg.h>
#endif

using namespace ocf;

int main()
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    int result = EXIT_SUCCESS;

    AppDelegate app;
    result = Applicaiton::getInstance()->run();

    return result;
}