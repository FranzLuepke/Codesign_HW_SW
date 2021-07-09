// Includes
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
// Define information about this kernel module
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Franz Luepke <fk.luepke146@uniandes.edu.co>");
MODULE_DESCRIPTION("Exposes a misc device to user space that lets users enable values.");
MODULE_VERSION("1.0");
#define RAM_SPAN	(1024)
#define IO_BUF_SIZE	(256)
// Variables
unsigned long int start_address;
unsigned long int end_address;
// char[] name;
static int number_devices = 0;
// Drivers array
static char *arr[] =
{
	"avalon_enable"
};
// Globals
char name[] = "Avalon Enable Driver";
// Prototypes
static ssize_t dev_open(struct inode *ip, struct file *fp);
static int dev_release(struct inode *ip, struct file *fp);
static int dev_probe(struct platform_device *pdev);
static int dev_remove(struct platform_device *pdev);
static ssize_t dev_read(struct file *fp, char __user *user_buffer, size_t len, loff_t *offset);
static loff_t dev_llseek(struct file *fp, loff_t offset, int whence);
static ssize_t dev_write(struct file *fp, const char __user *user_buffer, size_t len, loff_t *offset);
// An instance of this structure will be created for every IP component in the system
struct ip_component_dev
{
	struct miscdevice miscdev;
	void __iomem *regs;
	u8 control_value;
	unsigned char io_buf[IO_BUF_SIZE];
	// struct fpga_buffer **kbuf; /* FPGA writes, driver reads! */
};
// Specify which device tree devices this driver supports
static struct of_device_id dev_dt_ids[] =
{
	{ .compatible = "dev,avalon-enable" },
	{ /* end of table */ }
};
// Inform the kernel about the devices this driver supports
MODULE_DEVICE_TABLE(of, dev_dt_ids);
// Data structure that links the probe and remove functions with our driver
static struct platform_driver driver_platform =
{
	.probe = dev_probe,
	.remove = dev_remove,
	.driver =
	{
		.name = name,
		.owner = THIS_MODULE,
		.of_match_table = dev_dt_ids
	}
};
// The file operations that can be performed on the avalon_control character file
static const struct file_operations dev_fops =
{
	.owner = THIS_MODULE,
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
	.llseek = dev_llseek
};
// Called when the driver is installed.
static int dev_init(void)
{
	int ret_val = 0;
	pr_info("---------- %s ----------\n", name);
	pr_info("dev_init: Initializing the %s.\n", name);
	// Register our driver with the "Platform Driver" bus
	ret_val = platform_driver_register(&driver_platform);
	if(ret_val != 0)
	{
		pr_err(" dev_init: platform_driver_register returned %d\n", ret_val);
		return ret_val;
	}
	pr_info("dev_init: %s successfully initialized!\n", name);
	return ret_val;
}
// Called when device is opened.
static int dev_open(struct inode *ip, struct file *fp)
{
	pr_info(" dev_open: %s\n", "--- DEVICE OPEN ---");
	// pr_info("  dev_open: %s\n", "function.");
	// pr_info(" dev_open: %s\n", "exit.");
	return 0;
}
// Called whenever the kernel finds a new device that our driver can handle
static int dev_probe(struct platform_device *pdev)
{
	int ret_val = -EBUSY;
	struct ip_component_dev *dev;
	struct resource *r = 0;
	number_devices += 1;
	// printk(" dev_probe: %s\n", "open");
	// Get the memory resources for this Control device
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(r != NULL)
	{
		start_address = (long unsigned int) r->start;
		end_address = (long unsigned int) r->end;
		pr_info("  r->start = 0x%08lx\n", start_address);
		pr_info("  r->end = 0x%08lx\n", end_address);
		pr_info("  r->name = %s\n", r->name);
	}
	else
	{
		pr_err(" dev_probe: %s\n", "IORESOURCE_MEM (register space) does not exist");
		goto bad_exit_return;
	}
	// Create structure to hold device-specific information (like the registers)
	dev = devm_kzalloc(&pdev->dev, sizeof(struct ip_component_dev), GFP_KERNEL);
	// Both request and ioremap a memory region
	dev->regs = devm_ioremap_resource(&pdev->dev, r);
	if(IS_ERR(dev->regs))
		goto bad_ioremap;
	// pr_info("  dev_probe: Registering %s device. [%d]\n", arr[number_devices-1], number_devices);
	// Initialize the misc device (this is used to create a character file in userspace)
	dev->miscdev.minor = MISC_DYNAMIC_MINOR;    // Dynamically choose a minor number
	dev->miscdev.name = arr[number_devices-1];
	dev->miscdev.fops = &dev_fops;
	ret_val = misc_register(&dev->miscdev);
	if(ret_val != 0)
	{
		pr_info(" dev_probe: Couldn't register misc device :(");
		goto bad_exit_return;
	}
	else
	{
		pr_info(" dev_probe: %s successfully registered!", arr[number_devices-1]);
	}
	// Give a pointer to the instance-specific data to the generic platform_device structure
	// so we can access this data later on (for instance, in the read and write functions)
	platform_set_drvdata(pdev, (void*)dev);
	// printk(" dev_probe: %s\n", "close");
	return ret_val;
	bad_exit_return:
		pr_info(" dev_probe: %s\n", "bad exit");
	bad_ioremap:
		pr_info(" dev_probe: %s\n", "bad ioremap");
		ret_val = PTR_ERR(dev->regs);
	return 0;
}
// Called when reading from the device.
static ssize_t dev_read(struct file *fp, char __user *user_buffer, size_t len, loff_t *offset)
{
	int success = 0;
	// unsigned int value = 0;
	struct ip_component_dev *dev = container_of(fp->private_data, struct ip_component_dev, miscdev);
	// pr_info("  dev_read: open\n");
	pr_info("  dev_write: READ\n");
	// pr_info("   dev_read: len = %d\n", len);
	pr_info("   dev_read: offset = %lld\n", *offset);
	// pr_info("   dev_read: len regs = %d\n", (int) sizeof(dev->regs));
	// Get the ip_component_dev structure out of the miscdevice structure.
	dev->control_value = ioread32(dev->regs + *offset);
	pr_info("   dev_read: value = %d\n", (int) dev->control_value);
	// pr_info("   dev_read: %d\n", dev->miscdev.mode);
	success = copy_to_user(user_buffer, &dev->control_value, sizeof(dev->control_value));
	if(success != 0)
	{
		pr_info("   dev_read: Failed to return current value to userspace.\n");
		return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
	}
	pr_info("  dev_read: exit\n");
	return 0;
}
// Called when requiring an offset
static loff_t dev_llseek(struct file *fp, loff_t offset, int whence)
{
	// struct ip_component_dev *dev = container_of(fp->private_data, struct ip_component_dev, miscdev);
	loff_t max_offset = RAM_SPAN;
	loff_t next_offset;
	pr_info("  dev_write: SEEK\n");
	// pr_info("  dev_llseek: enter\n");
	pr_info("   dev_llseek: offset = %lld\n", offset);
	pr_info("   dev_llseek: whence = %d\n", whence);
	switch(whence)
	{
		case SEEK_SET:
			next_offset = offset;
			break;
		case SEEK_CUR:
			next_offset = fp->f_pos + offset;
			break;
		case SEEK_END:
			next_offset = max_offset;
			break;
		default:
			pr_info("   dev_llseek: bad whence exit\n");
			return -EINVAL;
	}
	if (next_offset < 0)
	{
		pr_info("   dev_llseek: negative offset exit\n");
		return -EINVAL;
	}
	if (next_offset > max_offset)
		next_offset = max_offset;
	fp->f_pos = next_offset;
	pr_info("  dev_llseek: exit\n");
	return next_offset;
}
// Called when writing to the device.
static ssize_t dev_write(struct file *fp, const char __user *user_buffer, size_t len, loff_t *offset)
{
	int success = 0;
	struct ip_component_dev *dev = container_of(fp->private_data, struct ip_component_dev, miscdev);
	pr_info("  dev_write: WRITE\n");
	pr_info("  dev_write: len = %d\n", len);
	pr_info("  dev_write: offset = %lld\n", *offset);
	// Get ip_component_dev structure out of the miscdevice structure.
	success = copy_from_user(&dev->control_value, user_buffer, sizeof(dev->control_value));
	pr_info("   dev_write: value = %d\n", dev->control_value);
	if(success != 0)
	{
		pr_info("  dev_write: Failed to read value from userspace.\n");
		return -EFAULT;
	}
	else
	{
		iowrite32(dev->control_value, dev->regs + *offset);
	}
	return len;
}
// Called when the device is released.
static int dev_release(struct inode *ip, struct file *fp)
{
	pr_info(" dev_release: %s\n", "enter.");
	// pr_info("  dev_release: %s\n", "function.");
	// pr_info(" dev_release: %s\n", "exit.");
	pr_info(" dev_open: %s\n", "--- DEVICE RELEASE ---");
	return 0;
}
// Gets called whenever a device this driver handles is removed.
static int dev_remove(struct platform_device *pdev)
{
	// Grab the instance-specific information out of the platform device
	struct ip_component_dev *dev = (struct ip_component_dev*)platform_get_drvdata(pdev);
	// pr_info(" dev_remove: enter\n");
	pr_info(" dev_remove: Unregistering %s device...\n", dev->miscdev.name);
	misc_deregister(&dev->miscdev);
	// pr_info(" dev_remove: exit\n");
	return 0;
}
// Called when the driver is removed.
static void dev_exit(void)
{
	pr_info("dev_exit: Unregistering %s.\n", name);
	// Unregister our driver from the "Platform Driver" bus
	platform_driver_unregister(&driver_platform);
	pr_info("dev_exit: %s successfully unregistered\n\n", name);
}
// Tell the kernel which functions are the initialization and exit functions
module_init(dev_init);
module_exit(dev_exit);