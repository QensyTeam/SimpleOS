#include "../../../../kernel/include/kernel/kernel.h"

#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT  0x4B
#define KEY_RIGHT 0x4D

static volatile uint8_t alt_flag = 0;
static volatile uint8_t shift_flag = 0;
static volatile uint8_t caps_lock_flag = 0;

void process_arrow_key(uint8_t arrow_key) {
    size_t row = terminal_getRow();
    size_t column = terminal_getcolumn();

    if (arrow_key == KEY_LEFT) {
        if (column > 0) {
            column--;
        } else if (row > 0) {
            row--;
            column = VGA_WIDTH - 1;
        }
    } else if (arrow_key == KEY_RIGHT) {
        if (column < VGA_WIDTH - 1) {
            column++;
        } else if (row < VGA_HEIGHT - 1) {
            row++;
            column = 0;
        }
    } else if (arrow_key == KEY_UP) {
        if (row > 0) {
            row--;
        }
    } else if (arrow_key == KEY_DOWN) {
        if (row < VGA_HEIGHT - 1) {
            row++;
        }
    }

    // Обновляем положение курсора в VGA
    update_cursor(column, row);

    // Также обновляем внутренние переменные терминала
    terminal_set_row(row);
    terminal_set_column(column);
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);

    if (((scancode & ~0x80) == 0x2A) || ((scancode & ~0x80) == 0x36)) {
        shift_flag = !(scancode & 0x80);
    } else if ((scancode & ~0x80) == 0x38) {
        alt_flag = !(scancode & 0x80);
    } else if (scancode == 0x3A) {
        caps_lock_flag = !caps_lock_flag;
    } else if (scancode == 0xE0) { // Префикс для клавиш-стрелок
        uint8_t arrow_key = inb(0x60);
        process_arrow_key(arrow_key); // Обрабатываем стрелку
    } else if (scancode < 128) {
        // Существующая обработка символов клавиатуры
        uint16_t c;
        uint16_t base = keyboard_layout[scancode];

        bool is_letter = (base >= 'a' && base <= 'z');
        
        bool shift_letters = caps_lock_flag ^ shift_flag;
        bool shift_numbers = shift_flag;

        bool shift = is_letter ? shift_letters : shift_numbers;

        if (shift) {
            c = shifted_keyboard_layout[scancode];
        } else {
            c = keyboard_layout[scancode];
        }

        if (c != 0) {
            keyboard_add_to_buffer(c); // Добавляем символ в буфер
        }
    }
}


void ps2_keyboard_preinit() {
    uint8_t stat;

    // Enable keyboard
    ps2_write(0xf4);
    stat = ps2_read();

    if (stat != 0xfa) {
        printf("Kbd error: LN %u\n", __LINE__);
        return;
    }

    // Set scancode
    ps2_write(0xf0);
    stat = ps2_read();

    if (stat != 0xfa) {
        printf("Kbd error: LN %u\n", __LINE__);
        return;
    }

    // Scancode number 0! (IDK what does it mean)
    ps2_write(0);
    stat = ps2_read();

    if (stat != 0xfa) {
        printf("Kbd error: LN %u\n", __LINE__);
        return;
    }

    // With error code PS/2 controller wants to send us scancode set number! Awwwwwww
    uint8_t scancode = ps2_read();
    (void)scancode;  // Consume it 

    ps2_write(0xf3);
    stat = ps2_read();

    if (stat != 0xfa) {
        printf("Kbd error: LN %u (rpt = %u)\n", __LINE__, stat);
        return;
    }

    ps2_in_wait_until_empty();

    outb(PS2_DATA_PORT, 0);
    stat = ps2_read();

    if (stat != 0xfa) {
        printf("Kbd error: LN %u\n", __LINE__);
        return;
    }

    uint8_t conf = ps2_read_configuration_byte();

    ps2_write_configuration_byte(conf | 0b1000001);
}

void ps2_keyboard_init() {
    install_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    terminal_writestring(" KEYBOARD - [ OK ]\n");
}
