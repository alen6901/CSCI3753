

#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>



/* Define device_buffer and other global data structures you will need here */
#define buffSize 1024
#define deviceName "simple_character_device"
static int majorNumber = 240;
static char *deviceBuff;
static int openTimes = 0;
static int closeTimes = 0;
static int unreadBytes = 0;
static int bytesAvailable = 0;
static int unwrittenBytes = 0;

ssize_t pa2_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
        /* length is the length of the userspace buffer*/
        /* offset will be set to current position of the opened file after read*/
        /* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	if(length > (buffSize - *offset) || *offset < 0){
		printk(KERN_ALERT "ERROR Attempt to read beyond EOF or before BOF\n");
		return -1;
	}
	unreadBytes = copy_to_user(buffer,deviceBuff+*offset,length);
	if(unreadBytes == 0){
		printk(KERN_ALERT "%lu bytes read\n", length-unreadBytes);
	}
	else{
		printk(KERN_ALERT "ERROR bytes not copied\n");
	}
	*offset = *offset + length;	
	return length;

}



ssize_t pa2_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
        /* length is the length of the userspace buffer*/
        /* offset will be set to current position of the opened file after read*/
        /* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	bytesAvailable = buffSize - *offset;
	if(bytesAvailable < length || *offset < 0){	
		printk(KERN_ALERT "ERROR Attempt to write beyond EOF or before BOF\n");
		return -1;
        }
	else{	
		unwrittenBytes = copy_from_user(deviceBuff+*offset, buffer, length);
		if(unwrittenBytes == 0){
			printk(KERN_ALERT "%lu bytes wrote\n", length - unwrittenBytes);
		}
		else{
			printk(KERN_ALERT "ERROR bytes not copied\n");
		}
	}
	*offset = *offset + length;	
	return length -unwrittenBytes;
}



int pa2_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	openTimes++;
	printk(KERN_ALERT "Device was opened %d times\n", openTimes);
	return 0;
}

int pa2_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	closeTimes++;
        printk(KERN_ALERT "Device was closed %d times\n", closeTimes);
	return 0;
}

loff_t pa2_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	loff_t newFP = 0;
	switch(whence)
	{
	case 0:
		newFP = offset;
		break;
	case 1:
		newFP = pfile->f_pos + offset;
		break;
	case 2:
		newFP = buffSize - offset;
		break;
	default:
		printk(KERN_ALERT "Please select 0, 1, or 2 for whence");
		newFP = pfile->f_pos;
		return newFP;
		break;
	}
	
	if(newFP > buffSize-1 || newFP < 0){
		newFP = pfile->f_pos;
		printk(KERN_ALERT "ERROR Attempt to seek beyond EOF or before BOF\n");
		return -1;
	}	
	pfile->f_pos=newFP;	
	return 0;
}

struct file_operations pa2_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.read = pa2_char_driver_read,
	.write = pa2_char_driver_write,
	.open = pa2_char_driver_open,
	.release = pa2_char_driver_close,
	.llseek = pa2_char_driver_seek
};


static int pa2_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	deviceBuff = kmalloc(buffSize,GFP_KERNEL);
	if(register_chrdev(majorNumber, deviceName, &pa2_char_driver_file_operations) > 0){
		printk(KERN_ALERT "Registering character device failed for major number %d\n", majorNumber);
	}
	printk(KERN_ALERT "inside %s funciton\n",__FUNCTION__);
	return 0;
}

static void pa2_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(majorNumber, deviceName);
	printk(KERN_ALERT "inside %s funciton\n",__FUNCTION__);
	kfree(deviceBuff);
	deviceBuff = NULL;
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(pa2_char_driver_init);
module_exit(pa2_char_driver_exit);
