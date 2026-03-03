#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> //for copy_to/from_user
#include <linux/ioctl.h>
#include <linux/wait.h> //DECLARE_WAIT_QUEUE_HEAD & wait_event_interruptible
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/input.h>
#include <linux/slab.h>


//name of device, variable for major number,cdev structure, device class
#define DEVICE_NAME "gamepad"
static dev_t dev_num;
static struct cdev cdev;
static struct class *gamepad_class;
static struct proc_dir_entry *proc_entry;

//done-> struct to map the gamepad buttons
struct map_buttons{
  char button_id;
  char command[256];
};
//NEED-> to define the ioctl command for mapping buttons

static int button_pressed = 0;
static unsigned char button_id = 0;
static DECLARE_WAIT_QUEUE_HEAD(read_wait);


//proc file implementation
static ssize_t stats_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    char stats[512];
    int len;
    len = snprintf("Gamepad Status: %d\n", button_id);
    // Check if the user has already read the file
    if (*ppos > 0 || count < len) {
        return 0;
    }
    // Copy the stats to userspace
    if (copy_to_user(buf, stats, len)) {
        return -EFAULT;
    }
    // Update the file position
    *ppos = len;
    return len;
}

//don't use it - just for the brief
static ssize_t stats_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    return -EINVAL;
}

static const struct proc_ops stats_proc_ops = {
    .proc_read = stats_proc_read,
    .proc_write = stats_proc_write,
};

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
  switch (cmd) {
    default:
      pr_info("Default\n");
      break;
  }

  return 0;
}

//Mark: DONE -> func gamepad_init for when module is initially loaded
static int __init gamepad_init(void){
    //allocating its device number
  if(alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME)<0){
    pr_alert("Failed to allocate device number\n");
    return -1;
  }

  //making its device class for /dev/
  gamepad_class = class_create(DEVICE_NAME);
  if(IS_ERR(gamepad_class)) {
    unregister_chrdev_region(dev_num, 1);
    return PTR_ERR(gamepad_class);
  }
  //making its device file
  device_create(gamepad_class, NULL, dev_num, NULL, DEVICE_NAME);

  //for creating the character device
  cdev_init(&cdev, &gamepad_fops);
  cdev_add(&cdev, dev_num, 1);

  // Create the proc file
  proc_entry = proc_create("stats_gamepad", 0444, NULL, &stats_proc_ops);
  if (!proc_entry) {
      pr_alert("Failed to create proc file\n");
      return -ENOMEM;
  }

  pr_info("Initialised your Gamepad. Your major number is: %d\n", dev_num);
  return 0;
}

//Cameron: DONE -> func gamepad_exit for when removing module when finished
static void __exit gamepad_exit(void){
  // Remove the proc file
  remove_proc_entry("stats_gamepad", NULL);

  device_destroy(gamepad_class, dev_num);

  class_destroy(gamepad_class);

  cdev_del(&cdev);

  unregister_chrdev_region(dev_num, 1);

  pr_info("Removed your gamepad :(\n");
}

//start and finish the lkm
module_init(gamepad_init);
module_exit(gamepad_exit);
//just general module info you can call in the terminal
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark, Cameron");
MODULE_DESCRIPTION("A gamepad Character device driver");








//figuring out the input stuff

static const struct input_device_id gamepad_id[] = {
  {
    .flags = INPUT_DEVICE_ID_MATCH_VENDOR | INPUT_DEVICE_ID_MATCH_PRODUCT,
    .vendor = 0x2dc8
    .product = 0x9020
  },
  {},
}


static struct input_handler gamepad_handler = {
  .event = gamepad_event,
  .connect = gamepad_connect,
  .disconnect = gamepad_disconnect,
  .id_table = gampad_id
  .name = "gamepad_handler",
};



static void gamepad_disconnect(struct input_handle *handle){
  pr_info("Disconnected");
  input_close_device(handle);
  input_unregister_handle(handle);
  kfree(handle);
}

static void gamepad_event(struct input_handle *handle, unsigned int type, unsigned int code, int value){
  if(type == EV_KEY && value == 1){
    button_id = (unsigned char)code;
    button_pressed = 1;
    pr_info("lkm - captured button id %d\n", code);
    wake_up_interruptible(&read_wait);
  }
}