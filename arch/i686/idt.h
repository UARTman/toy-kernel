#pragma once
#include "stdint.h"

#define IDT_GATETYPE_TASK_32 0x5
#define IDT_GATETYPE_INT_16 0x6
#define IDT_GATETYPE_TRAP_16 0x7
#define IDT_GATETYPE_INT_32 0xE
#define IDT_GATETYPE_TRAP_32 0xF

typedef struct __attribute__((__packed__))
{
    uint16_t limit;
    uint32_t base;
} idt_header_t;

typedef union
{
    uint8_t byte;
    struct
    {
        unsigned int gatetype : 4;
        unsigned int storage : 1;
        unsigned int privilege_level : 2;
        unsigned int present : 1;
    } info;
} idt_type_attribute_t;

typedef struct
{
    unsigned int offset_0_15 : 16;
    unsigned int selector : 16;
    unsigned int zero : 8;
    unsigned int type_attr : 8;
    unsigned int offset_16_31 : 16;
} idt_descriptor_t;

typedef struct
{
    unsigned int rpl_privilege_level : 2;
    unsigned int table_indicator : 1;
    unsigned int segment : 13;
} idt_selector_t;

typedef struct
{
    uint32_t offset;
    idt_selector_t selector;
    idt_type_attribute_t type_attr;
} idt_descriptor_info_t;

typedef struct __attribute__((__packed__)) registers
{
    uint32_t ds;
    // Pushed by pusha.
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    // Pushed by the processor automatically.
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

idt_descriptor_t idt_descriptor_encode(idt_descriptor_info_t info);
idt_descriptor_info_t idt_descriptor_decode(idt_descriptor_t descript);

void idt_init();

int isr_set_handler(int isr, void (*handler)(registers_t));
int isr_unset_handler(int isr);
