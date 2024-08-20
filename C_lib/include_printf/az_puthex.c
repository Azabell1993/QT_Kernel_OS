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

void az_puthex(unsigned int n)
{
	char c;
	char *map;

	map = "0123456789abcdef";
	if (n)
	{
		c = (char)(n & 0xF);
		n = (n >> 4);
		az_puthex(n);
		az_putchar(map[(int)c]);
	}
}

/*
 void	az_puthex(uint64_t nb)
{
	char	hex[16];
	int		tmp;
	int		i;

	az_bzero(hex, 16);
	i = 0;
	while (nb)
	{
		tmp = nb % 16;
		hex[i] = (tmp + (tmp >= 10 ? ('A' - 10) : '0'));
		nb /= 16;
		i++;
	}
	az_putstr("0x");
	while (i >= 0)
	{
		az_putchar(hex[i]);
		i--;
	}
}
*/
