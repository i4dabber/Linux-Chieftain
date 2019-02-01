#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>

struct cdev my_cdev;
struct file_operations myfops;
static int devno;


#define SW1_MINOR 0
#define SW1_MAJOR 60
#define SW1_GPIO 12
#define N_MINOR 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Morten Degn-Pedersen,Berat Keya");
MODULE_DESCRIPTION("Driver for Switch1 in ASE HAT");

 static int __mygpio_init(void){
     
    printk("This is Switch1 driver\n");
    int err=0;
    // gpio request
    err=gpio_request(SW1_GPIO,"switch1");
    if(err<0) goto 
            err_exit;
    
    
    printk("GPIO request: Success\n");
    
    err=gpio_direction_input(SW1_GPIO);
    if(err<0) 
            goto err_direc;
    
    printk("set GPIO direction: Success\n");
    
    devno =MKDEV(SW1_MAJOR,SW1_MINOR);
    
    
    err=register_chrdev_region(devno,N_MINOR,"switch1_driver");
    
    cdev_init(&my_cdev,&myfops);
    
    err=cdev_add(&my_cdev,devno,N_MINOR);
    
    if(err<0)
            goto err_unregister;
            
    
   
    
    return 0;
    
    // Remove device if error occured
    
    err_unregister:
            printk("register_chrdev_region failed\n");
            unregister_chrdev_region(devno,N_MINOR);
    
    err_direc: 
            printk("set direction of device failed\n");
            gpio_free(SW1_GPIO);
    
    err_exit: 
            printk("GPIO request failed\n");
            
            return err;
}


static void __mygpio_exit(void){
    
	printk("your device is Removed\n");
	cdev_del(&my_cdev);
	
	unregister_chrdev_region(devno,N_MINOR);
	gpio_free(SW1_GPIO);
    
}
    




    

int mygpio_open(struct inode *inode, struct file *filep)
 {
     
 int major, minor;
 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
 return 0;
 }

 int mygpio_release(struct inode *inode, struct file *filep)
 {
     
 int minor, major;

 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);

 return 0;
 }


ssize_t mygpio_read(struct file *filep, char __user *buf, 
            size_t count, loff_t *f_pos)
 {
     
     int err=0;
     char data_buf[2];
     
     int val=gpio_get_value(SW1_GPIO);
     
     data_buf[0]=val+'0';
     data_buf[1]=0;
     int buflen=strlen(data_buf)+1;
     buflen=buflen >count?count:buflen;
     err=copy_to_user(buf, data_buf, buflen);
     if(err<0)
         return err;
     
    *f_pos += buflen;
    return buflen;


 }

     
module_init(__mygpio_init);
module_exit(__mygpio_exit);

struct file_operations myfops=
{
  read: mygpio_read,
  open: mygpio_open,
  release :mygpio_release,
  owner : THIS_MODULE
    
    
    
};
