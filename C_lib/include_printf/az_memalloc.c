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
#include <stdlib.h>

void *az_memalloc(size_t size)
{
	void *tmp;

	tmp = malloc(size);

	if (tmp == NULL)
		return (NULL);

	return (az_memset(tmp, 0, size));
}
