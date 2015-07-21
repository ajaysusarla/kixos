#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "Not using a Cross compiler!!"
#endif

#if !defined(__i686__)
#error "Need an i686-elf compiler"
#endif

/* libc */
size_t strlen(const char* str)
{
	size_t ret = 0;

	while (str[ret] != 0)
		ret++;

	return ret;
}

/* VGA access mode and constants */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Hardware text mode colour constants. */
enum vga_colour {
	COLOUR_BLACK = 0,
	COLOUR_BLUE = 1,
	COLOUR_GREEN = 2,
	COLOUR_CYAN = 3,
	COLOUR_RED = 4,
	COLOUR_MAGENTA = 5,
	COLOUR_BROWN = 6,
	COLOUR_LIGHT_GREY = 7,
	COLOUR_DARK_GREY = 8,
	COLOUR_LIGHT_BLUE = 9,
	COLOUR_LIGHT_GREEN = 10,
	COLOUR_LIGHT_CYAN = 11,
	COLOUR_LIGHT_RED = 12,
	COLOUR_LIGHT_MAGENTA = 13,
	COLOUR_LIGHT_BROWN = 14,
	COLOUR_WHITE = 15,
};

uint8_t make_colour(enum vga_colour fg, enum vga_colour bg)
{
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t colour)
{
	uint16_t c16 = c;
	uint16_t colour16 = colour;

	return c16 | colour16 << 8;
}

/* terminal setup and access functions */
size_t term_row;
size_t term_column;
uint8_t term_colour;
uint16_t *term_buff;

void term_init() {
	term_row = 0;
	term_column = 0;
	term_colour = make_colour(COLOUR_LIGHT_GREY, COLOUR_BLACK);

	term_buff = (uint16_t *)0xB8000;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t i = y * VGA_WIDTH + x;
			term_buff[i] = make_vgaentry(' ', term_colour);
		}
	}
}

void term_setcolour(uint8_t colour)
{
	term_colour = colour;
}

void term_putc_at(char c, uint8_t colour, size_t col, size_t row)
{
        const size_t i = row * VGA_WIDTH + col;
        term_buff[i] = make_vgaentry(c, colour);
}

void term_putc(char c)
{
        term_putc_at(c, term_colour, term_column, term_row);

        if (++term_column == VGA_WIDTH) {
                term_column = 0;
                if (++term_row == VGA_HEIGHT) {
                        term_row = 0;
                }
        }
}

void term_write(const char *data)
{
        size_t len = strlen(data);
        for (size_t i = 0; i < len; i++)
                term_putc(data[i]);
}

#if defined(__cplusplus)
extern "C"
#endif

void kernel_main()
{
        /* Console init */
        term_init();
        term_write("Hello, World!");
}
