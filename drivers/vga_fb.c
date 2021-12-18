/**
 * VBE Text-mode terminal driver
 *
 * Feature-complete
 */

#include "vga_fb.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

static size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_cls()
{
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(enum vga_color fg, enum vga_color bg)
{
    terminal_color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll()
{
    for (size_t y = 1; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            const size_t index_prev = (y - 1) * VGA_WIDTH + x;
            terminal_buffer[index_prev] = terminal_buffer[index];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
    }
}

void terminal_newline()
{
    terminal_column = 0;
    terminal_row += 1;
    if (terminal_row == VGA_HEIGHT)
    {
        terminal_scroll();
        terminal_row -= 1;
    }
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_newline();
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_newline();
    }
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}

void terminal_setpos(int col, int row)
{
    terminal_column = col;
    terminal_row = row;
}

void terminal_getpos(int *col, int *row)
{
    *col = terminal_column;
    *row = terminal_row;
}