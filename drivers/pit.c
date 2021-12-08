#include "pit.h"
#include "arch/i686/idt.h"
#include "drivers/tty.h"
#include "arch/i686/helpers.h"
#include "third_party/printf/printf.h"

int seconds_elapsed = 0;
int tick_counter = 0;

void irq0_handler() {
    tick_counter += 1;
    if (tick_counter > 18) {
        tick_counter = 0;
        seconds_elapsed += 1;
        int c, r;
        terminal_getpos(&c, &r);
        terminal_setpos(0, 0);
        printf("Total seconds passed: %i\n", seconds_elapsed);
        terminal_setpos(c, r);
    }
}

void pit_init() {
    
    outb(0x43, 0x36);
    outb(0x40, 255);
    outb(0x40, 255);

    irq_sethandler(0, irq0_handler);
}
