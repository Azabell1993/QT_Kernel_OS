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

char *az_strsub(char const *s, unsigned int start, size_t len)
{
	size_t i;
	char *tmp;

	i = 0;
	tmp = az_memalloc(len + 1);

	if (tmp == NULL)
		return (NULL);

	while (i < len)
	{
		tmp[i] = s[start + i];
		i++;
	}

	tmp[i] = '\0';
	return (tmp);
}
