#include <stdio.h>

int main()
{
        int a;
        scanf("%d", &a);
        if(a > 10)
                printf("\033[0;32m[+] Success\n\033[0m"); // 초록색 출력
        else
                printf("\033[0;31m[+] Fail\n\033[0m"); // 빨간색 출력

        return 0;
}

