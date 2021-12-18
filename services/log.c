#include "log.h"
#include "drivers/serial.h"
#include "drivers/tty.h"
#include "third_party/printf/printf.h"

void logf(char *format, ...) {
    va_list va;
    va_start(va, format);
    char buf[100]= {};
    vsnprintf(buf, 100, format, va);
    terminal_writestring(buf);
    serial_write_str(buf);
    va_end(va);
}