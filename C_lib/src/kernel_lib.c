// kernel_lib.c
#include <stdio.h>
#include <stdbool.h>

// 예제 커널 함수: 새로운 프로세스를 생성하는 함수
bool kernel_create_process(const char *process_name)
{
    // 실제 커널 레벨에서는 시스템 호출이나 다른 복잡한 작업이 수행되겠지만,
    // 여기서는 단순히 콘솔에 메시지를 출력하는 것으로 예제를 만듭니다.
    printf("Creating process: %s\n", process_name);
    // 프로세스 생성이 성공했다고 가정
    return true;
}
