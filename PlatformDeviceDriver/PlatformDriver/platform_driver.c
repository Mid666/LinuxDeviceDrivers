/*
    Midhun Nair
    
    This is a generic framework which can be used to write linux platform driver
    
    Date: 17/01/2021
*/

#include<linux/module.h>
#include<linux/device.h>
#include"platform.h"

struct  my_pdevice_private_data
{
    struct my_platform_data pdata;
    char *buffer;
    dev_t dev_num;  //holds device number-indexing
    struct cdev cdev;
}

loff_t pcd_lseek(struct file* filep, loff_t offset, int whence)
{
    return 0;
}


size_t pcd_read(struct file* filep, const char __user* buff, size_t count, loff_t fpos)
{
    return 0;
}

size_t pcd_write(struct file* filep, const char __user* buff, size_t count, loff_t fpos)
{
    return 0;
}

int pcd_open(struct inode* inode, struct file* filep)
{
    return 0;
}

int pcd_release(struct inode* inode, struct file* filep)
{
    return 0;
}

struct file_opertions pcd_fops = {
    .open = pcd_open,
    .read = pcd_read,
    .write = pcd_write,
    .llseek = pcd_lseek,
    .release = pdd_release,
    .owner = THIS_MODULE
}

int my_platform_driver_probe(struct platform_device *pdev)
{    
    pr_info("device detected");
                                                               
    /*get platform data*/
    struct my_platform_data *pdata;
    pdata = pdev->dev.platform_data;
    //pdata = (struct my_platform_data *)dev_get_platform(&pdev->dev);
    
    /*dynamically allocate memory for device private data*/
    struct my_pdevice_private_data dev_data;
    dev_data = kzalloc(sizeof(*dev_data), GFP_KERNEL);
    if(!dev_data)
        return -ENOMEM;
    
    /*dynamically allocate memory for device buffer using size information from platform data*/
    dev_data->buffer = kzalloc(dev_data->pdata.size, GFP_KERNEL);
    if(!dev_data->buffer)
        return -ENOMEM;
    
    /*get device no.*/
    
    
    /*cdev initialization and cdev add*/
    cdev_init(&dev_data->cdev, &pcd_fops);
    
    
    /*create device file for detected evice*/
}

int my_platform_driver_remove()
{    
    /*kfree all the memories allocated by probe if it didnt use resource managed APIs*/
}


/*initializing driver instance*/
struct platform_driver my_platform_driver = {
    .probe = my_platform_driver_probe,
    .remove = my_platform_driver_remove,
    .driver = {
                .name = "my_platform_device-stm32f4x",
              }
};


static int __init my_platform_driver_init(void)
{
    /*registering driver*/
    platform_driver_register(&my_platform_driver);
    pr_info("driver registered");
    return 0;
}

static void __exit my_platform_driver_exit(void)
{
    /*unregister driver*/
    platform_driver_unregister(&my_platform_driver);
    pr_info("driver unregistered");
}

module_init(my_platform_driver_init);
module_exit(my_platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Platform Device Driver module raw example");
MODULE_AUTHOR("Midhun Nair");

