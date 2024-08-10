# Multiclicker
Multiclicker for Linux Ubuntu 24 on Wayland

by Ketaslava Ket
and KTVINCCO Team

# License
Licensed under MIT License
[https://opensource.org/license/mit]

# About
* Mede for Ubuntu 24 on Wayland
* Up to 30 CPS on average system
* Uses libinput for track clicks
* Uses /dev/eventX file stream to emulate clicks
* Does not require a kernel patch
* Compiled by g++

# How it works
* Activates automaticaly with quick clicks

# Setup
1. In the terminal run $sudo lsinput
2. find the device name in list like (name: "USB OPTICAL MOUSE")
3. Copy "USB OPTICAL MOUSE"
4. Paste "USB OPTICAL MOUSE" in to the settings

To run use "sudo sh ./run.sh" to build and run, or use "./main" when compiled

If there are no clicks, it is recomended to increase the delay (time) between clicks
Only works with sudo
