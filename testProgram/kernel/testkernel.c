#include <linux/module.h>
#include <linux/kernel.h>

int __init test_init(void) {
    // 초기화 코드
    printk(KERN_INFO "Test module initialized\n");
    return 0;
}

void __exit test_exit(void) {
    // 종료 코드
    printk(KERN_INFO "Test module exited\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple test module");
