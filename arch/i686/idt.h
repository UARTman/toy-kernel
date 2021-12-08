#pragma once
#include "stdint.h"

#define IDT_GATETYPE_TASK_32 0x5
#define IDT_GATETYPE_INT_16 0x6
#define IDT_GATETYPE_TRAP_16 0x7
#define IDT_GATETYPE_INT_32 0xE
#define IDT_GATETYPE_TRAP_32 0xF

typedef struct __attribute__((__packed__)) {
    uint16_t limit;
    uint32_t base;
} idt_header_t;

typedef union 
{
    uint8_t byte;
    struct {
        unsigned int gatetype : 4;
        unsigned int storage : 1;
        unsigned int privilege_level : 2;
        unsigned int present: 1;
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

typedef struct {
    unsigned int rpl_privilege_level: 2;
    unsigned int table_indicator:1;
    unsigned int segment:13;
} idt_selector_t;

typedef struct
{
    uint32_t offset;
    idt_selector_t selector;
    idt_type_attribute_t type_attr;
} idt_descriptor_info_t;

idt_descriptor_t idt_descriptor_encode(idt_descriptor_info_t info);
idt_descriptor_info_t idt_descriptor_decode(idt_descriptor_t descript);

void idt_init();

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void isr48();