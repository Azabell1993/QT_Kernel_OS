
kernel_printf("Hello world!\n");
kernel_printf("A single character : %c \n", 'T');
kernel_printf("An integer : %d \n", 37);
kernel_printf("An integer : %d \n", 299);
kernel_printf("5-4 = %d\n", 1);

int a;
int b;

a = 1;
b = 2;

kernel_printf("%d + %d = %d\n", a, b, a + b);
kernel_printf("%d\t\t\t String.\n", 12345678);
kernel_printf("-650\n");
kernel_printf("%+d\n", 430);
kernel_printf("%+1d\n", 650);
kernel_printf("%+10d\n", 499);
kernel_printf("% 3d\n", 1230);
kernel_printf("%08d\n", 342);
kernel_printf("%+03d\n", -430);
kernel_printf("%3d\n", -43);
kernel_printf("%u\n", 23919293929392);
kernel_printf("%+-u\n", 12345);
kernel_printf("%+10u\n", 12345);
kernel_printf("%-4s\n", "Az");
kernel_printf("%o\n", 333);
kernel_printf("%-0#+10o\n", 2048);
kernel_printf("%X\n", 12345678);
kernel_printf("%#+x\n", 12345678);
kernel_printf("\n\nfunction call Test\n");
kernel_printf("%d + %d = %d", 5, 6, function_Test(5, 6));
kernel_printf("\n");
kernel_putchar('H');
kernel_putchar('E');
kernel_putchar('L');
kernel_putchar('L');
kernel_putchar('O');
kernel_putchar('\n');
kernel_printf("-\n");
kernel_printf(".\n");
kernel_printf("/\n");
kernel_printf("'()*+,-./\n");
