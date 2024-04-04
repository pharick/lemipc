#!/bin/bash

# Check for root privileges
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

# Function to install SDL2 and SDL2_ttf on Ubuntu
install_sdl2_ubuntu() {
	echo "Detected Ubuntu. Installing SDL2 and SDL2_ttf..."
	apt update
	apt install -y libsdl2-dev libsdl2-ttf-dev
}

# Function to install SDL2 and SDL2_ttf on Fedora
install_sdl2_fedora() {
	echo "Detected Fedora. Installing SDL2 and SDL2_ttf..."
	dnf install -y SDL2-devel SDL2_ttf-devel
}

# Check for distribution and call the appropriate installation function
if [ -f /etc/lsb-release ]; then
	install_sdl2_ubuntu
elif [ -f /etc/fedora-release ]; then
	install_sdl2_fedora
else
	echo "Unsupported Linux distribution. Exiting."
	exit 1
fi
