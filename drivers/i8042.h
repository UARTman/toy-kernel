#pragma once

struct i8042_status_register {
    unsigned output_buffer_full: 1;
    unsigned input_buffer_full: 1;
    unsigned system_flag: 1;
    unsigned command_data: 1;
    unsigned unknown_bit4: 1;
    unsigned unknown_bit5: 1;
    unsigned timeout_error: 1;
    unsigned parity_error: 1;
} __attribute__((packed));

struct i8042_config {
    unsigned port1_interrupt: 1;
    unsigned port2_interrupt: 1;
    unsigned system_flag: 1;
    unsigned zero_bit3: 1;
    unsigned port1_clock_disabled: 1;
    unsigned port2_clock_disabled: 1;
    unsigned port1_translation: 1;
    unsigned zero_bit7: 1;
} __attribute__((packed));

struct i8042_status_register i8042_status();

struct i8042_config i8042_config_read();

void i8042_port1_disable();

void i8042_port1_enable();

void i8042_port2_disable();

void i8042_port2_enable();

int i8042_init();