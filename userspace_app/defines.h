#ifndef DEFINES
#define DEFINES

#ifdef __KERNEL__
  #include <linux/ioctl.h>
#else
  #include <sys/ioctl.h>
#endif

#define GAMEPAD_MAGIC_NUM 'G'
#define GAMEPAD_MAP_BUTTON      _IOW(GAMEPAD_MAGIC_NUM, 1, struct map_buttons)
#define GAMEPAD_GET_MAPPING     _IOR(GAMEPAD_MAGIC_NUM, 2, struct map_buttons)
#define GAMEPAD_GET_PRESS_COUNT _IOR(GAMEPAD_MAGIC_NUM, 3, int)
#define MAX_BUTTONS_SIZE 256

#endif