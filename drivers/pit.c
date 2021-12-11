#include "pit.h"
#include "arch/i686/idt.h"
#include "drivers/tty.h"
#include "arch/i686/helpers.h"
#include "third_party/printf/printf.h"


void pit_init() {
    
    outb(0x43, 0x36);
    outb(0x40, 255);
    outb(0x40, 255);
}
