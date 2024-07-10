#!/bin/bash

# Start Xvfb (virtual framebuffer X server)
Xvfb :1 -screen 0 1024x768x24 &

# Start a window manager
startxfce4 &

# Start x11vnc server
x11vnc -display :1 -forever -nopw -listen 0.0.0.0 -xkb &

# Start the application
wine ./build/PlanetVsZombies.exe
