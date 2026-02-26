#include <linux/ioctl.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/wait.h>

static int major;

static int button_pressed = 0;

static const struct file_operations gamepad_fops = {
  .owner = THIS_MODULE,
  .open = open_gamepad,
  .release = release_gamepad,
  .read = read_gamepad,
  .write = write_gamepad,
};


//reading and blocking until it dectects a button press
static ssize_t read_gamepad(struct file *file, char __user *buf, size_t count, loff_t *fpos){
  wait_event_interruptible(read_wait, pressed_button != 0);
}