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

int az_atoi(const char *s)
{
	int i;
	int conv;
	int res;
	int neg_chk;

	i = 0;
	conv = 0;

	neg_chk = 1;

	if (s == NULL)
		return (0);

	if (s[i] == '-')
	{
		neg_chk = -1;
		i++;
	}
	else if (s[i] == '+')
		i++;

	while (s[i] >= 48 && s[i] <= 57)
	{
		conv = conv * 10 + s[i++] - 48;
	}
	res = conv * neg_chk;
	return (res);
}
