arch_i686 = files('boot.s', 
            'a20.s', 'a20.h', 
            'gdt.c', 'gdt.h', 
            'idt.h', 'idt.c', 
            'pic.c', 'pic.h',
            'interrupt_handlers.c', 'interrupt_handlers.h',
            'interrupt_frame.h',
            'helpers.h')

# ih_generated_c = custom_target('ih_generated_c',
#     output: 'ih_output.c',
#     command: ['../../utils/generate_interrupt_handlers.py'])

interrupt_handler = static_library('interrupt_handler_tmp',
    files('isr.c', 'isr.h'), 
    c_args: '-mgeneral-regs-only', include_directories: include_directories('../..'))
kernel_objs += interrupt_handler.extract_objects('isr.c')
architectures += arch_i686