#include "arch/i686/a20.h"
#include "third_party/printf/printf.h"
#include "arch/i686/gdt.h"
#include "arch/i686/idt.h"
#include "arch/i686/pic.h"
#include "drivers/vga_fb.h"
#include "drivers/serial.h"
#include "drivers/i8042.h"
#include "services/interrupt.h"
#include "services/timer.h"
#include "services/log.h"

#if defined(__linux__)
#error "You should be building the codebase for bare metal, not linux!"
#endif

#if !defined(__i386__)
#error "This should be compiled with elf-i386 compiler"
#endif

_Noreturn __attribute__((unused)) void kernel_main()
{
    terminal_initialize();

    printf("\n");

    if (!serial_init()) {
        logf("COM1 initialized!\n");
    } else {
        logf_warn("COM1 not found\n");
    }

    logf("Hello, World!\n");
    if (check_a20())
    {
        logf("A20 line found!\n");
    }
    gdt_init();
    gdt_prettyprint();

    i8042_port1_disable();
    i8042_port2_disable();

    PIC_remap(32, 40);

    irq_service_init();
    timer_service_init();

    idt_init();

    logf_warn("Testing warning...\n");
    logf_err("Testing error...\n");

    i8042_init();

    while (1)
    {
    }
}
