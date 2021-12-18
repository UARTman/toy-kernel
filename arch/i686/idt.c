#include "idt.h"
#include "pic.h"
#include "isr.h"
#include "services/log.h"
// #include "generated.h"

// extern void isr_32_handler();

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

idt_descriptor_t idt_isr(void(*ptr))
{
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

    idt_table[0] = idt_isr(&isr_0_handler);
    idt_table[1] = idt_isr(&isr_1_handler);
    idt_table[2] = idt_isr(&isr_2_handler);
    idt_table[3] = idt_isr(&isr_3_handler);
    idt_table[4] = idt_isr(&isr_4_handler);
    idt_table[5] = idt_isr(&isr_5_handler);
    idt_table[6] = idt_isr(&isr_6_handler);
    idt_table[7] = idt_isr(&isr_7_handler);
    idt_table[8] = idt_isr(&isr_8_handler);
    idt_table[9] = idt_isr(&isr_9_handler);
    idt_table[10] = idt_isr(&isr_10_handler);
    idt_table[11] = idt_isr(&isr_11_handler);
    idt_table[12] = idt_isr(&isr_12_handler);
    idt_table[13] = idt_isr(&isr_13_handler);
    idt_table[14] = idt_isr(&isr_14_handler);
    idt_table[15] = idt_isr(&isr_15_handler);
    idt_table[16] = idt_isr(&isr_16_handler);
    idt_table[17] = idt_isr(&isr_17_handler);
    idt_table[18] = idt_isr(&isr_18_handler);
    idt_table[19] = idt_isr(&isr_19_handler);
    idt_table[20] = idt_isr(&isr_20_handler);
    idt_table[21] = idt_isr(&isr_21_handler);
    idt_table[22] = idt_isr(&isr_22_handler);
    idt_table[23] = idt_isr(&isr_23_handler);
    idt_table[24] = idt_isr(&isr_24_handler);
    idt_table[25] = idt_isr(&isr_25_handler);
    idt_table[26] = idt_isr(&isr_26_handler);
    idt_table[27] = idt_isr(&isr_27_handler);
    idt_table[28] = idt_isr(&isr_28_handler);
    idt_table[29] = idt_isr(&isr_29_handler);
    idt_table[30] = idt_isr(&isr_30_handler);
    idt_table[31] = idt_isr(&isr_31_handler);
    idt_table[32] = idt_isr(&isr_32_handler);
    idt_table[33] = idt_isr(&isr_33_handler);
    idt_table[34] = idt_isr(&isr_34_handler);
    idt_table[35] = idt_isr(&isr_35_handler);
    idt_table[36] = idt_isr(&isr_36_handler);
    idt_table[37] = idt_isr(&isr_37_handler);
    idt_table[38] = idt_isr(&isr_38_handler);
    idt_table[39] = idt_isr(&isr_39_handler);
    idt_table[40] = idt_isr(&isr_40_handler);
    idt_table[41] = idt_isr(&isr_41_handler);
    idt_table[42] = idt_isr(&isr_42_handler);
    idt_table[43] = idt_isr(&isr_43_handler);
    idt_table[44] = idt_isr(&isr_44_handler);
    idt_table[45] = idt_isr(&isr_45_handler);
    idt_table[46] = idt_isr(&isr_46_handler);
    idt_table[47] = idt_isr(&isr_47_handler);
    

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr)
                     :);
    __asm__ volatile("sti");
}

