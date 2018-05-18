#pragma once
#include <cstdio>

#ifdef KNY_MATH_DEBUG
#define LOG_DEBUG(...) do{\
    fprintf(stderr, "DEBUG: ");fprintf(stderr, __VA_ARGS__);fprintf(stderr, "\n");\
}while(false)

#define LOG_ERROR(...) do{\
    fprintf(stderr, "ERROR: ");fprintf(stderr, __VA_ARGS__);fprintf(stderr, "\n");\
}while(false)

#else
#define LOG_DEBUG(...)
#define LOG_ERROE(...)
#endif
