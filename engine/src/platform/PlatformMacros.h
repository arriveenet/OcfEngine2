#pragma once
#include <stdint.h>
#include <string.h>

#define OCF_SAFE_DELETE(p) \
    do                     \
    {                      \
        delete (p);        \
        (p) = nullptr;     \
    } while (0)

#define OCF_SAFE_DELETE_ARRAY(p) \
    do                           \
    {                            \
        if (p)                   \
        {                        \
            delete[](p);         \
            (p) = nullptr;       \
        }                        \
    } while (0)

#define OCF_SAFE_RELEASE(p) \
    do                      \
    {                       \
        if(p)               \
        {                   \
            (p)->release(); \
        }                   \
    } while (0);            \

#define OCF_SAFE_RETAIN(p)  \
    do                      \
    {                       \
        if(p)               \
        {                   \
            (p)->retain();  \
        }                   \
    } while (0);            \

#define OCF_SAFE_FREE(p)  \
    do                    \
    {                     \
        free(p);          \
        (p) = nullptr;    \
    } while (0)

#define OCF_BREAK_IF(cond) \
    if (cond)              \
    break

