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

void az_putunsigned(unsigned int n)
{
	if (n > 9)
	{
		az_putunsigned(n / 10);
		az_putunsigned(n % 10);
	}
	else
		kernel_putchar(n + '0');
}
