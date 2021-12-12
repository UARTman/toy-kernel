#include "interrupt_handlers.h"
#include "third_party/printf/printf.h"

void (*isr_handlers[256])(struct interrupt_frame *, unsigned int int_no, unsigned int err_code) = {};

void _panic()
{
    printf("Kernel panic!\n");
    asm("cli");
    while (1)
    {
        asm("hlt");
    }
}


void isr_handler(struct interrupt_frame *frame, unsigned int int_no, unsigned int err_code)
{
    if (isr_handlers[int_no])
    {
        isr_handlers[int_no](frame, int_no, err_code);
        return;
    }

    printf("Unhandled Interrupt %i. Error code %x\n", int_no, err_code);
    _panic();
}

int isr_set_handler(int isr, void (*handler)(struct interrupt_frame *frame, unsigned int int_no, unsigned int err_code))
{
    if (isr < 0 || isr > 255)
    {
        return -1;
    }
    if (isr_handlers[isr])
    {
        return -2;
    }
    isr_handlers[isr] = handler;
    return 0;
}

int isr_unset_handler(int isr)
{
    if (isr < 0 || isr > 255)
    {
        return -1;
    }
    if (!isr_handlers[isr])
    {
        return -2;
    }
    isr_handlers[isr] = 0;
    return 0;
}