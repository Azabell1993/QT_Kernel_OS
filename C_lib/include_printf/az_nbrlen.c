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

size_t az_nbrlen(int n)
{
	size_t len;

	len = 0;

	if (n < 0)
		len++;

	while (n / 10 != 0)
	{
		n = n / 10;
		len++;
	}

	len++;

	return (len);
}
