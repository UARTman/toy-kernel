#include "timer.h"
#include "services/interrupt.h"
#include "drivers/pit.h"
#include "third_party/printf/printf.h"
#include "drivers/tty.h"

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

void timer_service_init() {
    pit_init();
    irq_set_handler(0, irq0_handler);
}

int timer_service_seconds_elapsed() {
    return seconds_elapsed;
}