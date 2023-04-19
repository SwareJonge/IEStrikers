
#include <stl/stdarg.h>
#include "nw4r/db/db_console.h"
namespace nw4r {
namespace db {
// WIP
void Assertion_Printf_(const char *text, ...) {
    va_list list;
    va_start(list, text);
    if (sAssertionConsole == nullptr)
    {
        OSVReport(text, list);
    }
    va_end(list);
}

}
}