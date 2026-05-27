#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define DEVICE_NAME "jiffies_dev"

static int major;
static unsigned long start_jiffies;

static ssize_t driver_read(struct file *file,char __user *buffer,size_t len,loff_t *offset)
{
char msg[100];
int bytes;
unsigned long elapsed;

if (*offset > 0)
return 0;

elapsed = jiffies - start_jiffies;
bytes = sprintf(msg,"Elapsed Jiffies: %lu\nElapsed Seconds: %lu\n",elapsed,elapsed / HZ);
if (copy_to_user(buffer, msg, bytes))
{
    return -EFAULT;
}

*offset = bytes;

return bytes;
}

static struct file_operations fops =
{
.owner = THIS_MODULE,
.read = driver_read,
};

static int __init jiffies_init(void)
{
start_jiffies = jiffies;

major = register_chrdev(0, DEVICE_NAME, &fops);

printk(KERN_INFO "Jiffies Driver Loaded\n");
printk(KERN_INFO "Start Jiffies = %lu\n", start_jiffies);
return 0;
}

static void __exit jiffies_exit(void)
{
printk(KERN_INFO "Jiffies Driver Removed\n");

unregister_chrdev(major, DEVICE_NAME);
}

module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenAI");
MODULE_DESCRIPTION("Simple Jiffies Driver");
