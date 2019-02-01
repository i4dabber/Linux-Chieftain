#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct gpio_dev {
  int no;   // GPIO number
  int dir; // 0: in, 1: out
};
static struct gpio_dev gpio_devs[2] = {{12,0}, {21, 1}};
static int gpio_len = 2;

struct cdev plat_drv_cdev;
struct file_operations plat_drv_fops;
static dev_t devno;
struct class *plat_drv_class;
static struct platform_driver plat_drv_platform_dr;


#define SW1_MINOR 0
#define GPIO 26
#define N_DEVICES 255

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Morten Degn-Pedersen,Berat Kaya");
MODULE_DESCRIPTION("Driver for plat_drv in ASE HAT");

 static int __plat_drv_init(void){
     
    printk("This is plat_drv driver\n");
    int err=0;
    // gpio request
   

        // allokere et major nummer 
    err = alloc_chrdev_region(&devno,SW1_MINOR,N_DEVICES,"plat_drv_driver");
    if(err<0)
            goto err_nothing;
    
    
    cdev_init(&plat_drv_cdev,&plat_drv_fops);
    err=cdev_add(&plat_drv_cdev,devno,N_DEVICES);
    if(err<0) 
            goto err_unregister;
    // laver en folder som hedder plat_drv_class
    
    plat_drv_class = class_create(THIS_MODULE,"plat_drv_class");
    if(IS_ERR(plat_drv_class)) 
            goto err_dl;
    
    err = platform_driver_register(&plat_drv_platform_dr);
    
    if(err<0)
            goto err_destroy;;
            

   
    
    return 0;
    
    // Remove device if error occured
    err_destroy:
            printk(KERN_ALERT"failed to register driver\n");
            class_destroy(plat_drv_class);
    err_dl: 
            printk(KERN_ALERT"failed to create class\n");
            cdev_del(&plat_drv_cdev);
    err_unregister:
            printk("register_chrdev_region failed\n");
            unregister_chrdev_region(devno,N_DEVICES);
    
   
    
    err_nothing: 
            printk(KERN_ALERT"EPIC FAIL\n");
            
            return err;
}


static void __plat_drv_exit(void){
    // destruere vores klasse igen
    platform_driver_unregister(&plat_drv_platform_dr);
	class_destroy(plat_drv_class);
    
	cdev_del(&plat_drv_cdev);
	unregister_chrdev_region(MKDEV(MAJOR(devno),0),N_DEVICES);
	//gpio_free(GPIO);
    //printk("your device is Removed\n");
}
    




    

int plat_drv_open(struct inode *inode, struct file *filep)
 {
     
 int major, minor;
 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
 return 0;
 }

 int plat_drv_release(struct inode *inode, struct file *filep)
 {
     
 int minor, major;

 major = MAJOR(inode->i_rdev);
 minor = MINOR(inode->i_rdev);
 printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);

 return 0;
 }


ssize_t plat_drv_read(struct file *filep, char __user *buf, 
            size_t count, loff_t *f_pos)
 {
     
     int err=0;
     char data_buf[2];
     
     int m = iminor(filep->f_inode);
     int val=gpio_get_value(gpio_devs[m].no);
     
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
 ssize_t plat_drv_write(struct file *filep, const char __user *ubuf, 
            size_t count, loff_t *f_pos)
 {
     int m = iminor(filep->f_inode);
    int err;
    char val[count];
    err=copy_from_user(val, ubuf, count);
   
    int val_input;
    err=sscanf(val,"%d",&val_input);
    gpio_set_value(gpio_devs[m].no, val_input);
    if(err!=0) return count-err;
    
 return count;
 }
 
 
 static int plat_drv_probe(struct platform_device *pdev)
{
    int err=0;
    int i=0;
     printk("hello from probe\n");
     printk(KERN_DEBUG "New Platform device: %s\n", pdev->name);
      struct device *plat_drv_device;

     for(;i<gpio_len;i++){
     err = gpio_request(gpio_devs[i].no, "my_p_dev_gpio");
     if(err<0)         
            goto err_exit;
     
     
      err=gpio_direction_output(gpio_devs[i].no, gpio_devs[i].dir);
      if(err<0) 
            goto err_direc;
     
	// fix a sw problem copied from whyislinux
	 gpio_set_value(gpio_devs[i].no, 1);
     
	 plat_drv_device = device_create(plat_drv_class,0,MKDEV(MAJOR(devno),i),0,"my_plat%d",gpio_devs[i].no);
     
     if(IS_ERR(plat_drv_device)) 
            goto err_direc;
     
     }
    return 0;
    
    
    
 
    
    err_direc: 
    for(i=0;i<gpio_len;i++){
            printk("set direction of device failed\n");
            gpio_free(gpio_devs[i].no);
    
    err_exit: 
            printk("gpio request failed\n");
    }
    return err;
	 
	 
}
static int plat_drv_remove(struct platform_device *pdev)
{
    int i=0;
    printk (KERN_ALERT "Removing device %s\n", pdev->name); 
    for(;i<gpio_len;i++){
      

        gpio_free(gpio_devs[i].no);
        device_destroy(plat_drv_class,MKDEV(MAJOR(devno),i));
    
    
    }
    printk("Goodbye from probe\n");
    
return 0;

}

static const struct of_device_id plat_drv_platform_match[] =
{
{ .compatible = "ase, plat_drv",}, {},
};   

static struct platform_driver plat_drv_platform_dr ={ 
    
    .remove = plat_drv_remove,
    .probe = plat_drv_probe,
    .driver = {
                .name="plat_drv",
                .of_match_table = plat_drv_platform_match,
                .owner = THIS_MODULE,
        },
    
};

module_init(__plat_drv_init);
module_exit(__plat_drv_exit);

struct file_operations plat_drv_fops=
{
  read: plat_drv_read,
  open: plat_drv_open,
  write: plat_drv_write,
  release: plat_drv_release,
  owner : THIS_MODULE
    
    
    
};
