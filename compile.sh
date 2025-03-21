#!/bin/bash

# Remove old binaries
rm -fr dist/*

# Compile
g++ src/main.cpp -o dist/main -linput

# Get data
USERNAME=${SUDO_USER:-$(whoami)}

# Change owner and premissions
sudo chown "$USERNAME:$USERNAME" dist/main || { echo "ERROR: unable to change binaries owner"; exit 1; }
sudo chmod 755 dist/main || { echo "ERROR: unable to change binaries access"; exit 1; }

# Check compilation
if [ ! -x "dist/main" ]; then
    echo "== COMPILATION ERROR! =="
    exit 1
fi

