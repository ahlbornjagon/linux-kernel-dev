#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h> 
#include <linux/export.h>

#define DEVICE_NAME "doggo_char"


// TODO: Write doggo_file_read and _file_write functions

static int doggo_major = 0;
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read  = doggo_file_read,
    .write = doggo_file_write,
};

static int register_doggo(void)
{
    int registerStatus = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops );
    if (registerStatus < 0){

        printk(KERN_WARNING "doggo_char: couldnt register device with error code %i/n", registerStatus)
        return registerStatus
    }
    doggo_major = registerStatus;
    printk(KERN_NOTICE "doggo_char: registered device with major number %i/n", doggo_major);
    return 0;
}


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