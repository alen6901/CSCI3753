#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage long sys_cs3753_add(int number1, int number2, int *result)
{
printk(KERN_ALERT "number1 = %d\nnumber2 = %d\n", number1, number2);

int add = number1 + number2;
printk(KERN_ALERT "result = %d\n", add);

copy_to_user(result, &add, 4); //int is 4 bytes
return 0;
}
