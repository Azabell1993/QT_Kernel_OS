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

int az_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}
