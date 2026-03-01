#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> //for copy_to/from_user
#include <linux/ioctl.h>
#include <linux/wait.h> //DECLARE_WAIT_QUEUE_HEAD & wait_event_interruptible
#include <linux/sched.h>
#include <linux/proc_fs.h> //for proc files


//name of device, variable for major number,cdev structure
#define DEVICE_NAME "gamepad"
static int major;
static struct cdev cdev;
static struct class *gamepad_class;

//need-> struct to map the gamepad buttons



//need-> to define the ioctl command for mapping buttons


//need -> read write variables
static int button_pressed = 0;
static char button_id = 0;
static DECLARE_WAIT_QUEUE_HEAD(read_wait);


//func prototypes
static int open_gamepad(struct inode *, struct file *);
static int release_gamepad(struct inode *, struct file *);
static ssize_t read_gamepad(struct file *, char __user *, size_t, loff_t *);
static long ioctl_gamepad(struct file *, unsigned int, unsigned long);

//don't think we need this for our thing
static ssize_t write_gamepad(struct file *, const char __user *, size_t, loff_t *);


//file ops structure for func calling
static const struct file_operations gamepad_fops = {
  .owner = THIS_MODULE,
  .open = open_gamepad,
  .release = release_gamepad,
  .read = read_gamepad,
  .write = write_gamepad,
  .unlocked_ioctl = ioctl_gamepad,
  //will prolly need 1 or 2 more here
};

static int open_gamepad(struct inode *inode, struct file *file){
  pr_info("lkm - Gamepad device opened\n");
  return 0;
}

static int release_gamepad(struct inode *inode, struct file *file){
  pr_info("lkm - Gamepad device closed\n");
  return 0;
}


//reading and blocking until it dectects a button press
static ssize_t read_gamepad(struct file *file, char __user *buf, size_t count, loff_t *fpos){
  wait_event_interruptible(read_wait, button_pressed != 0);

  if(copy_to_user(buf, &button_id, 1)!=0){
    return -EFAULT;
  }

  button_pressed = 0;
  return 1;
}

//just return an error if it tries to write to our gamepad because it shouldn't be
static ssize_t write_gamepad(struct file *file, const char __user *buf, size_t count, loff_t *f_pos){
  return -EINVAL;
}

//func for calling ioctl commands
static long ioctl_gamepad(struct file *file, unsigned int cmd, unsigned long arg){

}

//func gamepad_init for when module is initially loaded
static int __init gamepad_init(void){

}

//func gamepad_exit for when removing module when finished
static void __exit gamepad_exit(void){

}


//also need some func to create a proc file with info about the device/lkm

//start and finish the lkm
module_init(gamepad_init);
module_exit(gamepad_exit);


//just general module info you can call in the terminal
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Mark, Alan, Cameron, Akram");
MODULE_DESCRIPTION("A gamepad Character device driver");
