#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

static int __init add(void)
{
	printk(KERN_INFO"****Hello World****\n");
	return 0;
}

static void __exit remove(void)
{
	printk(KERN_INFO"****Good Bye World****\n");
	
} 
module_init(add);
module_exit(remove);

MODULE_DESCRIPTION("Hello X Module");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kopal <kopalsingh.work@gmail.com>");
