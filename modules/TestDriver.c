#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>


static int __init TestDriver_Init(void)
{
	printk("Welcome to my driver!\n");
	
	return 0;
}

static void __exit TestDriver_Exit(void)
{
	printk("Leaving my driver!\n");

	return;
}

module_init(TestDriver_Init);
module_exit(TestDriver_Exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MABDELM");
MODULE_DESCRIPTION("Test of module signing only");
MODULE_VERSION("1.0");
