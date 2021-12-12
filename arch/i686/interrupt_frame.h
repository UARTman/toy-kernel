#pragma once
#include <stdint.h>

struct __attribute__((__packed__)) interrupt_frame {
    uint32_t ei;
    uint32_t cs;
    uint32_t flags;
};