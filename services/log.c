#include "log.h"
#include "drivers/serial.h"
#include "drivers/tty.h"
#include "third_party/printf/printf.h"

#define LOG_BUF_SIZE 100

void vlogf(char *format, va_list va) {
    char buf[LOG_BUF_SIZE]= {};
    vsnprintf(buf, LOG_BUF_SIZE, format, va);
    terminal_writestring(buf);
    serial_write_str(buf);
}

void logf(char *format, ...) {
    va_list va;
    va_start(va, format);
    vlogf(format, va);
    va_end(va);
}

void warning_logf(char *format, ...) {
    va_list va;
    va_start(va, format);
    terminal_setcolor(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    serial_write_str("\033[33m");
    vlogf(format, va);
    terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    serial_write_str("\033[m");
    va_end(va);
}

void error_logf(char *format, ...) {
    va_list va;
    va_start(va, format);
    terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    serial_write_str("\033[31m");
    vlogf(format, va);
    terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    serial_write_str("\033[m");
    va_end(va);
}