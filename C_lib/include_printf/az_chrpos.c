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

int az_chrpos(const char *s, int c)
{
	int i;

	i = 0;
	while (s[i])
	{
		if ((int)s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
