/* DESCRIPTION- this LKM will create a file in the file system
 * refer to http://www.icepack-linux.com/linux-kernel-methods-to-read-and-write-file-data/ 
 */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include "ioctl.h"

unsigned int cmd;
unsigned long arg;
static struct class *char_class;
static struct device *pdev;
struct cdev *cdev;
struct file *filp;
static dev_t dev;
static long device_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= device_ioctl,

};
static int __init add(void)
{
	int ret, check;
	printk(KERN_DEBUG"%s\n", __func__);
	ret=alloc_chrdev_region(&dev,0,1,"ioctl_module");
	if (ret!=0) {
		printk(KERN_ALERT "alloc_chrdev failed!!!\n");
		return -1;
	}

	cdev=kmalloc(sizeof(struct cdev),GFP_KERNEL);	
	printk("##%d\n",MAJOR(dev));
	printk("##%d\n",MINOR(dev));
	cdev->owner = THIS_MODULE;
	cdev_init(cdev,&fops);
	check=cdev_add(cdev,dev,1);
	if (check<0) {
		printk(KERN_ALERT "cdev_add failed!!!\n");
		return -1;
	}
	char_class = class_create(THIS_MODULE, "char_class");
        if (IS_ERR(char_class)) {
                printk(KERN_ERR"unable to create class\n");
                return PTR_ERR(char_class);
        }

        pdev = device_create(char_class, NULL, dev, NULL, "char_dev_node");
        if (IS_ERR(pdev)) {
		printk(KERN_ERR"unable create device node\n");
	        return PTR_ERR(pdev);
        }       
	
	return 0;
}

static void __exit remove(void)
{
	printk(KERN_DEBUG"%s\n", __func__);
	cdev_del(cdev);
	device_destroy(char_class, dev);
	unregister_chrdev_region(dev,1);
	class_destroy(char_class);
	char_class = NULL; 
}

static long device_ioctl(struct file *filp,unsigned int cmd,unsigned long arg) 
{
	
	char *path;
	printk(KERN_DEBUG "##%s\n",__func__);
	switch(cmd) {
	case IOCTL_HELLO: 
		printk("IOCTL HELLO\n");
		break;
	case IOCTL_WRITE:
		printk("IOCTL_WRITE\n");
		path=(char*)arg;
		filp=filp_open(path,O_CREAT | O_RDWR, 0666);
		if (IS_ERR(filp)) {
			printk(KERN_ERR"**Error**\n");	
			return -1;
		}
		break;
		
	default:
		printk("Default\n");
		return 1;
	}
return 0;
}
 
module_init(add);
module_exit(remove);

MODULE_DESCRIPTION("ioctl hello");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kopal <kopalsingh.work@gmail.com>");
