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

void az_bzero(void *s, size_t n)
{
	if (s == NULL)
		az_putstr("Error: Null input");
	az_memset(s, 0, n);
}
