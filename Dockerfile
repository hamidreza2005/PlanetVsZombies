# Use an official Ubuntu base image
FROM ubuntu:24.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV DISPLAY=:1

# Install necessary dependencies and add the PPA for Qt 6
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:okirby/qt6-backports && \
    apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libgl1-mesa-glx \
    libpulse0 \
    git \
    wget \
    x11vnc \
    xvfb \
    xfce4 \
    sudo \
    qt6-base-dev \
    qt6-multimedia-dev \
    qt6-tools-dev \
    qt6-qmake \
    qt6-qml-dev \
    && rm -rf /var/lib/apt/lists/*

# Create a user and switch to it
RUN useradd -ms /bin/bash qtuser
USER qtuser
WORKDIR /home/qtuser

# Add Qt binaries to PATH
ENV PATH="/home/qtuser/Qt/6.5.0/gcc_64/bin:${PATH}"

# Create a directory for the application
RUN mkdir /app

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY --chown=qtuser:qtuser . .

# Build the application
RUN cmake -B build -S .
RUN cmake --build build

# Expose the VNC port
EXPOSE 5900

# Script to start Xvfb, window manager, and application
COPY start.sh /start.sh
RUN chmod +x /start.sh

# Set the entrypoint to the script
ENTRYPOINT ["/start.sh"]
