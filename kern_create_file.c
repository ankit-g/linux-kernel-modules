/* DESCRIPTION- this LKM will create a file in the file system
 * refer to http://www.icepack-linux.com/linux-kernel-methods-to-read-and-write-file-data/ 
 */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

struct file *filp;
static int __init add(void)
{
	printk("%s\n", __func__);
	filp=filp_open("/home/kopal/git/linux-kernel-modules/kern_file",O_CREAT,0);
	if (IS_ERR(filp)) {
		printk(KERN_WARNING "**Error**\n");	
		return -1;
	}
	return 0;
}

static void __exit remove(void)
{
	filp_close(filp,NULL);
	printk("%s\n", __func__);
	
} 
module_init(add);
module_exit(remove);

