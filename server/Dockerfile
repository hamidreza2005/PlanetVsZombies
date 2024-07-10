# Use the official Ubuntu base image with MinGW for Qt6
FROM stateoftheartio/qt6:6.7-mingw-aqt
#USER root
# Set environment variables for Qt and MinGW
ENV QT_QPA_PLATFORM offscreen
ENV PATH /qt/6.7.0/mingw_64/bin:/qt/Tools/mingw1120_64/bin:$PATH

# Fix permissions on apt directories
RUN sudo mkdir -p /var/lib/apt/lists/partial && \
    sudo chmod 644 /var/lib/apt/lists/partial && \
    sudo apt-get clean

# Install necessary libraries including D-Bus (if required)
RUN sudo apt-get update && \
    sudo apt-get install -y dbus

# Set a working directory
WORKDIR /app
# Switch to the non-root user
#RUN chown -R root:root /home/user/.wine

# Copy the src directory contents into the container
COPY . /app

# Run CMake to configure the project and build it
RUN cmake -G "Ninja" -Bbuild -H. && \
    cmake --build build --config Release

# Ensure the executable is correctly located
CMD ["wine", "./build/server.exe"]
