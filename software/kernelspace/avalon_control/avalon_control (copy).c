#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/dma-mapping.h>
// Variables
static int number_devices = 0;
static char *arr[] = { "avalon_control_0", "avalon_control_1", "avalon_control_2", "avalon_control_3", "avalon_control_4", "avalon_control_5"};
// Prototypes
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static int control_probe(struct platform_device *pdev);
static int control_remove(struct platform_device *pdev);
static ssize_t control_read(struct file *file, char *buffer, size_t len, loff_t *offset);
static ssize_t control_write(struct file *file, const char *buffer, size_t len, loff_t *offset);
static loff_t  dev_llseek(struct file *filp, loff_t off, int whence);

#define BUF_SIZE    256

struct fpga_buffer
{
	void *addr;
	dma_addr_t dma_addr;
	int end_offset; /* Counting elements, not bytes */
};
// An instance of this structure will be created for every avalon_control IP in the system
struct avalon_control_dev
{
	struct miscdevice miscdev;
	void __iomem *regs;
	// u32 control_value;
	u8 control_value;
	// struct cdev cdev;
	// struct device *dev_device;
	// u8 led;

	// u32 kbuf[BUF_SIZE];
	// struct fpga_buffer **kbuf; /* FPGA writes, driver reads! */
};
// Specify which device tree devices this driver supports
static struct of_device_id avalon_control_dt_ids[] =
{
	{ .compatible = "dev,avalon-control" },
	{ /* end of table */ }
};
// Inform the kernel about the devices this driver supports
MODULE_DEVICE_TABLE(of, avalon_control_dt_ids);
// Data structure that links the probe and remove functions with our driver
static struct platform_driver control_platform = {  
	.probe = control_probe,
	.remove = control_remove,
	.driver = {
		.name = "Avalon Control Driver",
		.owner = THIS_MODULE,
		.of_match_table = avalon_control_dt_ids
	}
};
// The file operations that can be performed on the avalon_control character file
static const struct file_operations avalon_control_fops =
{
	.open = dev_open,
	.owner = THIS_MODULE,
	.read = control_read,
	.write = control_write,
	.release = dev_release,
	.llseek = dev_llseek
};
// Called when the driver is installed
static int control_init(void)
{
	int ret_val = 0;
	pr_info("---------- Avalon Control ----------\n");
	pr_info(" control_init: Initializing the Avalon Control module\n");
	// Register our driver with the "Platform Driver" bus
	ret_val = platform_driver_register(&control_platform);
	if(ret_val != 0)
	{
		pr_err(" control_init: platform_driver_register returned %d\n", ret_val);
		return ret_val;
	}
	pr_info(" control_init: Avalon Control module successfully initialized!\n");
	return 0;
}
static int dev_open(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO " dev_open: Device has been opened.\n");
   return 0;
}
// Called whenever the kernel finds a new device that our driver can handle
// (In our case, this should only get called for the one instantiation of the Avalon Control module)
static int control_probe(struct platform_device *pdev)
{
	int ret_val = -EBUSY;
	struct avalon_control_dev *dev;
	struct resource *r = 0;
	number_devices += 1;
	printk(" control_probe: %s", "open");
	// Get the memory resources for this Control device
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(r == NULL)
	{
		pr_err("  control_probe: IORESOURCE_MEM (register space) does not exist\n");
		goto bad_exit_return;
	}
	// Create structure to hold device-specific information (like the registers)
	dev = devm_kzalloc(&pdev->dev, sizeof(struct avalon_control_dev), GFP_KERNEL);
	// Both request and ioremap a memory region
	// This makes sure nobody else can grab this memory region
	// as well as moving it into our address space so we can actually use it
	dev->regs = devm_ioremap_resource(&pdev->dev, r);
	if(IS_ERR(dev->regs))
		goto bad_ioremap;
	// Puts Control in 0 value (access the 0th register in the avalon Control module)
	// dev->control_value = 0x00;
	// iowrite32(dev->control_value, dev->regs);
	pr_info("  control_probe: Registering %s device.\n", arr[number_devices-1]);
	// Initialize the misc device (this is used to create a character file in userspace)
	dev->miscdev.minor = MISC_DYNAMIC_MINOR;    // Dynamically choose a minor number
	dev->miscdev.name = arr[number_devices-1];
	dev->miscdev.fops = &avalon_control_fops;
	ret_val = misc_register(&dev->miscdev);
	if(ret_val != 0)
	{
		pr_info(" control_probe: Couldn't register misc device :(");
		goto bad_exit_return;
	}
	// Give a pointer to the instance-specific data to the generic platform_device structure
	// so we can access this data later on (for instance, in the read and write functions)
	platform_set_drvdata(pdev, (void*)dev);
	printk(" control_probe: %s", "close");
	return 0;
	bad_ioremap:
	   ret_val = PTR_ERR(dev->regs);
	bad_exit_return:
		pr_info(" control_probe: bad exit :(\n");
	return ret_val;
}
static loff_t dev_llseek(struct file *filp, loff_t off, int whence)
{
	// // struct avalon_control_dev *dev = filp->private_data;
	loff_t newpos;
	printk(" dev_llseek: %s", "open");
	// printk(" filp: %s", filp);
	printk("  loff_t: %lld", off);
	switch(whence)
	{
		case 0:
			newpos = off;
			break;
		// case 1:
			// newpos = filp->f_pos + off;
			// break;
		// case 2:
			// newpos = dev->size + off;
		default:
			return -EINVAL;
	}
	if (newpos<0)
	{
		return -EINVAL;
	}
	// filp->f_pos = newpos;
	printk(" dev_llseek: %s", "close");
	return newpos;
}
// This function gets called whenever a read operation occurs on one of the character files
static ssize_t control_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
	int success = 0;
	// int data = 9;
	// int off = 32*data;
	/* 
	* Get the avalon_control_dev structure out of the miscdevice structure.
	* Remember, the Misc subsystem has a default "open" function that will set
	* "file"s private data to the appropriate miscdevice structure. We then use the
	* container_of macro to get the structure that miscdevice is stored inside of (which
	* is our avalon_control_dev structure that has the current control value).
	*/
	// struct avalon_control_dev *cdevp = container_of(file->private_data, struct avalon_control_dev, miscdev);
	struct avalon_control_dev *dev = container_of(file->private_data, struct avalon_control_dev, miscdev);
	printk(" control_read: %s", "open");
	// struct avalon_control_dev *cdevp = file->private_data;
	// Give the user the current control value
	success = copy_to_user(buffer, &dev->control_value, sizeof(dev->control_value));
	// success = copy_to_user(buffer, cdevp->kbuf, len);
	// pr_info(" control_read: reading %d\n",  len);
	pr_info("  control_read: reading %d\n",  sizeof(dev->control_value));
	// pr_info(" control_read: reading %lld\n",  *offset);
	// pr_info(" control_read: reading %d\n",  off);
	// pr_info(" control_read: reading %x\n", sizeof(dev->control_value));
	// pr_info(" control_read: value -> %d\n", &dev->control_value);
	// If we failed to copy the value to userspace, display an error message
	if(success != 0)
	{
		pr_info("  control_read: Failed to return current control value to userspace\n");
		return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
	}
	printk(" control_read: %s", "close");
	return 0; // "0" indicates End of File, aka, it tells the user process to stop reading
}

// This function gets called whenever a write operation occurs on one of the character files
static ssize_t control_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
	int success = 0;
	/* 
	* Get the avalon_control_dev structure out of the miscdevice structure.
	* Remember, the Misc subsystem has a default "open" function that will set
	* "file"s private data to the appropriate miscdevice structure. We then use the
	* container_of macro to get the structure that miscdevice is stored inside of (which
	* is our avalon_control_dev structure that has the current control value).
	*/
	struct avalon_control_dev *dev = container_of(file->private_data, struct avalon_control_dev, miscdev);
	pr_info(" control_write: enter\n");
	// Get the new control value (this is just the first byte of the given data)
	success = copy_from_user(&dev->control_value, buffer, sizeof(dev->control_value));
	pr_info(" control_write: writing %x\n", dev->control_value);
	pr_info(" control_write: writing %x\n", sizeof(dev->control_value));
	// If we failed to copy the value from userspace, display an error message
	if(success != 0)
	{
		pr_info(" control_write: Failed to read control value from userspace\n");
		return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
	}
	else
	{
		// We read the data correctly, so update the Control
		iowrite32(dev->control_value, dev->regs);
	}
	// Tell the user process that we wrote every byte they sent 
	// (even if we only wrote the first value, this will ensure they don't try to re-write their data)
	pr_info(" control_write: exit\n");
	return len;
}
static int dev_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO " dev_release: Device successfully closed.\n");
   return 0;
}
// Gets called whenever a device this driver handles is removed.
// This will also get called for each device being handled when 
// our driver gets removed from the system (using the rmmod command).
static int control_remove(struct platform_device *pdev)
{
	// Grab the instance-specific information out of the platform device
	struct avalon_control_dev *dev = (struct avalon_control_dev*)platform_get_drvdata(pdev);
	pr_info(" control_remove: enter\n");
	// Turn Control off
	// iowrite32(0x00, dev->regs);
	// Unregister the character file (remove it from /dev)
	misc_deregister(&dev->miscdev);
	pr_info(" control_remove: exit\n");
	return 0;
}
// Called when the driver is removed
static void control_exit(void)
{
	pr_info(" control_exit: Avalon Control module exit\n");
	// Unregister our driver from the "Platform Driver" bus
	// This will cause "control_remove" to be called for each connected device
	platform_driver_unregister(&control_platform);
	pr_info(" control_exit: Avalon Control module successfully unregistered\n");
	pr_info(" ***\n");
	pr_info(" \n");
}
// Tell the kernel which functions are the initialization and exit functions
module_init(control_init);
module_exit(control_exit);
// Define information about this kernel module
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Franz Luepke <fk.luepke146@uniandes.edu.co>");
MODULE_DESCRIPTION("Exposes a character device to user space that lets users change Control values");
MODULE_VERSION("1.0");