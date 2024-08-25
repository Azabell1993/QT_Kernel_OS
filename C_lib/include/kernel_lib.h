// kernel_lib.h
/*
 * Kernel Lib
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#ifndef KERNEL_LIB_H
#define KERNEL_LIB_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif
    bool kernel_create_process(const char *process_name);
    void kernel_list_processes();
    bool kernel_kill_process(const char *process_name);
    void register_print_function(void (*print_function)(const char *str));

    // 새로운 az_printf 함수 선언
    void az_printf(const char *format, ...);

    // cmd창의 printf 함수 구현
    int kernel_printf(const char *format, ...);
#ifdef __cplusplus
}
#endif

#endif
