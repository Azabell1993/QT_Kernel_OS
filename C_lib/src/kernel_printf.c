// kernel_printf_function
/*
 * Kernel Printf Function
 *
 * Maintainer: Park Jiwoo
 *
 * Copyright (C) 2024 Park-Jiwoo
 *
 */
#include <stdio.h>
#include <stdarg.h>
#include "kernel_pr_he.h"

// 함수 전방 선언
int az_default_mod(char *format);
static void az_default(char *format, va_list ap);
static void az_input(char *format, va_list ap);
void az_mod(char *format);
void az_putstr(const char *s);
int az_chrpos(const char *s, int c);
void az_putchar(char c);
int az_skip(char *format, int i);
void az_fill(int fillcnt, char c);
char az_getflag(char flag);
void az_plusflag(int d, char flag, int param, char *format);
void az_minusflag(int d, char flag);
void az_spaceflag(int d, char flag, int param);
void az_zeroflag(int d, char flag);
int az_output(char *format, char flag, va_list ap, int i);
void az_speciftypeini(char *format, char specif, char flag, va_list ap);
void az_o_support(int o, int param, char flag, char f_addon);
void az_o_support_p2(char flag, char *fmt, char f_addon);
void az_c_support(char c, int param, char flag, char f_addon);
int az_getparam(char *format, char flag);
void az_c(va_list ap, char *format, char flag);
void az_s(va_list ap, char *format, char flag);
void az_p(va_list ap);
void az_x(va_list ap, char *format, char flag);
void az_d(va_list ap, char *format, char flag);
void az_i(va_list ap, char *format, char flag);
void az_o(va_list ap, char *format, char flag);
void az_u(va_list ap, char *format, char flag);

// az_default 함수는 포맷 문자열을 분석하고 그에 따라 적절한 처리를 수행합니다.
static void az_default(char *format, va_list ap)
{
    int st;

    st = az_default_mod(format); // 포맷 문자열에 따라 처리 방식을 결정

    if (st == 1)
    {
        az_input(format, ap); // 포맷 문자열에 따라 가변 인자를 처리
    }
    else if (st == 2)
    {
        az_mod(format); // 포맷 문자열에서 '%%'를 처리
    }
    else
    {
        if (st < 0)
        {
            az_putstr(format); // 포맷에 '%' 문자가 없으면 문자열을 그대로 출력
        }
    }
}

// az_default_mod 함수는 포맷 문자열에 '%' 문자가 포함되어 있는지 확인합니다.
int az_default_mod(char *format)
{
    int mod;

    mod = az_chrpos(format, '%'); // 포맷 문자열에서 '%'의 위치를 찾음
    if (format[mod + 1] == '%' && format[mod] == '%')
    {
        return (2); // '%%'가 있으면 2를 반환 (az_mod 호출을 위해)
    }
    else if (mod == -1)
    {
        return (-1); // '%'가 없으면 -1을 반환
    }
    else if (mod == 0 || format[0] == '%')
    {
        return (1); // 포맷의 시작이 '%'이거나 '%x' 형태라면 1을 반환 (az_input 호출을 위해)
    }
    else if (mod > 0)
    {
        return (1); // 중간에 '%'가 있으면 1을 반환
    }
    else
    {
        return (0); // 다른 경우에는 0을 반환
    }
}

// az_mod 함수는 포맷 문자열에서 '%%'를 처리하여 '%'를 출력합니다.
void az_mod(char *format)
{
    int i;
    i = 0;
    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '%')
            {
                az_putchar(format[i++]); // '%%'를 '%'로 변환하여 출력
            }
        }
        az_putchar(format[i]); // 나머지 문자 출력
        i++;
    }
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// az_input 함수는 포맷 문자열을 순차적으로 분석하여 가변 인자를 적절히 처리합니다.
static void az_input(char *format, va_list ap)
{
    char flag;
    int i;

    i = 0;
    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            flag = az_getflag(format[i]);       // 플래그 문자를 가져옴
            i = az_output(format, flag, ap, i); // 플래그에 따른 출력 처리
        }
        else if (format[i] != '\0')
        {
            az_putchar(format[i]); // 일반 문자 출력
        }
        i++;
    }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////// function flag setting ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// az_skip 함수는 플래그 문자 이후 알파벳이 나올 때까지 건너뜁니다.
int az_skip(char *format, int i)
{
    while (!az_isalpha(format[i])) // 알파벳이 나올 때까지
        i++;
    return (i);
}

// az_fill 함수는 특정 문자로 지정된 횟수만큼 채웁니다.
void az_fill(int fillcnt, char c)
{
    int i;

    i = 0;
    while (i <= fillcnt)
    {
        az_putchar(c); // fillcnt 횟수만큼 c 문자를 출력
        i++;
    }
}

// az_getflag 함수는 플래그 문자를 반환합니다.
char az_getflag(char flag)
{
    if (flag == '0')
        return ('0');
    else if (flag == '#')
        return ('#');
    else if (flag == '+')
        return ('+');
    else if (flag == '-')
        return ('-');
    else if (flag == '*')
        return ('*');
    else if (flag == '.')
        return ('.');
    else if (flag == '*')
        return ('*');
    else
        return ('\0'); // 플래그가 없으면 null 문자 반환
}

// az_plusflag 함수는 '+' 플래그에 따른 처리를 수행합니다.
void az_plusflag(int d, char flag, int param, char *format)
{
    int width;
    char add_flag;
    char add_flag_val;

    width = param - az_nbrlen(d); // 출력 너비 계산
    add_flag = format[az_chrpos(format, flag) + 1];
    add_flag_val = format[az_chrpos(format, flag) + 2];

    if (flag == '+' && add_flag != '0')
    {
        if (width)
            az_fill(width, ' '); // 빈칸 채움
        if (d > 0)
            az_putchar('+'); // 양수이면 '+' 출력
        az_putnbr(d);        // 숫자 출력
    }

    if (flag == '+' && add_flag == '0')
    {
        d *= -1;         // 음수로 변환
        az_putchar('-'); // '-' 출력

        if (add_flag_val != '1')
            az_putchar('0'); // '0' 출력
        az_putnbr(d);        // 숫자 출력
    }
}

// az_minusflag 함수는 '-' 플래그에 따른 처리를 수행합니다.
void az_minusflag(int d, char flag)
{
    if (flag == '-')
        az_putnbr(d); // '-' 플래그일 경우 숫자 출력
}

// az_spaceflag 함수는 ' ' 플래그에 따른 처리를 수행합니다.
void az_spaceflag(int d, char flag, int param)
{
    if (flag == ' ' && (param < 0) && d > 0)
        az_putchar(' '); // 공백 플래그가 설정되었을 때 공백 출력

    if (flag == ' ' && param)
    {
        az_fill(param - az_nbrlen(d), ' '); // 공백을 채움
        az_putnbr(d);                       // 숫자 출력
    }
}

// az_zeroflag 함수는 '0' 플래그에 따른 처리를 수행합니다.
void az_zeroflag(int d, char flag)
{
    if (flag == '0' && d < 0)
    {
        d *= -1;         // 음수로 변환
        az_putchar('-'); // '-' 출력
        az_putchar('0'); // '0' 출력
        az_putnbr(d);    // 숫자 출력
    }
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

// az_output 함수는 플래그에 따라 포맷팅된 출력 문자열을 생성합니다.
int az_output(char *format, char flag, va_list ap, int i)
{
    i = az_skip(format, i);                        // 플래그 이후의 위치로 이동
    az_speciftypeini(format, format[i], flag, ap); // 플래그와 타입에 따른 출력 처리
    return (i);
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// support  ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// az_o_support 함수는 'o' (8진수) 형식에 대한 처리를 지원합니다.
void az_o_support(int o, int param, char flag, char f_addon)
{
    if (o)
    {
        if (flag == '+' && param > 0)
            az_fill(param - az_nbrlen(o), ' '); // '+' 플래그에 따른 공백 채움
        if (flag == '0' && param > 0)
            az_fill(param - az_nbrlen(o), '0'); // '0' 플래그에 따른 0 채움
        if (flag == '#')
        {
            if (f_addon == '+' && param > 0)
                az_fill(param - az_nbrlen(o), ' '); // '#' 플래그에 따른 공백 채움
            az_putchar('0');                        // '0' 출력
        }
        az_putoctal(o); // 8진수 출력
        if (flag == '#' && f_addon == '-')
            az_fill(param - az_nbrlen(o), ' '); // '-' 플래그에 따른 공백 채움
        if (flag == '-' && param > 0)
            az_fill(param - az_nbrlen(o), ' '); // '-' 플래그에 따른 공백 채움
    }
}

// az_o_support_p2 함수는 'o' (8진수) 형식에 대한 추가 처리를 지원합니다.
void az_o_support_p2(char flag, char *fmt, char f_addon)
{
    char *tmp;
    int param;

    param = '\0';
    if (flag == '-' && f_addon == '0')
    {
        if (az_chrpos(fmt, '#') > 0)
        {
            az_putchar('0'); // '0' 출력
            tmp = az_strsub(fmt, az_chrpos(fmt, '0') + 1, az_strlen(fmt));
        }
        else
            tmp = az_strsub(fmt, az_chrpos(fmt, '+'), az_strlen(fmt));
        param = az_getparam(tmp, flag) - 1;
    }
    else if (flag == '#' && f_addon == '-')
        tmp = az_strsub(fmt, az_chrpos(fmt, '+') + 1, az_strlen(fmt));
    else
        param = az_getparam(fmt, flag) - 1;
    if (param > 0 && !flag)
        az_fill(param - 1, ' '); // 공백 채움
}

// az_c_support 함수는 'c' (문자) 형식에 대한 처리를 지원합니다.
void az_c_support(char c, int param, char flag, char f_addon)
{
    if (param > 0 && !flag)
        az_fill(param - 1, ' '); // 공백 채움
    az_putchar(c);               // 문자 출력
    if (f_addon == '-' && flag == '+')
        az_fill(param - 1, ' '); // 공백 채움
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

/* getparam */

// az_getparam 함수는 포맷 문자열에서 파라미터 값을 추출합니다.
int az_getparam(char *format, char flag)
{
    char *tmp;
    int st;
    int i;
    int j;

    tmp = az_memalloc(az_strlen(format)); // 메모리 할당
    i = 0;
    if (format[i] == ' ' && az_isdigit(format[i + 1]))
        i++;

    if (flag)
        while (!az_isdigit(format[i]) && format[i] != ' ')
            i++;
    else if (az_isdigit(format[az_chrpos(format, '%') + 1]))
        i++;
    else
        i = az_chrpos(format, flag) + 1;
    j = 0;

    while (az_isdigit(format[i]))
        tmp[j++] = format[i++];
    tmp[j] = '\0';
    st = az_atoi(tmp); // 문자열을 정수로 변환

    free(tmp); // 메모리 해제
    return (st);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// az_speciftypeini 함수는 포맷 문자에 따라 적절한 출력 함수를 호출합니다.
void az_speciftypeini(char *format, char specif, char flag, va_list ap)
{
    if (specif == 'c' || specif == 'C')
        az_c(ap, format, flag);
    else if (specif == 's' || specif == 'S')
        az_s(ap, format, flag);
    else if (specif == 'p')
        az_p(ap);
    else if (specif == 'x' || specif == 'X')
        az_x(ap, format, flag);
    else if (specif == 'd' || specif == 'D')
        az_d(ap, format, flag);
    else if (specif == 'i')
        az_i(ap, format, flag);
    else if (specif == 'o' || specif == 'O')
        az_o(ap, format, flag);
    else if (specif == 'u' || specif == 'U')
        az_u(ap, format, flag);
}

// az_c 함수는 'c' (문자) 형식의 데이터를 처리합니다.
void az_c(va_list ap, char *format, char flag)
{
    char c;
    char f_addon;
    char *tmp;
    int param;
    int p_addon;

    c = (char)va_arg(ap, int);
    f_addon = format[az_chrpos(format, flag) + 1];
    tmp = az_strsub(format, az_chrpos(format, f_addon), az_strlen(format) - 2);
    param = az_getparam(format, flag) - 1;
    p_addon = az_getparam(tmp, flag) - 1;
    if (flag == '+' && f_addon != '-')
        az_fill(param - 1, ' ');
    if (flag == '0' && f_addon != '+')
        az_fill(param - 1, ' ');
    if (f_addon == '+' && flag == '0')
        az_fill(p_addon - 1, ' ');
    if (c)
        az_c_support(c, param, flag, f_addon);
}

// az_s 함수는 's' (문자열) 형식의 데이터를 처리합니다.
void az_s(va_list ap, char *format, char flag)
{
    char *s;
    int param;
    int s_length;

    s = va_arg(ap, char *);
    if (s == NULL && flag == '\0')
        az_putstr("(null)");
    else
    {
        param = az_getparam(format, flag) - 1;
        s_length = az_strlen(s);
        if (az_strcmp(s, "") == 0 && !flag)
            az_putstr("");
        if (param > 0 && (flag == ' ' || flag == '+'))
            az_fill(param - s_length, ' ');
        if (s)
        {
            if (param > 0 && !flag)
                az_fill(param - s_length, ' ');
            az_putstr(s);
            if (flag == '-')
                az_fill(param - s_length, ' ');
        }
    }
}

// az_p 함수는 'p' (포인터) 형식의 데이터를 처리합니다.
void az_p(va_list ap)
{
    int nbr;

    nbr = va_arg(ap, int);
    az_putstr("0x");
    az_puthex(nbr); // 포인터 값 16진수로 출력
}

// az_x 함수는 'x' 또는 'X' (16진수) 형식의 데이터를 처리합니다.
void az_x(va_list ap, char *format, char flag)
{
    int x;
    int param;

    x = va_arg(ap, int);
    param = az_getparam(format, flag);
    if (flag == '#' && !az_strchr(format, 'X'))
        az_putstr("0x"); // '0x' 접두사 출력
    if (flag == '#' && az_strchr(format, 'X'))
        az_putstr("0X"); // '0X' 접두사 출력
    if (x && az_strchr(format, 'X'))
        az_putstr(az_itoa(x, 16)); // 16진수 대문자 형식으로 출력
    if (x && !param && !az_strchr(format, 'X'))
        az_puthex(x); // 16진수 소문자 형식으로 출력
}

// az_d 함수는 'd' 또는 'D' (10진수) 형식의 데이터를 처리합니다.
void az_d(va_list ap, char *format, char flag)
{
    int param;
    int d;

    param = az_getparam(format, flag) - 1;
    d = va_arg(ap, int);
    if (param && flag == '\0')
        az_fill(param - az_nbrlen(d), ' ');
    if (flag)
    {
        az_plusflag(d, flag, param, format); // '+' 플래그 처리
        az_minusflag(d, flag);               // '-' 플래그 처리
        az_spaceflag(d, flag, param);        // ' ' 플래그 처리
        az_zeroflag(d, flag);                // '0' 플래그 처리
    }
    else
        az_putnbr(d); // 숫자 출력
}

// az_i 함수는 'i' (정수) 형식의 데이터를 처리합니다.
void az_i(va_list ap, char *format, char flag)
{
    int i;
    int i_length;
    int param;

    i = va_arg(ap, int);
    param = az_getparam(format, flag) - 1;
    i_length = az_nbrlen(i) + 1;
    if (flag == '0')
        az_fill(param - i_length, flag);
    if (flag == '.')
        az_fill(param - i_length, '0');
    if (flag == '+')
        if (i > 0)
            az_putchar('+');
    az_putnbr(i); // 정수 출력
}

// az_o 함수는 'o' (8진수) 형식의 데이터를 처리합니다.
void az_o(va_list ap, char *format, char flag)
{
    int o;
    int param;
    char f_addon;
    char *tmp;

    o = va_arg(ap, int);
    param = az_getparam(format, flag) - 1;
    f_addon = format[az_chrpos(format, flag) + 1];
    if (flag == '0')
    {
        tmp = az_strsub(format, az_chrpos(format, '0') + 1, az_strlen(format));
        param = az_getparam(tmp, flag) - 1;
    }
    az_o_support_p2(flag, format, f_addon);    // 추가 지원 함수 호출
    az_o_support(o, param - 1, flag, f_addon); // 8진수 출력 지원
}

// az_u 함수는 'u' (unsigned 정수) 형식의 데이터를 처리합니다.
void az_u(va_list ap, char *format, char flag)
{
    unsigned int u;
    int param;
    int u_length;

    u = va_arg(ap, unsigned int);
    param = az_getparam(format, flag) - 1;
    u_length = az_nbrlen(u);
    if (flag == '0')
        az_fill((param - u_length), flag);
    if (flag == '.')
        az_fill((param - u_length), '0');
    if (param)
        az_fill(param - u_length, ' ');
    az_putunsigned(u); // unsigned 정수 출력
}

int kernel_printf(const char *format, ...)
{
    va_list ap; // 가변 인자를 저장하기 위한 va_list 선언

    va_start(ap, format);           // 가변 인자 리스트를 초기화
    az_default((char *)format, ap); // 포맷 문자열과 가변 인자 리스트를 처리하는 az_default 함수 호출
    va_end(ap);                     // 가변 인자 리스트를 종료

    return (0); // 0을 반환
}