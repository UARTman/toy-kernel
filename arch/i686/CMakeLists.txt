target_sources(kernel PUBLIC boot.s a20.s a20.h gdt.c gdt.h idt.h idt.c pic.c pic.h
            interrupt_handlers.c interrupt_handlers.h interrupt_frame.h helpers.h)

add_library(ihandler OBJECT isr.c isr.h)
target_compile_options(ihandler PRIVATE -mgeneral-regs-only)
target_link_libraries(kernel PUBLIC ihandler)