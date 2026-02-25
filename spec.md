# SPECIFICATIONS

Arduino Nano RP2040
    - 264KB SRAM
    - 16MB FLASH MEMORY
    - Interfaces
        - USB
        - SPI
        - 12C
        - 12S
        - PIO
        - UART
    - Wifi, Bluetooth
    - Temperature Monitoring

Bread Boards
    - LEDs, displayes, potentiometer

# COOL IDEAS

Anki Remote
    - Implement macros on the buttons, like calling lspci etc.

Arduino Nano
    - Detect temperature, print to system out
    - Have a bash script like "temp" that runs the code.
    - Could do something with wifi/bluetooth

# DEFINITIONS

Linux Loadable Kernel Module - Character Device Driver
    - A Loadable Kernel Module (LKM) is like a "plugin" or a contractor. It allows you to add new code to the kernel while the system is still running, without having to reboot or recompile the entire operating system.

    - In Linux, "Everything is a file." To talk to hardware, the kernel creates a special file in the /dev directory. There are two main types:

    Block Devices: Handle data in large chunks (like a Hard Drive).

    Character Devices: Handle data as a stream of bytes, one by one (like a Keyboard, a Serial Port, or a Mouse).
    
    - When you write a Character Device Driver, you are essentially writing a translator.

    The User writes "Hello" to a file in /dev/my_device.

    The Driver catches that "Hello" in the kernel, processes it, and maybe sends it to a piece of hardware.