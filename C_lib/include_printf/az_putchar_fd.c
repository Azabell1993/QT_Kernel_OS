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
#include <unistd.h>

void az_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
