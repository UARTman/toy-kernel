#pragma once

struct __attribute__((__packed__)) gdt_descriptor
{
    unsigned int size : 16;
    unsigned int offset : 32;
};

struct __attribute__((__packed__)) gdt_segment_descriptor 
{
    unsigned int limit_0_15 : 16;
    unsigned int base_0_15 : 16;
    unsigned int base_16_23 : 8;
    unsigned int access_byte : 8;
    unsigned int limit_16_19 : 4;
    unsigned int flags : 4;
    unsigned int base_24_31: 8;
};

struct __attribute__((__packed__)) gdt_segment_info
{
    unsigned int limit;
    unsigned int base;
    unsigned int flags;
    unsigned char access_byte;
};

struct gdt_descriptor gdt_get();

struct gdt_segment_info gdt_segment_decode(struct gdt_segment_descriptor descriptor);

void gdt_init();

void gdt_prettyprint();