# If KDIR is not set, use the default kernel source directory
KDIR ?= /lib/modules/$(shell uname -r)/build
# Module name
MODULE_NAME = gamepad

# Current directory (where the Makefile and source are)
PWD := $(shell pwd)

# Object files (if you have multiple source files, list them here)
$(MODULE_NAME)-objs := gamepad.o

# Default target: build the module
all: modules

# Build the module
modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean the module (remove generated files)
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

# Install the module (load it into the kernel)
install:
	sudo insmod $(MODULE_NAME).ko

# Uninstall the module (remove it from the kernel)
uninstall:
	sudo rmmod $(MODULE_NAME)



.PHONY: all modules clean install uninstall log log-follow
