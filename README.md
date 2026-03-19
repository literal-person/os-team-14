# OS Team 4teen

Team of 4 creating a Linux Loadable Kernel Module
Character Device Driver for 8BitDo mini gamepad

# Features

Pressing a button on the gamepad runs a command in the terminal, i.e. lspci, lsmod etc.

# Instructions to run:

Navigate to /lkm_char_driver
run 'make' in terminal
run 'sudo insmod lkm.ko'

Navigate to /userspace_app
run 'make' in terminal
run 'sudo ./main'

Make sure gamepad is in D-Mode

# Optional Run

run 'sudo dmesg -W' to see our kernel messages

# Instructions to remove

Navigate to /lkm_char_driver
run 'sudo rmmod lkm'




