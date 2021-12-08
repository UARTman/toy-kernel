#include "idt.h"
#include "third_party/printf/printf.h"
#include "pic.h"

idt_descriptor_t idt_descriptor_encode(idt_descriptor_info_t info)
{
    idt_descriptor_t descriptor;
    descriptor.offset_0_15 = info.offset & 0xFFFF;
    descriptor.offset_16_31 = (info.offset >> 16) & 0xFFFF;

    descriptor.zero = 0;
    descriptor.type_attr = info.type_attr.byte;
    descriptor.selector = *(uint16_t *)(&info.selector);

    return descriptor;
};

idt_descriptor_info_t idt_descriptor_decode(idt_descriptor_t descriptor)
{
    idt_descriptor_info_t info;
    info.offset = descriptor.offset_0_15 | (descriptor.offset_16_31 << 16);
    uint16_t selector = descriptor.selector;
    info.selector = *(idt_selector_t *)&selector;
    info.type_attr.byte = descriptor.type_attr;
    return info;
};

idt_header_t idtr;
idt_descriptor_t idt_table[256];

typedef struct __attribute__((__packed__)) registers {
    uint32_t ds;
    // Pushed by pusha.
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    // Pushed by the processor automatically.
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/* BLACK MAGIC – strongly discouraged! */
void interrupt_handler()
{
    __asm__("pushal");

    printf("Interrupt!\n");
    __asm__("cli");
    while (1)
    {
        asm("hlt");
    }
    /* do something */
    __asm__("popal; leave; iret"); /* BLACK MAGIC! */
}

idt_descriptor_t idt_isr(void(*ptr)) {
    idt_descriptor_info_t info = {
        .offset = (uint32_t)ptr,
        .selector = {
            .rpl_privilege_level = 0,
            .table_indicator = 0,
            .segment = 1,
        },
        .type_attr = {
            .info = {
                .gatetype = IDT_GATETYPE_TRAP_32,
                .present = 1,
                .privilege_level = 0,
                .storage = 0,
            },
        }};
    return idt_descriptor_encode(info);
}

void idt_init()
{
    idtr.limit = 256 * sizeof(idt_descriptor_t) - 1;
    idtr.base = (uint32_t)&idt_table;

    idt_table[0] = idt_isr(&isr0);
    idt_table[1] = idt_isr(&isr1);
    idt_table[2] = idt_isr(&isr2);
    idt_table[3] = idt_isr(&isr3);
    idt_table[4] = idt_isr(&isr4);
    idt_table[5] = idt_isr(&isr5);
    idt_table[6] = idt_isr(&isr6);
    idt_table[7] = idt_isr(&isr7);
    idt_table[8] = idt_isr(&isr8);
    idt_table[9] = idt_isr(&isr9);
    idt_table[10] = idt_isr(&isr10);
    idt_table[11] = idt_isr(&isr11);
    idt_table[12] = idt_isr(&isr12);
    idt_table[13] = idt_isr(&isr13);
    idt_table[14] = idt_isr(&isr14);
    idt_table[15] = idt_isr(&isr15);
    idt_table[16] = idt_isr(&isr16);
    idt_table[17] = idt_isr(&isr17);
    idt_table[18] = idt_isr(&isr18);
    idt_table[19] = idt_isr(&isr19);
    idt_table[20] = idt_isr(&isr20);
    idt_table[21] = idt_isr(&isr21);
    idt_table[22] = idt_isr(&isr22);
    idt_table[23] = idt_isr(&isr23);
    idt_table[24] = idt_isr(&isr24);
    idt_table[25] = idt_isr(&isr25);
    idt_table[26] = idt_isr(&isr26);
    idt_table[27] = idt_isr(&isr27);
    idt_table[28] = idt_isr(&isr28);
    idt_table[29] = idt_isr(&isr29);
    idt_table[30] = idt_isr(&isr30);
    idt_table[31] = idt_isr(&isr31);
    idt_table[32] = idt_isr(&isr32);
    idt_table[33] = idt_isr(&isr33);
    idt_table[34] = idt_isr(&isr34);
    idt_table[35] = idt_isr(&isr35);
    idt_table[36] = idt_isr(&isr36);
    idt_table[37] = idt_isr(&isr37);
    idt_table[38] = idt_isr(&isr38);
    idt_table[39] = idt_isr(&isr39);
    idt_table[40] = idt_isr(&isr40);
    idt_table[41] = idt_isr(&isr41);
    idt_table[42] = idt_isr(&isr42);
    idt_table[43] = idt_isr(&isr43);
    idt_table[44] = idt_isr(&isr44);
    idt_table[45] = idt_isr(&isr45);
    idt_table[46] = idt_isr(&isr46);
    idt_table[47] = idt_isr(&isr47);
    idt_table[48] = idt_isr(&isr48);

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr)
                     :);
    __asm__ volatile("sti");
}

void _panic() {
    printf("Kernel panic!\n");
    while (1) {
        asm("hlt");
    }
}



void (*irq_handlers[16])(void) = {};

void __attribute__((__cdecl)) isr_handler(registers_t regs)
{
    if (regs.int_no < 32) {
        printf("Interrupt %i. Error code %x\n", regs.int_no, regs.err_code);
        _panic();
    } else if (regs.int_no >= 32 && regs.int_no <= 40) {
        uint32_t pic_irq = regs.int_no - 32;
        if (irq_handlers[pic_irq] != 0) {
            irq_handlers[pic_irq]();
        } else {
            printf("Interrupt %i (PIC IRQ %i). Error code %x\n", regs.int_no, pic_irq, regs.err_code);
        };

        PIC_sendEOI(pic_irq);
    }  
}

void irq_sethandler(int irq, void (*handler)(void)) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_unsethandler(int irq) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = 0;
    }
}
