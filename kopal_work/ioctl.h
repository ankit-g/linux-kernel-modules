#include <linux/ioctl.h>
#define IOCTL_MAGIC 'k' // defines the magic number

#define IOCTL_HELLO _IO(IOCTL_MAGIC,0) // defines our ioctl call
