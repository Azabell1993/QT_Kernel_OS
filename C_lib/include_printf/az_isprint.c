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

int az_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
