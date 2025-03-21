#!/bin/bash

# Check sudo
if [ "$(id -u)" -ne 0 ]; then
    echo "ERROR: This script requires sudo to be activated"
    echo "Run: sudo sh $0"
    exit 1
fi

# Disable service to prevent incorrect behaviour
echo "Service will be disabled to prevent incorrect behaviour"
sudo sh disable_service.sh

# Compile binaries
sh compile.sh

# Run
sudo ./dist/main
