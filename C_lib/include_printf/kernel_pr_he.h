// kernel_lib_printf_functions
/*
 * Kernel Lib Printf Functions
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#ifndef STDIO_H
#define STDIO_H
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __cplusplus

extern "C"
{
#endif
    void az_putoctal(int n);
    void az_putunsigned(unsigned int n);
    void *az_memalloc(size_t size);
    int az_isdigit(int c);
    int az_isalpha(int c);
    char *az_itoa(int value, int base);
    int az_chrpos(const char *s, int c);
    char *az_strsub(char const *s, unsigned int start, size_t len);
    size_t az_nbrlen(int n);
    int az_isprint(int c);
    size_t az_strlen(const char *s);
    void az_puthex(unsigned int n);
    void az_putnbr(int n);
    int az_chrpos(const char *s, int c);
    void *az_memset(void *b, int c, size_t len);
    void az_bzero(void *s, size_t n);
    void az_putchar_fd(char c, int fd);
    void az_putchar(char c);
    void az_putstr(const char *s);
    int az_atoi(const char *s);
    int az_strcmp(const char *s1, const char *s2);
    char *az_strchr(const char *s, int c);
#ifdef __cplusplus
}
#endif
#endif
