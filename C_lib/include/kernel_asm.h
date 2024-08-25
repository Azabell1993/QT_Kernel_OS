#pragma once
#ifndef KERNEL_ASM_H
#define KERNEL_ASM_H

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <memory.h>

// 컴파일러에 따른 분기 설정
#if defined(__clang__) // Clang용 설정
#if defined(__x86_64__) || defined(__ppc64__)
#define SYSTEM_BIT 64 // 64비트 시스템
#else
#define SYSTEM_BIT 32 // 32비트 시스템
#endif
#elif defined(__GNUC__) // GCC용 설정
#if defined(__x86_64__) || defined(__ppc64__)
#define SYSTEM_BIT 64 // 64비트 시스템
#else
#define SYSTEM_BIT 32 // 32비트 시스템
#endif
#else
#error "Unsupported compiler"
#endif

typedef const char *STRING;       // STRING 타입을 const char*로 정의
typedef unsigned char BYTE;       // BYTE 타입을 unsigned char로 정의
typedef unsigned short WORD;      // WORD 타입을 unsigned short로 정의
typedef unsigned long DWORD;      // DWORD 타입을 unsigned long으로 정의
typedef unsigned long long QWORD; // QWORD 타입을 unsigned long long으로 정의

#if SYSTEM_BIT == 32
typedef DWORD INT; // 32비트 시스템에서는 INT를 DWORD로 정의
#else
typedef QWORD INT; // 64비트 시스템에서는 INT를 QWORD로 정의
#endif

// 프로시저 선언을 위한 매크로
#define PROTO(proc) static INT proc(); // 내부 프로시저 선언
#define EXTERN(proc) INT proc();       // 외부 프로시저 선언

// 프로시저 관련 함수 선언
static INT proc_init(); // 프로시저 초기화 함수
static INT proc_retn(); // 프로시저 반환 함수
static INT print_int(); // 콘솔에 정수를 출력하는 함수
static INT print_str(); // 콘솔에 문자열을 출력하는 함수

#define MAX_MEMORY_LEN 100 // 최대 메모리 길이

static BYTE m[MAX_MEMORY_LEN];                       // 메모리 배열 선언
static INT a, b, c, d;                               // INT 타입 변수 선언
static INT sp = MAX_MEMORY_LEN, bp = MAX_MEMORY_LEN; // 스택 포인터와 베이스 포인터 초기화
static INT flag = 0;                                 // 플래그 변수 초기화

static INT ip = 0;           // 명령 포인터 초기화
static jmp_buf _ipArray[10]; // 점프 버퍼 배열 선언

static jmp_buf _ip; // 점프 버퍼 선언

enum FLAG
{
    FLAG_SIGNED = 1,   // 부호 플래그
    FLAG_ZERO = 1 << 2 // 제로 플래그
};

// 프로시저 시작을 정의하는 매크로
#define PROC(proc_name)            \
    static int proc_name##DTCASE() \
    {                              \
        proc_init();

#define ENDP     \
    _end_proc:   \
    proc_retn(); \
    return 0;    \
    }

// PUSH 명령어를 정의하는 매크로
#define PUSH(param)          \
    {                        \
        INT *p;              \
        sp -= sizeof(INT);   \
        p = (INT *)(m + sp); \
        *p = (INT)(param);   \
    }
// POP 명령어를 정의하는 매크로
#define POP(param)           \
    {                        \
        INT *p;              \
        p = (INT *)(m + sp); \
        sp += sizeof(INT);   \
        (param) = *p;        \
    }

// MOVL 명령어를 정의하는 매크로
#define MOVL(dst, src)      \
    {                       \
        (dst) = (INT)(src); \
    }
// ADD 명령어를 정의하는 매크로
#define ADD(dst, src)        \
    {                        \
        (dst) += (INT)(src); \
    }
// SUB 명령어를 정의하는 매크로
#define SUB(dst, src)        \
    {                        \
        (dst) -= (INT)(src); \
    }
// MUL 명령어를 정의하는 매크로
#define MUL(dst, src)        \
    {                        \
        (dst) *= (INT)(src); \
    }
// DIV 명령어를 정의하는 매크로
#define DIV(dst, src)        \
    {                        \
        (dst) /= (INT)(src); \
    }

// JMP 명령어를 정의하는 매크로
#define JMP(lbl)  \
    {             \
        goto lbl; \
    }

// CMP 명령어를 정의하는 매크로
#define CMP(p1, p2)                            \
    {                                          \
        flag = ((p1) == (p2)) ? FLAG_ZERO : 0; \
    }
// JZ 명령어를 정의하는 매크로
#define JZ(lbl)           \
    if (flag & FLAG_ZERO) \
    {                     \
        goto lbl;         \
    }
// JNZ 명령어를 정의하는 매크로
#define JNZ(lbl)             \
    if (!(flag & FLAG_ZERO)) \
    {                        \
        goto lbl;            \
    }

// INC 명령어를 정의하는 매크로
#define INC(param) \
    {              \
        ++(param); \
    }
// DEC 명령어를 정의하는 매크로
#define DEC(param) \
    {              \
        --(param); \
    }

// LEA 명령어를 정의하는 매크로
#define LEA(dst, src)          \
    {                          \
        (dst) = (void *)(src); \
    }
// SETL 명령어를 정의하는 매크로
#define SETL(dst, src)         \
    {                          \
        *(INT *)(dst) = (src); \
    }
// GETL 명령어를 정의하는 매크로
#define GETL(dst, src)         \
    {                          \
        (dst) = *(INT *)(src); \
    }

// CALL 명령어를 정의하는 매크로
#define CALL(proc)                                         \
    {                                                      \
        static INT ip;                                     \
        static jmp_buf _ip;                                \
        static jmp_buf _ipArray[10];                       \
        if (setjmp(_ip) == 0)                              \
        {                                                  \
            memcpy(_ipArray[ip++], &_ip, sizeof(jmp_buf)); \
            PUSH(ip);                                      \
            proc();                                        \
        }                                                  \
    }
// INVOKE 명령어를 정의하는 매크로
#define INVOKE(proc)  \
    {                 \
        static INT n; \
        CALL(proc);   \
        POP(n);       \
    }
// RETURN 명령어를 정의하는 매크로
#define RETURN()        \
    {                   \
        goto _end_proc; \
    }
// RET 명령어를 정의하는 매크로
#define RET()                                            \
    {                                                    \
        static INT ip;                                   \
        static jmp_buf _ip;                              \
        static jmp_buf _ipArray[];                       \
        POP(ip);                                         \
        memcpy(&_ip, _ipArray[ip - 1], sizeof(jmp_buf)); \
        longjmp(_ip, a);                                 \
    }

// LOOP 명령어를 정의하는 매크로
#define LOOP(index, start, end) \
    MOVL(index, start);         \
    while (index < end)         \
    {

// ENDLOOP 명령어를 정의하는 매크로
#define ENDLOOP(index) \
    INC(index);        \
    }
// 프로그램 종료 명령어를 정의하는 매크로
#define EXIT()   \
    {            \
        exit(a); \
    }

// 프로시저를 정의하는 매크로 (엔트리 포인트 없음)
#define PROC_NAKED(proc_name) \
    static INT proc_name()    \
    {
// 프로시저 종료를 정의하는 매크로 (엔트리 포인트 없음)
#define ENDP_NAKED \
    _end_proc:     \
    return 0;      \
    }

static INT n; // 전역 변수 n 선언

// 프로시저 초기화 함수
static INT proc_init()
{
    PUSH(bp);     // 현재 베이스 포인터를 스택에 저장
    MOVL(bp, sp); // 베이스 포인터를 현재 스택 포인터로 설정
    return 0;
}
// 프로시저 반환 함수
static INT proc_retn()
{
    MOVL(sp, bp); // 스택 포인터를 베이스 포인터로 설정
    POP(bp);      // 이전 베이스 포인터를 스택에서 복원
    return 0;
}

// 콘솔에 정수를 출력하는 함수
static INT print_int()
{
    INT num = *(INT *)(m + sizeof(INT) + sp); // 스택에서 정수 값 가져오기
    return printf("%d", (int)num);            // 정수 값 출력
}
// 콘솔에 문자열을 출력하는 함수
static INT print_str()
{
    INT val = *(INT *)(m + sizeof(INT) + sp); // 스택에서 문자열 주소 가져오기
    return printf("%s", (const char *)val);   // 문자열 출력
}

#endif
