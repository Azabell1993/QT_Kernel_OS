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

static int az_pow(int nb, int pow)
{
	if (pow == 0)
		return (1);
	else
		return (nb * az_pow(nb, pow - 1));
}

char *az_itoa(int value, int base)
{
	int i;
	char *nbr;
	int neg;

	// 10진수
	// base = 10..

	i = 1;
	neg = 0;
	if (value < 0)
	{
		if (base == 10)
			neg = 1;
		value *= -1;
	}

	while (az_pow(base, i) - 1 < value)
		i++;

	nbr = (char *)malloc(sizeof(nbr) * i);

	nbr[i + neg] = '\0';

	while (i-- > 0)
	{
		nbr[i + neg] = (value % base) + (value % base > 9 ? 'A' - 10 : '0');
		value = value / base;
	}
	if (neg)
		nbr[0] = '-';

	return (nbr);
}
