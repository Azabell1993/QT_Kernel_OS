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

void kernel_putchar(char c)
{
    write(1, &c, 1);
}


// 인라인 어셈블리언어.
// 인텔전용 어셈블리 문법임.
// 맥에선 돌아가지 않기 때문에 이 부분은 주석 처리.
// ---- Intel Assembly
// -----------------------------------------------------------
// void az_putchar(char c)
// {
//     int dword_char;
//     dword_char = (char)c;

//     asm volatile(
//         ".intel_syntax noprefix;"
//         "mov r10,rsp;"  // save rsp.
//         "sub rsp,8;"    // space for buffer, align by 8.
//         "mov [rsp],al;" // store the character into buffer.
//         "mov edi,1;"    // STDOUT.
//         "mov rsi,rsp;"  // pointer to buffer.
//         "mov edx,1;"    // string length in bytes.
//         "mov eax,1;"    // WRITE.
//         "syscall;"      // clobbers rcx & r11.
//         "mov rsp,r10;"  // restore rsp.
//         ".att_syntax prefix;"
//         /* outputs */
//         :
//         /* inputs: eax */
//         : "b"(dword_char)
//         /* clobbered regs */
//         : "rcx", "rdx", "rsi", "rdi", "r10", "r11");
// }
