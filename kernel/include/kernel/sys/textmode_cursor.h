// Файл ports.h

#ifndef _KERNEL_SYS_TEXTMODE_CURSOR_H
#define _KERNEL_SYS_TEXTMODE_CURSOR_H

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);

#endif
