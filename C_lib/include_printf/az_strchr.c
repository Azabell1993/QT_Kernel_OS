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

char *az_strchr(const char *s, int c)
{
	char *tmp;
	int i;
	int track;

	tmp = (char *)malloc(sizeof(char) * az_strlen(s));
	i = 0;
	track = 0;
	if ((char)s[i] == '\0')
		return (NULL);
	while (s[i] != '\0' && s[i] != c)
		i++;
	if (s[i] != '\0')
		i = i - 1;
	while (s[i] != '\0')
	{
		tmp[track] = (char)s[i];
		i++;
		track++;
	}
	tmp[track] = '\0';
	if (track == 0)
		return (NULL);
	return (tmp);
}
