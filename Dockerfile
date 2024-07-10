# Use an appropriate base image with Qt and necessary dependencies
FROM stateoftheartio/qt6:6.7-mingw-aqt

ENV DEBIAN_FRONTEND=noninteractive
# Update package list and install necessary libraries (adjust as needed)
RUN sudo apt-get update
RUN sudo apt-get install -y \
        dbus \
        mesa-utils \
        libgl1-mesa-glx \
        libpulse-mainloop-glib0 \
        libxcomposite1 \
        libxrender1 \
        libxi6 \
        libxtst6 \
        && \
    sudo apt-get clean

ENV XDG_SESSION_TYPE=x11
# Set keyboard configuration to avoid interactive prompts
RUN echo "keyboard-configuration keyboard-configuration/variant select English (US)" | sudo debconf-set-selections && \
    echo "keyboard-configuration keyboard-configuration/layout select USA" | sudo debconf-set-selections && \
    DEBIAN_FRONTEND=noninteractive && sudo apt-get install -y xorg xserver-xorg-core libxext-dev libx11-dev

RUN sudo apt-get install -y \
                 wine \
                 wine64 \
                 libwine \
                winetricks \
                xvfb \
                winbind

# Set environment variables for Qt and X11 display
ENV DISPLAY=:99
ENV QT_QPA_PLATFORM=xcb
ENV PATH /qt/6.7.0/mingw_64/bin:/qt/Tools/mingw1120_64/bin:$PATH
ENV WINEARCH=win64
# ENV WINEPREFIX=/root/.wine
RUN sudo wineboot --init
# Set a working directory
WORKDIR /app

# Copy the Qt application source code into the container
COPY . /app
RUN sudo rm -rf ./server ./cmake-build-debug ./cmake-build-release ./build

# Run CMake to configure the project and build it
RUN cmake -G "MinGW Makefiles" -Bbuild -H. && cmake --build build --config Release
#RUN cmake -G "Ninja" -Bbuild -H. && cmake --build build --config Release

# Set up Xvfb
ENV DISPLAY=:99

# Disable DPI scaling
ENV WINEDLLOVERRIDES="winex11.drv=n"
ENV WINE_DISABLE_DISPLAY_SCALING=1

# Create a wrapper script to set up the environment and run the application
RUN sudo echo '#!/bin/bash\n\
Xvfb :99 -screen 0 1024x768x16 &\n\
sleep 1\n\
export DISPLAY=:99\n\
export QT_AUTO_SCREEN_SCALE_FACTOR=0\n\
export QT_SCALE_FACTOR=1\n\
export QT_SCREEN_SCALE_FACTORS=1\n\
wine64 /app/server.exe\n'\
> /run_app.sh && sudo chmod +x /run_app.sh

# Set the entry point to our wrapper script
ENTRYPOINT ["/run_app.sh"]

# CMD Xvfb :99 -screen 0 1024x768x16 & \
#     sleep 1 && \
#     wine64 ./build/PlanetVsZombies.exe
# Command to run the Qt GUI application
# CMD ["wine","./build/PlanetVsZombies.exe"]
