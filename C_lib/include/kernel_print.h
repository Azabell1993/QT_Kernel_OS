// kernel_print.h
/*
 * Kernel Print Lib
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#ifndef STDIO_H
#define STDIO_H

#include "kernel_pr_he.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    int kernel_printf(const char *format, ...);

    char az_getflag(char flag);
    int az_default_mod(char *format);
    int az_skip(char *format, int i);
    int az_output(char *format, char flag, va_list ap, int i);
    int az_getparam(char *foramt, char flag);

    static void az_default(char *format, va_list ap);
    static void az_input(char *format, va_list ap);

    void az_mod(char *format);
    void az_fill(int fillcnt, char c);
    void az_plusflag(int d, char flag, int param, char *format);
    void az_minusflag(int d, char flag);
    void az_spaceflag(int d, char flag, int param);
    void az_zeroflag(int d, char flag);
    void az_o_support(int o, int param, char flag, char f_addon);
    void az_o_support_p2(char flag, char *fmt, char f_addon);
    void az_c_support(char c, int param, char flag, char f_addon);
    void az_speciftypeini(char *format, char specif, char flag, va_list ap);
    void az_c(va_list ap, char *format, char flag);
    void az_s(va_list ap, char *format, char flag);
    void az_p(va_list ap);
    void az_x(va_list ap, char *format, char flag);
    void az_d(va_list ap, char *format, char flag);
    void az_i(va_list ap, char *format, char flag);
    void az_o(va_list ap, char *format, char flag);
    void az_u(va_list ap, char *format, char flag);
#ifdef __cplusplus
}
#endif

#endif
