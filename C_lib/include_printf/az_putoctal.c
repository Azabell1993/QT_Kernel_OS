// kernel_lib_printf_functions
/*
 * Kernel Lib Printf Functions
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include "kernel_pr_he.h"

void az_putoctal(int n)
{
	if (n < 0)
	{
		kernel_putchar('-');
		az_putnbr(-n);
	}
	else if (n > 9)
	{
		az_putoctal(n / 8);
		az_putoctal(n % 8);
	}
	else
		kernel_putchar(n + '0');
}
