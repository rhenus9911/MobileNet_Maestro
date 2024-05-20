#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");


static int test_init(void){
    printk(KERN_INFO "OK HELLO\n");
    return 0;
}

static int test_exit(void){
    printk(KERN_INFO "OK BYE\n");
}

module_init(test_init);
module_init(test_exit);