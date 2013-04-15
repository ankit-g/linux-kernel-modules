#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

static int __init add(void)
{
	printk("****Hello World****\n");
	return 0;
}

static void __exit remove(void)
{
	
} 
module_init(add);
module_exit(remove);

