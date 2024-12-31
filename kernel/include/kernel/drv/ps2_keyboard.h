#ifndef _KERNEL_DRV_KEYBOARD_H
#define _KERNEL_DRV_KEYBOARD_H

#include "../sys/ports.h"
#include "../sys/isr.h"

#define KEYBOARD_IRQ 1

// Определяем английскую раскладку клавиатуры
static const char keyboard_layout[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0, ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0
};

// Определяем таблицу символов для клавиш при зажатой клавише Shift
static const char shifted_keyboard_layout[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,   'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0, ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0
};


void keyboard_handler();
void ps2_keyboard_preinit();
void ps2_keyboard_init();

#endif
