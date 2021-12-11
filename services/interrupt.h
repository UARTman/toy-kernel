#pragma once

int irq_set_handler(int irq, void (*handler)(void));

int irq_unset_handler(int irq);

void irq_service_init();