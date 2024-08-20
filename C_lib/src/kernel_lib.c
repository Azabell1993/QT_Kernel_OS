// kernel_lib.c
/*
 * Kernel Lib Basic Code
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// 최대 프로세스 수 정의
#define MAX_PROCESSES 10

typedef struct
{
    char name[256];
    int running; // C에서 bool 대신 int 사용 (0: false, 1: true)
} Process;

static Process process_table[MAX_PROCESSES];
static int process_count = 0;

// Qt에서 구현된 함수 포인터를 사용해 출력
static void (*qt_print_function)(const char *str) = NULL;

void az_printf(const char *format, ...)
{
    if (qt_print_function)
    {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        qt_print_function(buffer);
    }
}

void register_print_function(void (*print_function)(const char *str))
{
    qt_print_function = print_function;
}

int kernel_create_process(const char *process_name)
{
    if (process_count >= MAX_PROCESSES)
    {
        az_printf("Error: Process table is full.\n");
        return 0; // false
    }

    // 새로운 프로세스 생성
    Process new_process;
    strncpy(new_process.name, process_name, sizeof(new_process.name) - 1);
    new_process.name[sizeof(new_process.name) - 1] = '\0';
    new_process.running = 1; // true

    // 프로세스 테이블에 추가
    process_table[process_count++] = new_process;

    az_printf("Process created: %s\n", process_name);
    return 1; // true
}

void kernel_list_processes()
{
    az_printf("Process List:\n");
    for (int i = 0; i < process_count; i++)
    {
        az_printf(" - %s (%s)\n", process_table[i].name, process_table[i].running ? "Running" : "Stopped");
    }
}

int kernel_kill_process(const char *process_name)
{
    for (int i = 0; i < process_count; i++)
    {
        if (strcmp(process_table[i].name, process_name) == 0)
        {
            process_table[i].running = 0; // false
            az_printf("Process killed: %s\n", process_name);
            return 1; // true
        }
    }
    az_printf("Error: Process not found: %s\n", process_name);
    return 0; // false
}
