#include "../include/kernel/kernel.h"

#define HEAP_START_ADDRESS 0x1000000

void irq_disable();
void irq_enable();

#define FRAME_WIDTH 20
#define FRAME_HEIGHT 7

void draw_frame() {
    // Верхняя граница
    terminal_set_row(7);
    for (size_t col = 0; col < FRAME_WIDTH - 1; ++col) {
        terminal_set_column(col);
        putchar(205); // Горизонтальная линия
    }
    terminal_set_column(FRAME_WIDTH - 1);
    putchar(188); // Правый верхний угол

    // Боковые границы
    for (size_t row = 0; row < FRAME_HEIGHT; ++row) {
        terminal_set_row(row);
        terminal_set_column(FRAME_WIDTH - 1);
        putchar(186); // Вертикальная линия
    }
}


void kernel_main(__attribute__((unused)) multiboot_info_t* multiboot_info) {
	terminal_initialize();
	// Рассчитываем размер кучи
    size_t heap_size = calculate_heap_size(multiboot_info);

    // Инициализируем кучу
    kheap_init((void*)HEAP_START_ADDRESS, heap_size);

    irq_disable();
    
    gdt_init();
    idt_init(GDT_CODE_SEL_1);
    
    pic_init();

    irq_enable();
	ps2_init();
    ps2_keyboard_preinit();
    ps2_keyboard_init();
	printf("\n");
	draw_frame();
	printf("\n\n\n");

	// Основной цикл ядра
    while (1) {
        // Ждем символ с клавиатуры и выводим его в терминал
        char c = keyboard_get_char();
        if (c) {
            terminal_putchar(c);
        }
    }
	
}

