#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
// Variables
static int number_devices = 0;
static char *arr[] = { "avalon_pwm_0", "avalon_pwm_1", "avalon_pwm_2", "avalon_pwm_3", "avalon_pwm_4", "avalon_pwm_5"};
// Prototypes
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static int pwm_probe(struct platform_device *pdev);
static int pwm_remove(struct platform_device *pdev);
static ssize_t pwm_read(struct file *file, char *buffer, size_t len, loff_t *offset);
static ssize_t pwm_write(struct file *file, const char *buffer, size_t len, loff_t *offset);
// An instance of this structure will be created for every avalon_pwm IP in the system
struct avalon_pwm_dev
{
    struct miscdevice miscdev;
    void __iomem *regs;
    u8 pwm_value;
};
// Specify which device tree devices this driver supports
static struct of_device_id avalon_pwm_dt_ids[] =
{
    { .compatible = "dev,avalon-pwm" },
    { /* end of table */ }
};
// Inform the kernel about the devices this driver supports
MODULE_DEVICE_TABLE(of, avalon_pwm_dt_ids);
// Data structure that links the probe and remove functions with our driver
static struct platform_driver pwm_platform = {
    .probe = pwm_probe,
    .remove = pwm_remove,
    .driver = {
        .name = "Avalon PWM Driver",
        .owner = THIS_MODULE,
        .of_match_table = avalon_pwm_dt_ids
    }
};
// The file operations that can be performed on the avalon_pwm character file
static const struct file_operations avalon_pwm_fops =
{
    .open = dev_open,
    .owner = THIS_MODULE,
    .read = pwm_read,
    .write = pwm_write,
    .release = dev_release
};
// Called when the driver is installed
static int pwm_init(void)
{
    int ret_val = 0;
    pr_info("---------- Avalon PWM ----------\n");
    pr_info(" pwm_init: Initializing the Avalon PWM module\n");
    // Register our driver with the "Platform Driver" bus
    ret_val = platform_driver_register(&pwm_platform);
    if(ret_val != 0)
    {
        pr_err(" pwm_init: platform_driver_register returned %d\n", ret_val);
        return ret_val;
    }
    pr_info(" pwm_init: Avalon PWM module successfully initialized!\n");
    return 0;
}
static int dev_open(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO " dev_open: Device has been opened.\n");
   return 0;
}
// Called whenever the kernel finds a new device that our driver can handle
// (In our case, this should only get called for the one instantiation of the Avalon PWM module)
static int pwm_probe(struct platform_device *pdev)
{
    int ret_val = -EBUSY;
    struct avalon_pwm_dev *dev;
    struct resource *r = 0;
    number_devices += 1;
    pr_info(" pwm_probe: enter\n");
    // Get the memory resources for this PWM device
    r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(r == NULL)
    {
        pr_err(" pwm_probe: IORESOURCE_MEM (register space) does not exist\n");
        goto bad_exit_return;
    }
    // Create structure to hold device-specific information (like the registers)
    dev = devm_kzalloc(&pdev->dev, sizeof(struct avalon_pwm_dev), GFP_KERNEL);
    // Both request and ioremap a memory region
    // This makes sure nobody else can grab this memory region
    // as well as moving it into our address space so we can actually use it
    dev->regs = devm_ioremap_resource(&pdev->dev, r);
    if(IS_ERR(dev->regs))
        goto bad_ioremap;
    // Puts PWM in 0 value (access the 0th register in the avalon PWM module)
    dev->pwm_value = 0x00;
    iowrite32(dev->pwm_value, dev->regs);
    pr_info(" pwm_probe: Registering %s device.\n", arr[number_devices-1]);
    // Initialize the misc device (this is used to create a character file in userspace)
    dev->miscdev.minor = MISC_DYNAMIC_MINOR;    // Dynamically choose a minor number
    dev->miscdev.name = arr[number_devices-1];
    dev->miscdev.fops = &avalon_pwm_fops;
    ret_val = misc_register(&dev->miscdev);
    if(ret_val != 0)
    {
        pr_info(" pwm_probe: Couldn't register misc device :(");
        goto bad_exit_return;
    }
    // Give a pointer to the instance-specific data to the generic platform_device structure
    // so we can access this data later on (for instance, in the read and write functions)
    platform_set_drvdata(pdev, (void*)dev);
    pr_info(" pwm_probe: exit\n");
    return 0;
    bad_ioremap:
       ret_val = PTR_ERR(dev->regs);
    bad_exit_return:
        pr_info(" pwm_probe: bad exit :(\n");
    return ret_val;
}
// This function gets called whenever a read operation occurs on one of the character files
static ssize_t pwm_read(struct file *file, char *buffer, size_t len, loff_t *offset)
{
    int success = 0;
    /* 
    * Get the avalon_pwm_dev structure out of the miscdevice structure.
    * Remember, the Misc subsystem has a default "open" function that will set
    * "file"s private data to the appropriate miscdevice structure. We then use the
    * container_of macro to get the structure that miscdevice is stored inside of (which
    * is our avalon_pwm_dev structure that has the current pwm value).
    */
    struct avalon_pwm_dev *dev = container_of(file->private_data, struct avalon_pwm_dev, miscdev);
    pr_info(" pwm_read: enter\n");
    // Give the user the current pwm value
    success = copy_to_user(buffer, &dev->pwm_value, sizeof(dev->pwm_value));
    pr_info(" pwm_read: value -> %u\n", &dev->pwm_value);
    // If we failed to copy the value to userspace, display an error message
    if(success != 0)
    {
        pr_info(" pwm_read: Failed to return current pwm value to userspace\n");
        return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
    }
    pr_info(" pwm_read: exit\n");
    return 0; // "0" indicates End of File, aka, it tells the user process to stop reading
}

// This function gets called whenever a write operation occurs on one of the character files
static ssize_t pwm_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
    int success = 0;
    /* 
    * Get the avalon_pwm_dev structure out of the miscdevice structure.
    * Remember, the Misc subsystem has a default "open" function that will set
    * "file"s private data to the appropriate miscdevice structure. We then use the
    * container_of macro to get the structure that miscdevice is stored inside of (which
    * is our avalon_pwm_dev structure that has the current pwm value).
    */
    struct avalon_pwm_dev *dev = container_of(file->private_data, struct avalon_pwm_dev, miscdev);
    pr_info(" pwm_write: enter\n");
    // Get the new pwm value (this is just the first byte of the given data)
    success = copy_from_user(&dev->pwm_value, buffer, sizeof(dev->pwm_value));
    // If we failed to copy the value from userspace, display an error message
    if(success != 0)
    {
        pr_info(" pwm_write: Failed to read pwm value from userspace\n");
        return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
    }
    else
    {
        // We read the data correctly, so update the PWM
        iowrite32(dev->pwm_value, dev->regs);
    }
    // Tell the user process that we wrote every byte they sent 
    // (even if we only wrote the first value, this will ensure they don't try to re-write their data)
    pr_info(" pwm_write: exit\n");
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
static int pwm_remove(struct platform_device *pdev)
{
    // Grab the instance-specific information out of the platform device
    struct avalon_pwm_dev *dev = (struct avalon_pwm_dev*)platform_get_drvdata(pdev);
    pr_info(" pwm_remove: enter\n");
    // Turn PWM off
    iowrite32(0x00, dev->regs);
    // Unregister the character file (remove it from /dev)
    misc_deregister(&dev->miscdev);
    pr_info(" pwm_remove: exit\n");
    return 0;
}
// Called when the driver is removed
static void pwm_exit(void)
{
    pr_info(" pwm_exit: Avalon PWM module exit\n");
    // Unregister our driver from the "Platform Driver" bus
    // This will cause "pwm_remove" to be called for each connected device
    platform_driver_unregister(&pwm_platform);
    pr_info(" pwm_exit: Avalon PWM module successfully unregistered\n");
    pr_info(" ***\n");
    pr_info(" \n");
}
// Tell the kernel which functions are the initialization and exit functions
module_init(pwm_init);
module_exit(pwm_exit);
// Define information about this kernel module
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Franz Luepke <fk.luepke146@uniandes.edu.co>");
MODULE_DESCRIPTION("Exposes a character device to user space that lets users change PWM values");
MODULE_VERSION("1.0");