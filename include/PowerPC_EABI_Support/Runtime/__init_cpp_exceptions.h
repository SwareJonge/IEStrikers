#ifndef RUNTIME_CPP_EXCEPTIONS_H
#define RUNTIME_CPP_EXCEPTIONS_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void __init_cpp_exceptions(void);
    void __fini_cpp_exceptions(void);

#ifdef __cplusplus
}
#endif

#endif 