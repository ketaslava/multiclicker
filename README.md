# Multiclicker
Multiclicker for Linux Ubuntu 24 on Wayland

# Developers
by Ketaslava Ket and KTVINCCO Team

# License
Licensed under MIT License

# About
* Mede for Ubuntu 24 on Wayland
* Up to 30 CPS on average system
* Uses libinput for track clicks
* Uses /dev/eventX file stream to emulate clicks
* Does not require a kernel patch
* Compiled by g++

# How it works
* Activates automaticaly with quick clicks (just click fast and the program will help you click even faster)

# Setup
1. In the terminal run $sudo lsinput
2. find the device name in list like (name: "USB OPTICAL MOUSE")
3. Copy "USB OPTICAL MOUSE"
4. Paste "USB OPTICAL MOUSE" in to the settings

# Run
1. Use "sudo sh ./run.sh" to build and run, or use "./dist/main" to run instantly

# Troubleshooting
* If there are no clicks, it is recomended to increase the delay (time) between clicks
* The program only works when run with sudo
* To stabilize the number of clicks (to avoid periodic delays) you can use the mode "isUseOptimizedContinuationRule = true" in the Settings Trigger section in the src/main.cpp file
