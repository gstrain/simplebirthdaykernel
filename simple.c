#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded */
int simple_init(void) {

	printk(KERN_INFO "Loading Module\n");
	
	int i;
	int d = 2;
	int m = 4;
	int y = 1995;
	for(i = 0; i < 5; i++) {
		struct birthday *person;
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		person->day = d;
		person->month = m;
		person->year = y;
		INIT_LIST_HEAD(&person->list);

		list_add_tail(&person->list, &birthday_list);	
		d++;
		m++;
		y++;
	}
	
	struct birthday *ptr;
	
	list_for_each_entry(ptr, &birthday_list, list) {
		printk("birthday: %d/%d/%d\n", ptr->month, ptr->day, ptr->year);
	}
	
	return 0;
}

/* This function is called when the module is removed */

void simple_exit(void) {
	
	struct birthday *ptr, *next;

	list_for_each_entry_safe(ptr, next, &birthday_list, list) {
		list_del(&ptr->list);
		kfree(ptr);
	}
	printk("List removed\n");
	printk(KERN_INFO "Remove Module\n");
}

/* Macros for registering module entry and exit points */

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Griffin Strain");
