#include "i8042.h"
#include "arch/i686/helpers.h"
#include "services/log.h"
#include "services/timer.h"
#include "services/interrupt.h"

struct i8042_status_register i8042_status() {
    uint8_t status = inb(0x64);
    struct i8042_status_register ret = *((struct i8042_status_register *) &status);
    return ret;
}

uint8_t i8042_data_read_nonblocking() {
    return inb(0x60);
}

void i8042_data_write_nonblocking(uint8_t data) {
    outb(0x60, data);
}

void i8042_data_write_blocking(uint8_t byte) {
    while(i8042_status().input_buffer_full) {
    }
    i8042_data_write_nonblocking(byte);
}

uint8_t i8042_data_read_blocking() {
    while(!i8042_status().output_buffer_full) {}
    uint8_t byte = i8042_data_read_nonblocking();
    return byte;
}

int i8042_data_read_timeout(uint8_t *dest, int timeout_seconds) {
    int seconds_start = timer_service_seconds_elapsed();
    while(!i8042_status().output_buffer_full) {
        if (timer_service_seconds_elapsed() - seconds_start >= timeout_seconds) {
            return 1;
        }
    }
    *dest = i8042_data_read_nonblocking();
    return 0;
}

int i8042_data_write_timeout(uint8_t byte, int timeout_seconds) {
    int seconds_start = timer_service_seconds_elapsed();
    while(!i8042_status().input_buffer_full) {
        if (timer_service_seconds_elapsed() - seconds_start >= timeout_seconds) {
            return 1;
        }
    }
    i8042_data_write_nonblocking(byte);
    return 0;
}

void i8042_cmd_write(uint8_t cmd) {
    outb(0x64, cmd);
}

struct i8042_config i8042_config_read() {
    i8042_cmd_write(0x20);
    uint8_t byte = i8042_data_read_blocking();
    struct i8042_config config = *((struct i8042_config *) &byte);
    return config;
}

void i8042_config_write(struct i8042_config config) {
    uint8_t byte = *((uint8_t *)&config);
    i8042_cmd_write(0x60);
    i8042_data_write_blocking(byte);
}

void i8042_port1_disable() {
    i8042_cmd_write(0xAD);
}

void i8042_port1_enable() {
    i8042_cmd_write(0xAE);
}

void i8042_port2_disable() {
    i8042_cmd_write(0xA7);
}

void i8042_port2_enable() {
    i8042_cmd_write(0xA8);
}

int port1_present = 0;
int port2_present = 0;

void i8042_port1_irq() {
    uint8_t data = i8042_data_read_blocking();
    logf("PS2 device 1 sent data: 0x%02X\n", data);
}

void i8042_port2_irq() {
    uint8_t data = i8042_data_read_blocking();
    logf("PS2 device 2 sent data: 0x%02X\n", data);
}

int i8042_init() {
    // Disable devices
    i8042_port1_disable();
    i8042_port2_disable();
    // Clear the buffer
    i8042_data_read_nonblocking();
    // Disable IRQs and translation
    struct i8042_config cfg = i8042_config_read();
    cfg.port1_interrupt = 0;
    cfg.port2_interrupt = 0;
    cfg.port1_translation = 0;
    i8042_config_write(cfg);
    // Run the self-test
    i8042_cmd_write(0xAA); // Send the self-test command to the controller.
    uint8_t self_test_response = i8042_data_read_blocking();
    if (self_test_response != 0x55) {
        logf_err("i8042 controller failed self-test (returned 0x%02X)!\n", self_test_response);
        return 1;
    }
    // Restore config in case the controller resets
    i8042_config_write(cfg);
    // Test for second port
    i8042_port2_enable();
    port2_present = !i8042_config_read().port2_clock_disabled;
    i8042_port2_disable();
    if (!port2_present) {
        logf("PS2 port 2 not present!\n");
    }
    // Test the ports
    i8042_cmd_write(0xAB); // Test the first port
    port1_present = 0;
    int port1_test_result = i8042_data_read_blocking();
    switch (port1_test_result) {
        case 0x00:
            port1_present = 1;
            break;
        case 0x01:
            logf_warn("PS2 port 1 failed test: clock line stuck low\n");
            break;
        case 0x02:
            logf_warn("PS2 port 1 failed test: clock line stuck high\n");
            break;
        case 0x03:
            logf_warn("PS2 port 1 failed test: data line stuck low\n");
            break;
        case 0x04:
            logf_warn("PS2 port 1 failed test: data line stuck high\n");
            break;
        default:
            logf_warn("PS2 port 1 test returned an invalid answer: %02X\n", port1_test_result);
    }
    if (port2_present) {
        i8042_cmd_write(0xA9); // Test the second port (if present)
        port2_present = 0;
        int port2_test_result = i8042_data_read_blocking();
        switch (port2_test_result) {
            case 0x00:
                port2_present = 1;
                break;
            case 0x01:
                logf_warn("PS2 port 2 failed test: clock line stuck low\n");
                break;
            case 0x02:
                logf_warn("PS2 port 2 failed test: clock line stuck high\n");
                break;
            case 0x03:
                logf_warn("PS2 port 2 failed test: data line stuck low\n");
                break;
            case 0x04:
                logf_warn("PS2 port 2 failed test: data line stuck high\n");
                break;
            default:
                logf_warn("PS2 port 2 test returned an invalid answer: %02X\n", port1_test_result);
        }
    }
    // Enable devices (and IRQs)
    cfg.port1_interrupt = 1;
    cfg.port2_interrupt = 1;
    i8042_config_write(cfg);
    if (port1_present) {
        i8042_port1_enable();
    }
    if (port2_present) {
        i8042_port2_enable();
    }
    // Reset devices
    if (port1_present) {
        i8042_data_write_blocking(0xFF);
        uint8_t ack = i8042_data_read_blocking();
        if (ack != 0xFA) {
            logf_warn("Device on port1 didn't acknowledge reset, returned %02X.\n", ack);
        }
        uint8_t test_result = i8042_data_read_blocking();
        if (test_result != 0xAA) {
            logf_warn("Device on port1 failed self-test, returned %02X.\n", test_result);
        }
        uint8_t dev_id;
        int timeout = i8042_data_read_timeout(&dev_id, 1);
        if (!timeout) {
            logf("Device on port1 has device id %02X.\n", dev_id);
        }
    }
    if (port2_present) {
        i8042_cmd_write(0xD4);
        i8042_data_write_blocking(0xFF);
        uint8_t ack = i8042_data_read_blocking();
        if (ack != 0xFA) {
            logf_warn("Device on port2 didn't acknowledge reset, returned %02X.\n", ack);
        }
        uint8_t test_result = i8042_data_read_blocking();
        if (test_result != 0xAA) {
            logf_warn("Device on port2 failed self-test, returned %02X.\n", test_result);
        }
        uint8_t dev_id;
        int timeout = i8042_data_read_timeout(&dev_id, 1);
        if (!timeout) {
            logf("Device on port2 has device id %02X.\n", dev_id);
        }
    }
    // Initiate IRQ handlers
    irq_set_handler(1, i8042_port1_irq);
    irq_set_handler(2, i8042_port2_irq);
    logf("8042 PS/2 controller init complete.\n");
    return 0;
}