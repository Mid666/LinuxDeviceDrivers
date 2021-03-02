/*
	CUSTOM I2C CLIENT DRIVER FOR STM32F4 BOARD
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/sysfs.h>
#include <linux/mod_devicetable.h>
#include <linux/log2.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

static int red;
static int green;
static int orange;
static int blue;

struct stm32f4_data {

	struct i2c_client*	client;
	dev_t 			dev;   /*For major and minor number*/
	char*			buf;
	struct class*		class;
};

/*blue led*/

static ssize_t blue_led_store(struct device *dev, struct device_attribute *attr, char *buf)
{
	int var;
	int reg;
	int ret;

	char *BUFFER = "BLUE";
	
	struct i2c_client *client = to_i2c_client(dev);

	struct stm32f4_data *data = (struct stm32f4_data *)i2c_get_clientdata(client);

	sscanf(buf, "%du", &var);
	
	red = var;	

	ret =  i2c_master_send(client, (const char *)BUFFER, 10);
	if(ret < 0)
		pr_err("i2c-transmit error\n");
		
	return ret;
}

static ssize_t blue_led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);

	int reg;
	int ret;
	
	int var = 0;
	var = blue;
			
	ret = i2c_master_recv(client, (char *)&buf, 8);
	if(ret < 0)
		pr_err("Error in receive\n");
		
	return sprintf(buf, "%d\n", var);
}

/*red led*/

static ssize_t red_led_store(struct device *dev, struct device_attribute *attr, char *buf)
{
	int var;
	int reg;
	int ret;

	char *BUFFER = "RED";
	
	struct i2c_client *client = to_i2c_client(dev);

	struct stm32f4_data *data = (struct stm32f4_data *)i2c_get_clientdata(client);

	sscanf(buf, "%du", &var);
	
	red = var;	

	ret =  i2c_master_send(client, (const char *)BUFFER, 10);
	if(ret < 0)
		pr_err("i2c-transmit error\n");
		
	return ret;
}

static ssize_t red_led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);

	int reg;
	int ret;
	
	int var = 0;
	var = red;
			
	ret = i2c_master_recv(client, (char *)&buf, 8);
	if(ret < 0)
		pr_err("Error in receive\n");
		
	return sprintf(buf, "%d\n", var);
}

/*green led*/

static ssize_t green_led_store(struct device *dev, struct device_attribute *attr, char *buf)
{
	int var;
	int reg;
	int ret;

	char *BUFFER = "GREEN";
	
	struct i2c_client *client = to_i2c_client(dev);

	struct stm32f4_data *data = (struct stm32f4_data *)i2c_get_clientdata(client);

	sscanf(buf, "%du", &var);
	
	red = var;	

	ret =  i2c_master_send(client, (const char *)BUFFER, 10);
	if(ret < 0)
		pr_err("i2c-transmit error\n");
		
	return ret;
}

static ssize_t green_led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);

	int reg;
	int ret;
	
	int var = 0;
	var = green;
			
	ret = i2c_master_recv(client, (char *)&buf, 8);
	if(ret < 0)
		pr_err("Error in receive\n");
		
	return sprintf(buf, "%d\n", var);
}

/*orange led*/

static ssize_t orange_led_store(struct device *dev, struct device_attribute *attr, char *buf)
{
	int var;
	int reg;
	int ret;

	char *BUFFER = "ORANGE";
	
	struct i2c_client *client = to_i2c_client(dev);

	struct stm32f4_data *data = (struct stm32f4_data *)i2c_get_clientdata(client);

	sscanf(buf, "%du", &var);
	
	red = var;	

	ret =  i2c_master_send(client, (const char *)BUFFER, 10);
	if(ret < 0)
		pr_err("i2c-transmit error\n");
		
	return ret;
}

static ssize_t orange_led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);

	int reg;
	int ret;
	
	int var = 0;
	var = orange;
			
	ret = i2c_master_recv(client, (char *)&buf, 8);
	if(ret < 0)
		pr_err("Error in receive\n");
		
	return sprintf(buf, "%d\n", var);
}

static DEVICE_ATTR(red_led, 0644, red_led_show, red_led_store);

static DEVICE_ATTR(green_led, 0600, green_led_show, green_led_store);

static DEVICE_ATTR(orange_led, 0600, orange_led_show, orange_led_store);

static DEVICE_ATTR(blue_led, 0600, blue_led_show, blue_led_store);


/*probe invoked after match occured*/

static int stm32f4_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	
	struct stm32f4_data* data = NULL;
	
	printk(KERN_INFO "Probe callback function is invoked..\n");

	data = devm_kzalloc(&client->dev, sizeof(struct stm32f4_data), GFP_KERNEL);
	if(!data)
		return -ENOMEM;
		
	data->buf = devm_kzalloc(&client->dev, 8, GFP_KERNEL);
	if(!(data->buf))
		return -ENOMEM;
		
	i2c_set_clientdata(client, data);
	
	ret = sysfs_create_file(&client->dev.kobj, &dev_attr_red_led.attr);
	if (ret)
		return ret;
	
	ret = sysfs_create_file(&client->dev.kobj, &dev_attr_green_led.attr);
	if (ret)
		return ret;
		
	ret = sysfs_create_file(&client->dev.kobj, &dev_attr_orange_led.attr);
	if (ret)
		return ret;
	
	ret = sysfs_create_file(&client->dev.kobj, &dev_attr_blue_led.attr);
	if (ret)
		return ret;
	
	return 0;
}

/*Remove method*/

static int stm32f4_remove(struct i2c_client *client)
{
	struct stm32f4_data*	data;
	
	printk(KERN_INFO "Remove callback function is invoked..\n");
	
	data = i2c_get_clientdata(client);

	kfree(data->buf);
	
	kfree(data);
	
	sysfs_remove_file(&client->dev.kobj, &dev_attr_red_led.attr);
	
	sysfs_remove_file(&client->dev.kobj, &dev_attr_green_led.attr);
	
	sysfs_remove_file(&client->dev.kobj, &dev_attr_orange_led.attr);

	sysfs_remove_file(&client->dev.kobj, &dev_attr_blue_led.attr);
	
	return 0;
}

static const struct of_device_id stm32f4_of_match[] = {

	{.compatible	= "stm32f4_custom", },
	{}
};

MODULE_DEVICE_TABLE(of, stm32f4_of_match);

struct i2c_device_id i2c_id_table[] = {
	
	{"stm32f4_custom", },
	{}	
};

MODULE_DEVICE_TABLE(i2c, i2c_id_table);

static struct i2c_driver stm32f4_i2c_driver = {

	.driver		 = {
				.name 		= "stm32f4_custom",
				.owner 		= THIS_MODULE,
				.of_match_table = of_match_ptr(stm32f4_of_match), 
		  	  },

	.probe 		 = stm32f4_probe,
	.remove 	 = stm32f4_remove,
	.id_table  = i2c_id_table,
};

/* Driver Initialization */

static int __init i2c_stm32f4_client_drv_init(void)
{
	/*Register with the i2c-core---Registering the driver module*/
	return i2c_add_driver(&stm32f4_i2c_driver);
}

/* Exit module */
static void __exit i2c_stm32f4_client_drv_exit(void)
{
	i2c_del_driver(&stm32f4_i2c_driver);
}

module_init(i2c_stm32f4_client_drv_init);
module_exit(i2c_stm32f4_client_drv_exit);

MODULE_DESCRIPTION("STM32F4X Slave I2C Client Driver");
MODULE_LICENSE("GPL");
