/*
 * Platform specific ties to generic uart functions that putc expects.
 *
 * Copyright (C) 2007 Bahadir Balban
 *
 */

#ifndef __PLATFORM_EB_UART_H__
#define __PLATFORM_EB_UART_H__

#include INC_PLAT(offsets.h)
#include INC_GLUE(memlayout.h)

#define PLATFORM_CONSOLE_BASE			EB_UART0_VBASE
#include <l4/drivers/uart/pl011/pl011_uart.h>

void uart_init(void);
void uart_putc(char c);

#endif /* __PLATFORM_EB_UART_H__ */
