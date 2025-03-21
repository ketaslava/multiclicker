#!/bin/bash

# Check sudo
if [ "$(id -u)" -ne 0 ]; then
    echo "ERROR: This script requires sudo to be activated"
    echo "Run: sudo sh $0"
    exit 1
fi

# Get data
SERVICE_NAME="multiclicker"
SERVICE_FILE="/etc/systemd/system/$SERVICE_NAME.service"

# Check if service exists
if [ -f "$SERVICE_FILE" ]; then

    # Disable and delete service
    sudo systemctl stop "$SERVICE_NAME"
    sudo systemctl disable "$SERVICE_NAME"
    sudo rm "$SERVICE_FILE"
    sudo systemctl daemon-reload

    # Info
    echo "Service $SERVICE_NAME disabled and deleted"
else
    # Info
    echo "Service $SERVICE_NAME is not found"
fi
