#include "kernel_asm.h"
#include "kernel_print.h"
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
    HelloWorld 출력
*/
#if DTCASE == 1
    void main_1()
    {
        STRING msg = "Hello, World!";
        PUSH(msg);
        CALL(print_str);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);
    }
#endif

/*
    가산기 출력 (+, -, /, *)
*/
#if DTCASE == 2
    void main_2()
    {
        MOVL(a, 10);
        MOVL(b, 5);

        ADD(a, b); // a = 10 + 5
        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);

        MOVL(a, 10);
        MOVL(b, 5);

        SUB(a, b); // a = 10 - 5
        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);

        MOVL(a, 10);
        MOVL(b, 5);

        MUL(a, b); // a = 10 * 5
        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);

        MOVL(a, 10);
        MOVL(b, 5);

        DIV(a, b); // a = 10 / 5
        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);
    }
#endif

/*
    랜덤으로 선택하여 do-while문으로 가산기 출력, exit를 입력하면 종료
*/
#if DTCASE == 3
    void main_3()
    {
        srand(time(0));
        STRING input_msg = "This Program is Random Choice Calculator.\n";
        PUSH(input_msg);
        CALL(print_str);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);

        STRING msg = "Enter 'exit' to quit.\n";
        PUSH(msg);
        CALL(print_str);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);

        char input[10];
        INT num1, num2;
        do
        {
            kernel_printf("Enter two numbers (or 'exit' to quit):\n");
            fflush(stdout);
            if (scanf("%9s", input) != 1 || strcmp(input, "exit") == 0)
                break;
            num1 = atoi(input);

            if (scanf("%9s", input) != 1 || strcmp(input, "exit") == 0)
                break;
            num2 = atoi(input);

            MOVL(a, num1);
            MOVL(b, num2);

            int choice = rand() % 4;

            switch (choice)
            {
            case 0:
                ADD(a, b);
                kernel_printf("Operation: ADD\n");
                fflush(stdout);
                break;
            case 1:
                SUB(a, b);
                kernel_printf("Operation: SUB\n");
                fflush(stdout);
                break;
            case 2:
                MUL(a, b);
                kernel_printf("Operation: MUL\n");
                fflush(stdout);
                break;
            case 3:
                DIV(a, b);
                kernel_printf("Operation: DIV\n");
                fflush(stdout);
                break;
            }

            PUSH(a);
            CALL(print_int);
            POP(n);
            kernel_printf("\n");
            fflush(stdout);

            // 스택 포인터를 초기 상태로 되돌림
            sp = MAX_MEMORY_LEN;
        } while (1);
    }
#endif

/*
    비재귀적 피보나치 수열 계산
*/
#if DTCASE == 4
    void main_4()
    {
        MOVL(a, 10);
        if (a == 0)
        {
            MOVL(b, 0);
        }
        else if (a == 1)
        {
            MOVL(b, 1);
        }
        else
        {
            INT fib1 = 0, fib2 = 1, temp, i;
            LOOP(i, 2, a + 1)
            MOVL(temp, fib1);
            ADD(fib1, fib2);
            MOVL(fib2, temp);
            ENDLOOP(i)
            MOVL(b, fib1);
        }
        PUSH(b);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);
    }
#endif

/*
    for문으로 1부터 10까지의 덧셈 구하기
*/
#if DTCASE == 5
    void main_5()
    {
        MOVL(a, 0);
        INT i;
        LOOP(i, 1, 11)
        ADD(a, i);
        ENDLOOP(i)

        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);
    }
#endif

/*
    포인터 연산 어셈블리로 하기(주소 값 대입으로 변형)
*/
#if DTCASE == 6
    void main_6()
    {
        INT x = 10;
        INT *p;
        LEA(p, &x);
        SETL(p, 20);

        PUSH(x);
        CALL(print_int);
        POP(n);
        kernel_printf("\n");
        fflush(stdout);
    }
#endif

/*
    게임 캐릭터 구조체를 만들어서 실제 게임하는 것처럼 연산해보기
*/
#if DTCASE == 7
    typedef struct
    {
        INT hp;
        INT attack;
        INT defense;
    } Character;

    void main_7()
    {
        Character hero = {100, 20, 10};
        Character monster = {80, 15, 5};

        // Hero attacks monster
        SUB(monster.hp, hero.attack);
        MOVL(a, monster.hp);
        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf(" - Monster HP after attack\n");
        fflush(stdout);

        // Monster attacks hero
        SUB(hero.hp, monster.attack);
        MOVL(a, hero.hp);
        PUSH(a);
        CALL(print_int);
        POP(n);
        kernel_printf(" - Hero HP after attack\n");
        fflush(stdout);
    }
#endif

#ifdef __cplusplus
}
#endif
