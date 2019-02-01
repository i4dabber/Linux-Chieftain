#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include<linux/gpio.h>

static struct cdev my_cdev;
struct file_operations myfops;
static int devno;


#define LED_MINOR 0
#define LED_MAJOR 63
#define GPIO 26
#define N_MINOR 1


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Morten Degn-Pedersen,Berat Keya");
MODULE_DESCRIPTION("Driver for led in ASE HAT");

 static int __mygpio_init(void){
     
    printk("This is led driver\n");
    int err=0;
    // gpio request
    err=gpio_request(GPIO,"Led");
    if(err<0) goto 
            err_exit;
    
    
    printk("GPIO request: Success\n");
    
    err=gpio_direction_output(GPIO, 0);
    if(err<0) 
            goto err_direc;
    
    printk("set gpio direction: Success\n");
    
    devno =MKDEV(LED_MAJOR,LED_MINOR);
    
    
    err=register_chrdev_region(devno,N_MINOR,"led_driver");
    
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
            gpio_free(GPIO);
    
    err_exit: 
            printk("gpio request failed\n");
            
            return err;
}


static void __mygpio_exit(void){
    
	printk("your device is Removed\n");
	cdev_del(&my_cdev);
	
	unregister_chrdev_region(devno,N_MINOR);
	gpio_free(GPIO);
    
}
    




    

int mygpio_open(struct inode *inode, struct file *filep)
 {
     
 int major, minor;
 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Opening Mygpio Device [major], [minor]: %i, %i\n", major, minor);
 return 0;
 }

 int mygpio_release(struct inode *inode, struct file *filep)
 {
     
 int minor, major;

 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Closing/Releasing Mygpio Device [major], [minor]: %i, %i\n", major, minor);

 return 0;
 }
 ssize_t mygpio_read(struct file *filep, char __user *buf, 
            size_t count, loff_t *f_pos)
 {
     
     int err=0;
     char data_buf[2];
     
     int val=gpio_get_value(GPIO);
     
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



  ssize_t mygpio_write(struct file *filep, const char __user *ubuf, 
            size_t count, loff_t *f_pos)
 {
     
    int err;
    char val[count];
    err=copy_from_user(val, ubuf, count);
   
    int val_input;
    err=sscanf(val,"%d",&val_input);
    gpio_set_value(GPIO, val_input);
    if(err!=0) return count-err;
    
 return count;
 }

     
module_init(__mygpio_init);
module_exit(__mygpio_exit);

struct file_operations myfops=
{
  write: mygpio_write,
  read: mygpio_read,
  open: mygpio_open,
  release: mygpio_release,
  owner:  THIS_MODULE
    
    
    
};
