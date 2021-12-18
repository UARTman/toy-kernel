#include "arch/i686/a20.h"
#include "third_party/printf/printf.h"
#include "arch/i686/gdt.h"
#include "arch/i686/idt.h"
#include "arch/i686/pic.h"
#include "drivers/tty.h"
#include "drivers/serial.h"
#include "services/interrupt.h"
#include "services/timer.h"
#include "services/log.h"

#if defined(__linux__)
#warning "You should be building the codebase for bare metal, not linux!"
#endif

#if !defined(__i386__)
#warning "This should be compiled with elf-i386 compiler"
#endif

int x = 10;

__attribute__((unused)) void kernel_main()
{
    terminal_initialize();

    printf("\n");

    if (!serial_init()) {
        logf("COM1 initialized!\n");
    } else {
        warning_logf("COM1 not found\n");
    }

    logf("Hello, World!\n");
    if (check_a20())
    {
        logf("A20 line found!\n");
    }
    gdt_init();
    gdt_prettyprint();

    PIC_remap(32, 40);

    irq_service_init();
    timer_service_init();

    idt_init();

    warning_logf("Testing warning...\n");
    error_logf("Testing error...\n");

    while (1)
    {
    }
}
