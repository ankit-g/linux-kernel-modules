/* DESCRIPTION- this LKM will create a file in the file system
 * refer to http://www.icepack-linux.com/linux-kernel-methods-to-read-and-write-file-data/ 
 */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include "ioctl.h"

unsigned int cmd;
unsigned long arg;
struct cdev *cdev;
struct file *filp;
dev_t dev;
static long device_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= device_ioctl,

};
static int __init add(void)
{
	int ret, check;
	printk(KERN_DEBUG"%s\n", __func__);
/*	filp=filp_open("/home/kopal/git/linux-kernel-modules/kern_file",O_CREAT,0);
	if (IS_ERR(filp)) {
		printk(KERN_ERR"**Error**\n");	
		return -1;
	}*/
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
	
	return 0;
}

static void __exit remove(void)
{
	//filp_close(filp,NULL);
	printk(KERN_DEBUG"%s\n", __func__);
	cdev_del(cdev);
	unregister_chrdev_region(dev,1);
}

static long device_ioctl(struct file *filp,unsigned int cmd,unsigned long arg) 
{
	printk(KERN_DEBUG "##%s\n",__func__);
	switch(cmd) {
	case IOCTL_HELLO: 
		printk("IOCTL HELLO\n");
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
