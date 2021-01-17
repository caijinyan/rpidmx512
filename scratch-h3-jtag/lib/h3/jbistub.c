/*
 * jbistub.c
 */

#include <stdlib.h>
#include <stdbool.h>

#include "jbiport.h"

#include "h3_gpio.h"
#include "board/h3_opi_zero.h"

#define GPIO_TDO		GPIO_EXT_12
#define GPIO_TDI		GPIO_EXT_16
#define GPIO_TCK		GPIO_EXT_18
#define GPIO_TMS		GPIO_EXT_22

extern void uart0_puts(char *s);
extern int uart0_printf(const char* fmt, ...);

static bool verbose = false;

void *jbi_malloc(unsigned int size) {
	return malloc(size);
}

void jbi_free(void *ptr) {
	free(ptr);
}

/************************************************************************
*
*	Customized interface functions for Jam STAPL ByteCode Player I/O:
*
*	jbi_jtag_io()
*	jbi_message()
*	jbi_delay()
*/

void jbi_jtag_io_init(void) {
	h3_gpio_fsel(GPIO_TDO, GPIO_FSEL_INPUT);
	h3_gpio_fsel(GPIO_TDI, GPIO_FSEL_OUTPUT);
	h3_gpio_fsel(GPIO_TCK, GPIO_FSEL_OUTPUT);
	h3_gpio_fsel(GPIO_TMS, GPIO_FSEL_OUTPUT);
}

int jbi_jtag_io(int tms, int tdi, int read_tdo) {
	int tdo = 0;

	if (tdi) {
		h3_gpio_set(GPIO_TDI);
	} else {
		h3_gpio_clr(GPIO_TDI);
	}

	if (tms) {
		h3_gpio_set(GPIO_TMS);
	} else {
		h3_gpio_clr(GPIO_TMS);
	}

	if (read_tdo) {
		tdo = h3_gpio_lev(GPIO_TDO);
	}

	h3_gpio_set(GPIO_TCK);
	h3_gpio_clr(GPIO_TCK);

	return tdo;
}

void jbi_message(char *message_text) {
	uart0_puts(message_text);
	uart0_puts("\n");
}

void jbi_delay(long microseconds) {
	udelay(microseconds);
}

void jbi_export_integer(char *key, long value) {
	if (verbose) {
		uart0_printf("Export: key = \"%s\", value = %ld\n", key, value);
	}
}

#define HEX_LINE_CHARS 72
#define HEX_LINE_BITS (HEX_LINE_CHARS * 4)

char conv_to_hex(unsigned long value) {
	char c;

	if (value > 9) {
		c = (char) (value + ('A' - 10));
	} else {
		c = (char) (value + '0');
	}

	return (c);
}

void jbi_export_boolean_array(char *key, unsigned char *data, long count) {
	char string[HEX_LINE_CHARS + 1];
	long i, offset;
	unsigned long size, line, lines, linebits, value, j, k;

	if (verbose) {
		if (count > HEX_LINE_BITS) {
			uart0_printf("Export: key = \"%s\", %ld bits, value = HEX\n", key, count);
			lines = (count + (HEX_LINE_BITS - 1)) / HEX_LINE_BITS;

			for (line = 0; line < lines; ++line) {
				if (line < (lines - 1)) {
					linebits = HEX_LINE_BITS;
					size = HEX_LINE_CHARS;
					offset = count - ((line + 1) * HEX_LINE_BITS);
				} else {
					linebits = count - ((lines - 1) * HEX_LINE_BITS);
					size = (linebits + 3) / 4;
					offset = 0L;
				}

				string[size] = '\0';
				j = size - 1;
				value = 0;

				for (k = 0; k < linebits; ++k) {
					i = k + offset;
					if (data[i >> 3] & (1 << (i & 7)))
						value |= (1 << (i & 3));
					if ((i & 3) == 3) {
						string[j] = conv_to_hex(value);
						value = 0;
						--j;
					}
				}
				if ((k & 3) > 0)
					string[j] = conv_to_hex(value);

				uart0_printf("%s\n", string);
			}
		} else {
			size = (count + 3) / 4;
			string[size] = '\0';
			j = size - 1;
			value = 0;

			for (i = 0; i < count; ++i) {
				if (data[i >> 3] & (1 << (i & 7)))
					value |= (1 << (i & 3));
				if ((i & 3) == 3) {
					string[j] = conv_to_hex(value);
					value = 0;
					--j;
				}
			}
			if ((i & 3) > 0)
				string[j] = conv_to_hex(value);

			uart0_printf("Export: key = \"%s\", %ld bits, value = HEX %s\n", key, count, string);
		}
	}
}
