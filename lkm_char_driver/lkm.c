#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> //for copy_to/from_user
#include <linux/ioctl.h>
#include <linux/wait.h> //DECLARE_WAIT_QUEUE_HEAD & wait_event_interruptible
#include <linux/sched.h>
#include <linux/proc_fs.h>


//name of device, variable for major number,cdev structure, device class
#define DEVICE_NAME "gamepad"
static int major;
static struct cdev cdev;
static struct class *gamepad_class;

//done-> struct to map the gamepad buttons
struct button_mapper{
  char button_id;
  char command[256];
}
//NEED-> to define the ioctl command for mapping buttons

static int button_pressed = 0;
static char button_id = 0;
static DECLARE_WAIT_QUEUE_HEAD(read_wait);

//func prototypes
static int open_gamepad(struct inode *, struct file *);
static int release_gamepad(struct inode *, struct file *);
static ssize_t read_gamepad(struct file *, char __user *, size_t, loff_t *);
static long ioctl_gamepad(struct file *, unsigned int, unsigned long);
static ssize_t write_gamepad(struct file *, const char __user *, size_t, loff_t *);


//file ops structure for func calling
static const struct file_operations gamepad_fops = {
  .owner = THIS_MODULE,
  .open = open_gamepad,
  .release = release_gamepad,
  .read = read_gamepad,
  .write = write_gamepad,
  .unlocked_ioctl = ioctl_gamepad,
};

static int open_gamepad(struct inode *inode, struct file *file){
  pr_info("lkm - Gamepad device opened\n");
  return 0;
}

static int release_gamepad(struct inode *inode, struct file *file){
  pr_info("lkm - Gamepad device closed\n");
  return 0;
}


//Mark: DONE -> reading and blocking until it dectects a button press
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

//Cameron: NEED -> func for calling ioctl commands
static long ioctl_gamepad(struct file *file, unsigned int cmd, unsigned long arg){

}

//Mark: DONE -> func gamepad_init for when module is initially loaded
static int __init gamepad_init(void){
    //allocating its device number
  if(alloc_chrdev_region(&major, 0, 1, DEVICE_NAME)<0){
    pr_alert("Failed to allocate device number\n");
    return -1;
  }

  //making its device class for /dev/
  gamepad_class = class_create(THIS_MODULE, DEVICE_NAME);
  if(IS_ERR(gamepad_class)) {
    unregister_chrdev_region(major, 1);
    return PTR_ERR(gamepad_class);
  }
  //making its device file
  device_creat(gamepad_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME)

  //for creating the character device
  cdev_init(&cdev, &gamepad_fops);
  cdev_add(&cdev, MKDEV(major, 0), 1);

  //make its procfile in /proc/
  proc_create("stats_gamepad", 0, NULL, &stats_proc_ops); //need to make the proc fops struct for this to work
  pr_info("Initialised your Gamepad. Major number is: %d\n", major);
  return 0;
}

//Cameron: NEED -> func gamepad_exit for when removing module when finished
static void __exit gamepad_exit(void){

}


//NEED -> some funcs to create a proc file with info about the device/lkm

//start and finish the lkm
module_init(gamepad_init);
module_exit(gamepad_exit);
//just general module info you can call in the terminal
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark, Cameron);
MODULE_DESCRIPTION("A gamepad Character device driver");
