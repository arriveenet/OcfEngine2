#pragma once
#include <assert.h>

#define OCF_ASSERT(expression) assert(expression)

#ifndef OCFASSERT
#define OCFASSERT(expression, msg)                                                                 \
    do {                                                                                           \
        OCF_ASSERT(expression);                                                                    \
    } while (0)
#endif // !OCFASSERT
