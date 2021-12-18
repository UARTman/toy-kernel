#include "pit.h"
#include "arch/i686/idt.h"
#include "arch/i686/helpers.h"

void pit_init()
{

    outb(0x43, 0x36);
    outb(0x40, 255);
    outb(0x40, 255);
}
