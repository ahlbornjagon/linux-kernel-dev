#include <linux/init.h>
#include <linux/module.h>

static int doggo_init(void)
{
    printk(KERN_INFO "Doggo goes bork bork hello\n");
    return 0;
}

static void doggo_exit(void)
{
    printk(KERN_INFO "Doggo wags tail goodbye\n");
    return;
}

module_init(doggo_init);
module_exit(doggo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jagon Ahlborn");