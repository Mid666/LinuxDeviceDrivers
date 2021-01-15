/* Midhun Nair
   Date: 15/01/2021
    
   **Platform Device Set-up Module**
   =================================
   --Responsible for Registering the Device
    
*/

#include<linux/module.h>
#include<linux/platform_device.h>
#include"platform.h"

/*our custom platform-info data structure*/
struct pdev_platform_data my_platform_data = {

        /* all values to be filled */
        /*
        .size = ;
        .permission = ;
        .serial_number = ;
        .buffer = ;
        */
};

/*create a platform device---system framework***A /sysfs directory entry will also be created 
  -- /sys/devices/platform/my_platform_device  is set up*/
struct platform_device my_platform_device = 
{
        .name = "my_platform_device-stm32f4x";
        .id = 0;
        /*feeds device_data to this "struct device" member*/
        .dev = {
                  .platform_data = &my_platform_data;
                 /* .release = ptr to callback function which frees the resources if any */
               }
};

/*Entry point***module-init method*/
static int __init my_platform_device_init(void)
{
        /*Register our platform device to the platform bus core*/
        platform_device_register(&my_platform_device);
        printk(KERN_INFO "MY DEVICE REGISTERED");
        return 0;
}

/*exit method*/
static void __exit my_platform_device_exit(void)
{
        /*Unregister our platform device to the platform bus core*/
        platform_device_unregister(&my_platform_device);
        printk(KERN_INFO "MY DEVICE UNREGISTERED");
}

module_init(my_platform_device_init);
module_exit(my_platform_device_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PLATFORM DEVICE SET UP MODULE");
MODULE_AUTHOR("Midhun Nair");

