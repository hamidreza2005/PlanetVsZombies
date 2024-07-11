FROM oldfrostdragon/qt6-ubuntu:v1.6
USER root
# Install necessary packages for running a GUI application
RUN apt-get update && apt-get install -y \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxtst-dev \
    libxrandr-dev \
    libfreetype6-dev \
    libfontconfig1-dev \
    && rm -rf /var/lib/apt/lists/*

# Set environment variables for Qt
ENV QT_X11_NO_MITSHM=1

WORKDIR /app
# Copy your Qt application into the container
COPY . /app

RUN ls -R /app

# Ensure all .ui files are included
RUN find /app -name "*.ui"
RUN cmake -Bbuild -H. && cmake --build build --config Release
#RUN cmake -G "Ninja" -Bbuild -H. && cmake --build build --config Release
# Run the Qt application
CMD ["./build/PlanetVsZombies"]
