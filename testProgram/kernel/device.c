#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

static int my_device_probe(struct platform_device *pdev) {
    pr_info("My device has been probed\n");
    // 하드웨어 초기화 및 점검 코드
    return 0;
}

static int my_device_remove(struct platform_device *pdev) {
    pr_info("My device has been removed\n");
    // 하드웨어 자원 해제 코드
    return 0;
}

static const struct of_device_id my_device_of_match[] = {
    { .compatible = "my,device" },
    { }
};
MODULE_DEVICE_TABLE(of, my_device_of_match);

static struct platform_driver my_device_driver = {
    .probe = my_device_probe,
    .remove = my_device_remove,
    .driver = {
        .name = "my_device",
        .of_match_table = my_device_of_match,
    },
};

module_platform_driver(my_device_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple device driver example");
