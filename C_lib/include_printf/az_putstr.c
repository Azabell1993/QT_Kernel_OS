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

void az_putstr(char const *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		kernel_putchar(s[i]);
		i++;
	}
}
