#ifndef _PLATFORM_H_
#define _PLATFORM_H_

/*  Supply all device specific data to the platform driver -When we register our platfor device, 
    we have to register our custom device_private_data structure
    
    Whether a data becomes a platform_data or not depends on project.
    
    eg: permission could be a platfor specific data or may not be.
        depends on agreement between driver and device
*/
struct pdev_platform_data
{
    int size;
    int permission;  //could be platform specific data or 
    const char *serial_number;
    char *buffer;  
};

#endif
