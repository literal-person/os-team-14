#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/wait.h>
#include <linux/sched.h>


//name of device, variable for major number,cdev structure
#define DEVICE_NAME "gamepad"
static int major;
static struct cdev cdev;

//need struct to map the gamepad buttons



//need to define the ioctl command for mapping buttons


//need - read write variables
static int button_pressed = 0;


//func prototypes
static int gamepad_open(Struct inode *, struct file *);
static int gamepad_release(struct inode *, struct file *);
static ssize_t read_gamepad(struct file *, const char __user *, size_t, loff_t *);
static long ioctl_gamepad(struct file *, unsigned int, unsigned long);

//don't think we need this for our thing
//static ssize_t write_gamepad(struct file *, const char __user *, size_t, loff_t *);


//file ops structure for func calling
static const struct file_operations gamepad_fops = {
  .owner = THIS_MODULE,
  .open = open_gamepad,
  .release = release_gamepad,
  .read = read_gamepad,
  .write = write_gamepad,
  //will prolly need 1 or 2 more here
};

static int gamepad _open(struct inode *inode, struct file *file){
  pr_info("lkm - Gamepad device opened\n");
  return 0;
}

static int gamepad_release(struct inode *inode, struct file *file){
  pr_info("lkm - Gamepad device closed");
  return 0;
}


//reading and blocking until it dectects a button press
static ssize_t read_gamepad(struct file *file, char __user *buf, size_t count, loff_t *fpos){
  wait_event_interruptible(read_wait, pressed_button != 0);
}

//func for calling ioctl commands
static long gamepad_ioctl(struct file *file, unsigned int cmd, unsigned long arg){

}

//func gamepad_init for when module is initially loaded
static int __init gamepad_init(void){

}

//func gamepad_exit for when removing module when finished
static void __exit gamepad_exit(void){

}



module_init(gamepad_init);
module_exit(gamepad_exit);


//just general module info you can call in the terminal
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Mark, Alan, Cameron, Akram");
MODULE_DESCRIPTION("A gamepad Character device driver");