#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
// Variables
static int number_devices = 0;
static char *arr[] = { "avalon_encoder_0", "avalon_encoder_1", "avalon_encoder_2", "avalon_encoder_3", "avalon_encoder_4", "avalon_encoder_5"};
// Prototypes
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static int encoder_probe(struct platform_device *pdev);
static int encoder_remove(struct platform_device *pdev);
static ssize_t encoder_read(struct file *file, char *buffer, size_t len, loff_t *offset);
// static ssize_t encoder_write(struct file *file, const char *buffer, size_t len, loff_t *offset);
// An instance of this structure will be created for every avalon_encoder IP in the system
struct avalon_encoder_dev
{
	struct miscdevice miscdev;
	void __iomem *regs;
	u32 encoder_value;
};
// Specify which device tree devices this driver supports
static struct of_device_id avalon_encoder_dt_ids[] =
{
	{ .compatible = "dev,avalon-encoder" },
	{ /* end of table */ }
};
// Inform the kernel about the devices this driver supports
MODULE_DEVICE_TABLE(of, avalon_encoder_dt_ids);
// Data structure that links the probe and remove functions with our driver
static struct platform_driver encoder_platform = {
	.probe = encoder_probe,
	.remove = encoder_remove,
	.driver = {
		.name = "Avalon Encoder Driver",
		.owner = THIS_MODULE,
		.of_match_table = avalon_encoder_dt_ids
	}
};
// The file operations that can be performed on the avalon_encoder character file
static const struct file_operations avalon_encoder_fops =
{
	.open = dev_open,
	.owner = THIS_MODULE,
	.read = encoder_read,
	// .write = encoder_write,
	.release = dev_release
};
// Called when the driver is installed
static int encoder_init(void)
{
	int ret_val = 0;
	pr_info("---------- Avalon Encoder ----------\n");
	pr_info(" encoder_init: Initializing the Avalon Encoder module\n");
	// Register our driver with the "Platform Driver" bus
	ret_val = platform_driver_register(&encoder_platform);
	if(ret_val != 0)
	{
		pr_err(" encoder_init: platform_driver_register returned %d\n", ret_val);
		return ret_val;
	}
	pr_info(" encoder_init: Avalon Encoder module successfully initialized!\n");
	return 0;
}
static int dev_open(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO " dev_open: Device has been opened.\n");
   return 0;
}
// Called whenever the kernel finds a new device that our driver can handle
// (In our case, this should only get called for the one instantiation of the Avalon Encoder module)
static int encoder_probe(struct platform_device *pdev)
{
	int ret_val = -EBUSY;
	struct avalon_encoder_dev *dev;
	struct resource *r = 0;
	number_devices += 1;
	// pr_info(" encoder_probe: enter\n");
	// Get the memory resources for this Encoder device
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(r == NULL)
	{
		pr_err(" encoder_probe: IORESOURCE_MEM (register space) does not exist\n");
		goto bad_exit_return;
	}
	// Create structure to hold device-specific information (like the registers)
	dev = devm_kzalloc(&pdev->dev, sizeof(struct avalon_encoder_dev), GFP_KERNEL);
	// Both request and ioremap a memory region
	// This makes sure nobody else can grab this memory region
	// as well as moving it into our address space so we can actually use it
	dev->regs = devm_ioremap_resource(&pdev->dev, r);
	if(IS_ERR(dev->regs))
		goto bad_ioremap;
	// Puts Encoder in 0 value (access the 0th register in the avalon Encoder module)
	// dev->encoder_value = 0x00;
	// iowrite32(dev->pwm_value, dev->regs);
	pr_info(" encoder_probe: Registering %s device.\n", arr[number_devices-1]);
	// Initialize the misc device (this is used to create a character file in userspace)
	dev->miscdev.minor = MISC_DYNAMIC_MINOR;    // Dynamically choose a minor number
	dev->miscdev.name = arr[number_devices-1];
	dev->miscdev.fops = &avalon_encoder_fops;
	ret_val = misc_register(&dev->miscdev);
	if(ret_val != 0)
	{
		pr_info(" encoder_probe: Couldn't register misc device :(");
		goto bad_exit_return;
	}
	// Give a pointer to the instance-specific data to the generic platform_device structure
	// so we can access this data later on (for instance, in the read and write functions)
	platform_set_drvdata(pdev, (void*)dev);
	// pr_info(" encoder_probe: exit\n");
	return 0;
	bad_ioremap:
	   ret_val = PTR_ERR(dev->regs);
	bad_exit_return:
		pr_info(" encoder_probe: bad exit :(\n");
	return ret_val;
}
// This function gets called whenever a read operation occurs on one of the character files
static ssize_t encoder_read(struct file *file, char *buffer, size_t len, loff_t *offset)
{
	int success = 0;
	/* 
	* Get the avalon_encoder_dev structure out of the miscdevice structure.
	* Remember, the Misc subsystem has a default "open" function that will set
	* "file"s private data to the appropriate miscdevice structure. We then use the
	* container_of macro to get the structure that miscdevice is stored inside of (which
	* is our avalon_encoder_dev structure that has the current encoder value).
	*/
	struct avalon_encoder_dev *dev = container_of(file->private_data, struct avalon_encoder_dev, miscdev);
	// pr_info(" encoder_read: enter\n");
	// Give the user the current encoder value
	// const void * test = 10;
	success = copy_to_user(buffer, &dev->encoder_value, sizeof(dev->encoder_value));
	// success = copy_to_user(buffer, test, sizeof(test));
	// pr_info(" encoder_read: value -> %x\n", &buffer);
	// If we failed to copy the value to userspace, display an error message
	if(success != 0)
	{
		pr_info(" encoder_read: Failed to return current encoder value to userspace\n");
		return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
	}
	// pr_info(" encoder_read: exit\n");
	return 0; // "0" indicates End of File, aka, it tells the user process to stop reading
}

// // This function gets called whenever a write operation occurs on one of the character files
// static ssize_t encoder_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
// {
// 	int success = 0;
// 	/* 
// 	* Get the avalon_encoder_dev structure out of the miscdevice structure.
// 	* Remember, the Misc subsystem has a default "open" function that will set
// 	* "file"s private data to the appropriate miscdevice structure. We then use the
// 	* container_of macro to get the structure that miscdevice is stored inside of (which
// 	* is our avalon_encoder_dev structure that has the current encoder value).
// 	*/
// 	struct avalon_encoder_dev *dev = container_of(file->private_data, struct avalon_encoder_dev, miscdev);
// 	pr_info(" encoder_write: enter\n");
// 	// Get the new encoder value (this is just the first byte of the given data)
// 	success = copy_from_user(&dev->encoder_value, buffer, sizeof(dev->encoder_value));
// 	// If we failed to copy the value from userspace, display an error message
// 	if(success != 0)
// 	{
// 		pr_info(" encoder_write: Failed to read encoder value from userspace\n");
// 		return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
// 	}
// 	else
// 	{
// 		// We read the data correctly, so update the Encoder
// 		iowrite32(dev->encoder_value, dev->regs);
// 	}
// 	// Tell the user process that we wrote every byte they sent 
// 	// (even if we only wrote the first value, this will ensure they don't try to re-write their data)
// 	pr_info(" encoder_write: exit\n");
// 	return len;
// }
static int dev_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO " dev_release: Device successfully closed.\n");
   return 0;
}
// Gets called whenever a device this driver handles is removed.
// This will also get called for each device being handled when 
// our driver gets removed from the system (using the rmmod command).
static int encoder_remove(struct platform_device *pdev)
{
	// Grab the instance-specific information out of the platform device
	struct avalon_encoder_dev *dev = (struct avalon_encoder_dev*)platform_get_drvdata(pdev);
	pr_info(" encoder_remove: enter\n");
	// Turn Encoder off
	iowrite32(0x00, dev->regs);
	// Unregister the character file (remove it from /dev)
	misc_deregister(&dev->miscdev);
	pr_info(" encoder_remove: exit\n");
	return 0;
}
// Called when the driver is removed
static void encoder_exit(void)
{
	pr_info(" encoder_exit: Avalon Encoder module exit\n");
	// Unregister our driver from the "Platform Driver" bus
	// This will cause "encoder_remove" to be called for each connected device
	platform_driver_unregister(&encoder_platform);
	pr_info(" encoder_exit: Avalon Encoder module successfully unregistered\n");
	pr_info(" ***\n");
	pr_info(" \n");
}
// Tell the kernel which functions are the initialization and exit functions
module_init(encoder_init);
module_exit(encoder_exit);
// Define information about this kernel module
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Franz Luepke <fk.luepke146@uniandes.edu.co>");
MODULE_DESCRIPTION("Exposes a character device to user space that lets users change Encoder values");
MODULE_VERSION("1.0");