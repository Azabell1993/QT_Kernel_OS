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

size_t az_strlen(const char *s)
{
	size_t i;

	i = 0;

	if (s[i] == '\0' || az_isprint(s[i]) == 0)
		return (0);

	while (s[i] != '\0' && az_isprint(s[i]))
		i++;

	return (i);
}
