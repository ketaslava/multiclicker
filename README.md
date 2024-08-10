# Multiclicker
Multiclicker for Linux Ubuntu 24 on Wayland

# Developers
Made by Ketaslava Ket and KTVINCCO Team

# License
Licensed under MIT License

# About
* Writen in C++
* Mede for Ubuntu 24 on Wayland
* Up to 30 CPS on average system
* Uses libinput for detect clicks
* Uses /dev/eventX file stream to emulate clicks
* Does not require a kernel patch
* Compiled by g++

# How it works
* Activates automaticaly with quick clicks (just click fast and the program will help you click even faster)

# Setup
1. Install g++ and libinput-dev. Run this command in terminal$: sudo apt-get install g++ libinput-dev
1. Run lsinput. In the terminal run$: sudo lsinput
2. Find the device name in list. Like (name: "USB OPTICAL MOUSE")
3. Copy "USB OPTICAL MOUSE"
4. Paste "USB OPTICAL MOUSE" to the config/device_name.txt file and save it

# Run
1. Use "sudo sh ./run.sh" to build and run program. Also you can use "sudo ./dist/main" to run program instantly

# Troubleshooting
* You can adjust settings in src/main.cpp file
* If there are no clicks, it is recomended to increase the delay (time) between clicks
* The program only works when run with sudo
* To stabilize the number of clicks (to avoid periodic delays) you can use the mode "isUseOptimizedContinuationRule = true" in the Trigger Settings
