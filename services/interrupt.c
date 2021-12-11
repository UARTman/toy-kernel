#include "interrupt.h"
#include "arch/i686/idt.h"
#include "arch/i686/pic.h"
#include "third_party/printf/printf.h"

void (*irq_handlers[16])(void) = {};

int irq_set_handler(int irq, void (*handler)(void)) {
    if (irq < 0 || irq > 15) {
        return -1;
    }
    if (irq_handlers[irq]) {
        return -2;
    }
    irq_handlers[irq] = handler;
    return 0;
}

int irq_unset_handler(int irq) {
    if (irq < 0 || irq > 15) {
        return -1;
    }
    if (!irq_handlers[irq]) {
        return -2;
    }
    irq_handlers[irq] = 0;
    return 0;
}

void irq_handler(registers_t regs)
{
    uint32_t pic_irq = regs.int_no - 32;
    if (irq_handlers[pic_irq] != 0)
    {
        irq_handlers[pic_irq]();
    }
    else
    {
        printf("Interrupt %i (PIC IRQ %i). Error code %x\n", regs.int_no, pic_irq, regs.err_code);
    };

    PIC_sendEOI(pic_irq);
}

void irq_service_init()
{
    for (int i = 32; i < 40; i++)
    {
        isr_set_handler(i, irq_handler);
    }
    
}