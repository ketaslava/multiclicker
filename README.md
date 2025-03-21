# Multiclicker
Multiclicker for Linux
Officially supports on Debian/Ubuntu/Pop_OS!  with Wayland and X11

# Developers
Made by Ketaslava Ket and KTVINCCO Team

# License
Licensed under MIT License

# About
* Written in C++
* Made for Debian/Ubuntu/Pop_OS! Linux on Wayland and X11
* Not depend on graphical environment
* Up to 30 CPS on average system
* Does not require a kernel patch
* Uses libinput for detect clicks
* Compiled by g++
* Uses /dev/eventX file stream to emulate clicks

# How it works
* Activates automatically with quick clicks (just click fast and the program will help you click even faster)

# Setup

Install necessary dependencies:
1. Install g++, libinput-dev, input-utils. 
Run automatic configuration: $ sudo sh setup.sh

Configure for input device:
1. Run lsinput in the terminal: $ sudo lsinput
2. Find the device (your mouse) name in list, it may looks like: "USB OPTICAL MOUSE"
3. Copy "USB OPTICAL MOUSE" (without "")
4. Paste "USB OPTICAL MOUSE" to the config/device_name.txt file and save it

# Run
* $ sudo sh run.sh

# Run as service
You can run multiclicker as a service:
* Multiclicker will continue to work after closing the terminal
* Multiclicker will automatically start after reboot

To run as a service:

* $ sudo sh enable_service.sh

To stop the service:

* $ sudo sh disable_service.sh

# Troubleshooting
* You can adjust settings in src/main.cpp file
* If there are no clicks, it is recommended to increase the delay (time) between clicks
* The program only works when run with sudo
* To stabilize the number of clicks (to avoid periodic delays) you can use the mode "isUseOptimizedContinuationRule = true" in the Trigger Settings

