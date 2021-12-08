#include "gdt.h"
#include "stdint.h"
#include "third_party/printf/printf.h"

struct gdt_descriptor gdt_get()
{
    struct gdt_descriptor gdt;
    asm("sgdt %0"
        : "=m"(gdt)
        :
        :);
    return gdt;
}

struct gdt_segment_info gdt_segment_decode(struct gdt_segment_descriptor descriptor)
{
    struct gdt_segment_info segment_info;
    segment_info.limit = descriptor.limit_0_15 | (descriptor.limit_16_19 << 16);
    segment_info.base = descriptor.base_0_15 | (descriptor.base_16_23 << 16) | (descriptor.base_24_31 << 24);
    segment_info.flags = descriptor.flags;
    segment_info.access_byte = descriptor.access_byte;
    return segment_info;
}

struct gdt_segment_descriptor gdt_segment_encode(struct gdt_segment_info info)
{
    struct gdt_segment_descriptor descriptor;
    descriptor.limit_0_15 = info.limit & 0xFFFF;
    descriptor.limit_16_19 = (info.limit >> 16) & 0xF;

    descriptor.base_0_15 = info.base & 0xFFFF;
    descriptor.base_16_23 = (info.base >> 16) & 0xFF;
    descriptor.base_24_31 = (info.base >> 24) & 0xFF;

    descriptor.flags = info.flags;
    descriptor.access_byte = info.access_byte;
    return descriptor;
}

struct gdt_descriptor rgdt;

struct gdt_segment_descriptor gdt_table[6] = {};

// TODO: Write my own GDT initializer
void gdt_init()
{
    rgdt.size = 39;
    uint32_t offset = (uint32_t)&gdt_table;
    rgdt.offset = offset;
    struct gdt_segment_info info0 = {
        .base = 0,
        .limit = 0,
        .access_byte = 0,
        .flags = 0,
    };
    gdt_table[0] = gdt_segment_encode(info0);
    struct gdt_segment_info info1 = {
        .base = 0,
        .limit = 0xfffff,
        .access_byte = 0x9a,
        .flags = 0xc,
    };
    gdt_table[1] = gdt_segment_encode(info1);
    struct gdt_segment_info info2 = {
        .base = 0,
        .limit = 0xfffff,
        .access_byte = 0x93,
        .flags = 0xc,
    };
    gdt_table[2] = gdt_segment_encode(info2);

    struct gdt_segment_info info3 = {
        .base = 0,
        .limit = 0xffff,
        .access_byte = 0x9e,
        .flags = 0x0,
    };
    gdt_table[3] = gdt_segment_encode(info3);
    
    struct gdt_segment_info info4 = {
        .base = 0,
        .limit = 0xffff,
        .access_byte = 0x92,
        .flags = 0x0,
    };
    gdt_table[4] = gdt_segment_encode(info4);
    

    asm("lgdt %0" :"=m"(rgdt) : :);
    asm("jmp $0x08, $reload_CS; reload_CS: mov $0x10, %ax; mov %ax, %ss; mov %ax, %ds; mov %ax, %es; mov %ax, %fs; mov %ax, %gs");
}

void gdt_prettyprint()
{
    struct gdt_descriptor gdt = gdt_get();
    printf("GDT Size=%u, Offset=0x%x\n", gdt.size, gdt.offset);
    uint64_t *gdtLocation = (uint64_t *)gdt.offset;
    for (int i = 0; i < ((gdt.size / 8) + 1); i++)
    {
        uint64_t gdtEntry = gdtLocation[i];
        struct gdt_segment_descriptor d = *(struct gdt_segment_descriptor *)&gdtEntry;
        struct gdt_segment_info info = gdt_segment_decode(d);

        printf("Entry%d = 0x%016llx\nBase=0x%08x Limit=0x%05x Flags=0x%x Access Byte=0x%02x\n",
               i, gdtEntry, info.base, info.limit, info.flags, info.access_byte);
    }
}