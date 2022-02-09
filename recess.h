// @NOTE: It is not recommended to use this header, as recess is an *optional* dynamic library.
//        Making a program rely on this header will cause very weird errors when the dynamic linker cannot find the library.
//        The header is only provided for convenience/testing purposes.

#include <stdbool.h>

/*
 *  Stop recess from making libcalls fail.
 */
extern bool g_recess_suppressed;