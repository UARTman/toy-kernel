.intel_syntax noprefix
/*
//
// interrupt.s -- Contains interrupt service routine wrappers.
//                Based on Bran's kernel development tutorials.
//                Rewritten for JamesM's kernel development tutorials.

// This macro creates a stub for an ISR which does NOT pass it's own
// error code (adds a dummy errcode byte).
*/
.macro ISR_NOERRCODE num
  .global isr\num
  isr\num:
    cli                         // Disable interrupts firstly.
    push 0                 // Push a dummy error code.
    push \num                // Push the interrupt number.
    jmp isr_common_stub         // Go to our common handler code.
.endm

// This macro creates a stub for an ISR which passes it's own
// error code.
.macro ISR_ERRCODE num
  .global isr\num
  isr\num:
    cli                         // Disable interrupts.
    push \num                // Push the interrupt number
    jmp isr_common_stub
.endm

//.global isrq
//isrq:
//    cli                         // Disable interrupts firstly.
//    push byte 0                 // Push a dummy error code.
//    push byte \num                // Push the interrupt number.
//    jmp isr_common_stub         // Go to our common handler code.

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 32
ISR_NOERRCODE 33
ISR_NOERRCODE 34
ISR_NOERRCODE 35
ISR_NOERRCODE 36
ISR_NOERRCODE 37
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47
ISR_NOERRCODE 48

// In isr.c
.extern isr_handler

// This is our common ISR stub. It saves the processor state, sets
// up for kernel mode segments, calls the C-level fault handler,
// and finally restores the stack frame.
isr_common_stub:
    pusha                    // Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               // Lower 16-bits of eax = ds.
    push eax                 // save the data segment descriptor

    mov ax, 0x10  // load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    // push eax // BEGIN Garbage hack, investigate later

    call isr_handler

    // pop ebx // END Garbage hack, investigate later

    pop ebx        // reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     // Pops edi,esi,ebp...
    add esp, 8     // Cleans up the pushed error code and pushed ISR number
    sti
    iret           // pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP