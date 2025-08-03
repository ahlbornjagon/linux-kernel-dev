#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h> 
#include <linux/export.h>
#include <linux/errno.h>
#include <linux/uaccess.h>


#define DEVICE_NAME "doggo_char"
static struct file_operations fops;

static int doggo_major = 0;

struct doggo_data {
    char *buffer;
    size_t size;
    size_t capacity;
};

struct doggo_data *global_data = NULL;

static int doggo_file_open(struct inode *inode, struct file *filePtr)
{
    printk(KERN_INFO "=== doggo_file_open START ===\n");
    
    if (!global_data) {
        printk(KERN_INFO "global_doggo_data is NULL, allocating...\n");
        
        global_data = kmalloc(sizeof(*global_data), GFP_KERNEL);
        if (!global_data) {
            printk(KERN_ERR "FAILED to allocate global_data\n");
            return -ENOMEM; 
        }
        printk(KERN_INFO "Successfully allocated global_data\n");
        
        global_data->buffer = kmalloc(1024, GFP_KERNEL);
        if (!global_data->buffer) {
            printk(KERN_ERR "FAILED to allocate buffer\n");
            kfree(global_data);
            global_data = NULL;
            return -ENOMEM; 
        }
        printk(KERN_INFO "Successfully allocated buffer\n");
        
        global_data->size = 0;
        global_data->capacity = 1024;
        printk(KERN_INFO "Initialized size=0, capacity=1024\n");
    } else {
        printk(KERN_INFO "global_data already exists\n");
    }
    filePtr->private_data = global_data;
    
    printk(KERN_INFO "=== doggo_file_open SUCCESS ===\n");
    return 0;
}

static int doggo_file_release(struct inode *inode, struct file *filePtr)
{

    printk(KERN_INFO "Releasing doggo file now");
    struct doggo_data *data = filePtr->private_data;

    
    filePtr->private_data = NULL;  // Clean that reference 

    printk(KERN_INFO "Returning from doggo file release");
    return 0;
}

static ssize_t doggo_file_read(struct file *filePtr, char __user *buffer, size_t count, loff_t *pos)
{
    if ((global_data->size == 0) ||  (*pos >= global_data->size))
    {
        return 0;
    }
    if (*pos + count > global_data->size)
    {
        count = global_data->size - *pos;
    }

    if ( copy_to_user(buffer, global_data->buffer + *pos, count) != 0)
    {
        return -EFAULT;
    }
    *pos += count;
    return count;
}

// Commenting on this one since it was a little harder
static ssize_t doggo_file_write(struct file *filePtr, const char __user *buffer, size_t len, loff_t *pos)
{    
    printk(KERN_INFO "doggo_file_write called: len=%zu, pos=%lld\n", len, *pos);

    // Check if the buffer position is greater than the capacity of the buffer
    if (*pos >= global_data->capacity)
    {
        return 0;
    }

    // If the position in the buffer plus the size of bytes to write is greater than the capacity of the buffer
    // then we set the length of bytes to write to the max (which is our capacity - the current buffer position)
    // and write the partial data
    if (*pos + len > global_data->capacity)
    {
        len = global_data->capacity - *pos;
    }

    // Copy the data from our buffer, starting at the current position (pos), to the user buffer that was passed in to us
    if ( copy_from_user(global_data->buffer + *pos, buffer, len) != 0)
    {
        return -EFAULT;
    }

    // Incrememnt the current buffer position via the length of bytes we wrote.
    *pos += len;

    // Check if the current position is greater than the remaining size of the buffer. If so, we set the new size of the buffer
    // i.e. the amount of space used 
    if (*pos > global_data->size)
    {
        global_data->size = *pos;  // New data size
    }

    // Return the amount of bytes we wrote to the file. 
    printk(KERN_INFO "doggo_file_write returning %zu bytes\n", len);

    return len;
}

static int register_doggo(void)
{
    int registerStatus = register_chrdev(doggo_major, DEVICE_NAME, &fops );
    if (registerStatus < 0){

        printk(KERN_INFO "doggo_char: couldnt register device with error code %i/n", registerStatus);
        return registerStatus;
    }
    doggo_major = registerStatus;
    printk(KERN_INFO "doggo_char: registered device with major number %i\n", doggo_major);
    return 0;
}
void unregister_doggo(void)
{
	printk( KERN_NOTICE "doggo_char: unregistered device with major number %i/n", doggo_major );
	if(doggo_major != 0)
	{
		unregister_chrdev(doggo_major, DEVICE_NAME);
	}
}

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read  = doggo_file_read,
    .write = doggo_file_write,
    .open = doggo_file_open,
    .release = doggo_file_release
};


static int doggo_init(void)
{
    int result;
    
    printk(KERN_INFO "Doggo goes bork bork hello\n");
    
    result = register_doggo();
    if (result < 0) {
        printk(KERN_ERR "Doggo failed to register device\n");
        return result;
    }
    
    printk(KERN_INFO "Doggo successfully initialized\n");
    return 0;
}

static void doggo_exit(void)
{
    printk(KERN_INFO "Doggo wags tail goodbye\n");
    unregister_doggo();


    if (global_data) {
        if (global_data->buffer) {
            kfree(global_data->buffer);
        }
        kfree(global_data);
        global_data = NULL;
        printk(KERN_INFO "doggo: freed global doggo_data\n");
    }
}

module_init(doggo_init);
module_exit(doggo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jagon Ahlborn");