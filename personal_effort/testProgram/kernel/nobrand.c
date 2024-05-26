#include <linux/module.h>
#include <linux/fs.h>

#define NOD_NAME "nobrand"
#define NOD_MAJOR 100  // Device File의 Major Number

MODULE_LICENSE("GPL");

static int nobrand_open(struct inode *inode, struct file *filp) {
    printk( KERN_INFO "OPEN!!!\n");
    return 0;
}

static int nobrand_release(struct inode *inode, struct file *filp) {
    printk( KERN_INFO "CLOSE!!!\n");
    return 0;
}

static ssize_t nobrand_read(struct file *filp, char *buf, size_t count, loff_t *ppos) {
	buf[0] = 'H';
	buf[1] = 'I';
	buf[2] = '\0';

	return count;
}

static ssize_t nobrand_write(struct file *filp, const char *buf, size_t count, loff_t *ppos) {
	printk( KERN_INFO "HIHI\n\n");
	printk( KERN_INFO "app message : %s\n", buf);

	return count;
}

static long nobrand_ioctl(struct file *flip, unsigned int cmd, unsigned long arg)
{
	printk( KERN_INFO "%d\n", cmd);
	return 0;
}

static struct file_operations fops = {
    .open = nobrand_open,  // app.c에서 open 명령어 실행시 실행
    .release = nobrand_release, // app.c에서 close 명령어 실행 시 실행
    .read = nobrand_read,
    .write = nobrand_write,
    .unlocked_ioctl = nobrand_ioctl
};

static int nobrand_init(void) {  // insmod 명령어 입력시 실행
    printk( KERN_INFO "OK HELLO NOBRAND!!\n");  // KERN_INFO : log에 print하기 위해
    if (register_chrdev(NOD_MAJOR, NOD_NAME, &fops) < 0) { // device file에 접근 실패시 실행
        printk( KERN_INFO "ERROR!!! register error\n");
    }

    return 0;
}

static void nobrand_exit(void) { // rmmod 명령어 입력시 실행
    unregister_chrdev(NOD_MAJOR, NOD_NAME);
    printk( KERN_INFO "BYE BYE\n\n");
}



module_init(nobrand_init);
module_exit(nobrand_exit);
