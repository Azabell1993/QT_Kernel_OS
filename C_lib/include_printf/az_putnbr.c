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

void az_putnbr(int n)
{
	unsigned int i;

	i = 0;
	if (n >= 0)
	{
		i = n;
	}

	if (n < 0)
	{
		i = n * -1;
		kernel_putchar('-');
	}

	if (i >= 10)
	{
		az_putnbr(i / 10);
	}

	kernel_putchar((i % 10) + '0');
}
