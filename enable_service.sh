#!/bin/bash

# Check sudo
if [ "$(id -u)" -ne 0 ]; then
    echo "ERROR: This script requires sudo to be activated"
    echo "Run: sudo sh $0"
    exit 1
fi

# Compile binaries
sh compile.sh

# Get data
SERVICE_NAME="multiclicker"
SERVICE_FILE="/etc/systemd/system/$SERVICE_NAME.service"
CURRENT_DIR="$(pwd)"
EXECUTABLE="dist/main"

# Create / Rewrite systemd service
echo "[Unit]
Description=Multiclicker

[Service]
WorkingDirectory=$CURRENT_DIR
ExecStart=$CURRENT_DIR/$EXECUTABLE
Restart=always
User=root

[Install]
WantedBy=multi-user.target" | sudo tee "$SERVICE_FILE" > /dev/null

# Reload systemd, start service
sudo systemctl daemon-reload
sudo systemctl enable "$SERVICE_NAME"
sudo systemctl restart "$SERVICE_NAME"

# Info
echo "Service $SERVICE_NAME enabled!"
