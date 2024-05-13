#include <linux/module.h>      // 필수 헤더, 모듈 관련 매크로가 포함됨
#include <linux/kernel.h>      // printk() 함수를 사용하기 위함
#include <linux/init.h>        // __init 및 __exit 매크로 사용

MODULE_LICENSE("GPL");         // 모듈 라이센스 설정
MODULE_AUTHOR("Your Name");    // 모듈 작성자
MODULE_DESCRIPTION("A simple Hello World Module");

static int __init hello_start(void) {
    printk(KERN_INFO "Hello, World!\n");
    return 0; // 성공적으로 로드됨
}

static void __exit hello_end(void) {
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(hello_start);
module_exit(hello_end);
