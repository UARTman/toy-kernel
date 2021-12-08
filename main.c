#include "drivers/tty.h"
#include "arch/i686/a20.h"
#include "third_party/printf/printf.h"
#include "arch/i686/gdt.h"
#include "arch/i686/idt.h"
#include "arch/i686/pic.h"
#include "stdint.h"

#if defined(__linux__)
#warning "You should be building the codebase for bare metal, not linux!"
#endif

#if !defined(__i386__)
#warning "This should be compiled with elf-i386 compiler"
#endif

int x = 10;


__attribute__((unused)) void kernel_main()
{
    terminal_initialize();
    printf("Hello, World!\n");
    if (check_a20())
    {
        printf("A20 line found!\n");
    }
    // gdt_init();
    gdt_prettyprint();
    idt_header_t header;
    // uint8_t bytes [8];

    idt_init();
    PIC_remap(32, 40);
    while (1) {
        
    }
    // __asm__ volatile ("sidt %0" : "=m"(header) ::);
    // printf("Bytes: 0x%x", bytes);
    // printf("IDT Limit: %d Offset: 0x%lx\n", header.limit, header.base);
    // asm ("int $6");
    // x = x / 0;
    
    // printf("After interrupt, x = %i\n", x);
}
