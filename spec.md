# SPECIFICATIONS

8BitDo Micro Bluetooth Gamepad
    - Controller Mode
        - D-input
        - Switch mode
        - Keyboard mode
    - Connectivity
        - Bluetooth
        - Wired
    - Button Mapping

# COOL IDEAS

Terminal Mode
    - Navigate through the terminal using various functions, including but not limited to:
        - Mapped arrow buttons
        - Pre-saved terminal commands
        - Copy and paste commands

Mouse Mode
    - Use the d-pad to move the mouse around by pressing the appropriate arrows to move along the axis
    - Right and left clicked mapped buttons

Keyboard Mode
    - Latin alphabet keyboard mapped to the different gamepad buttons
    - Enter functionality

All could be swapped between by using the controller mode switch.

# DEFINITIONS

Linux Loadable Kernel Module - Character Device Driver
    - A Loadable Kernel Module (LKM) is like a "plugin" or a contractor. It allows you to add new code to the kernel while the system is still running, without having to reboot or recompile the entire operating system.

    - In Linux, "Everything is a file." To talk to hardware, the kernel creates a special file in the /dev directory. There are two main types:

    Block Devices: Handle data in large chunks (like a Hard Drive).

    Character Devices: Handle data as a stream of bytes, one by one (like a Keyboard, a Serial Port, or a Mouse).
    
    - When you write a Character Device Driver, you are essentially writing a translator.

    The User writes "Hello" to a file in /dev/my_device.

    The Driver catches that "Hello" in the kernel, processes it, and maybe sends it to a piece of hardware.