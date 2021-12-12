#pragma once
#include "stdint.h"
#include "interrupt_frame.h"

void isr_handler(struct interrupt_frame *frame, unsigned int int_no, unsigned int err_code);
int isr_set_handler(int isr, void (*handler)(struct interrupt_frame *, unsigned int, unsigned int));
int isr_unset_handler(int isr);