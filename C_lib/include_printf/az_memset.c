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

void *az_memset(void *b, int c, size_t len)
{
	unsigned char *s;
	size_t i;

	i = 0;
	if (len == 0)
		return (b);

	s = b;

	while (i < len)
	{
		s[i] = (unsigned char)c;
		i++;
	}

	return (b);
}
