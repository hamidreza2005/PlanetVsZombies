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

 #Add swap memory
#RUN fallocate -l 2G /swapfile && \
#    chmod 777 /swapfile && \
#    mkswap /swapfile && \
#    swapon /swapfile && \
#    echo '/swapfile none swap sw 0 0' >> /etc/fstab

WORKDIR /app
# Copy your Qt application into the container
COPY . /app
RUN rm -r ./build/*
#RUN mkdir -p /home/hamidreza/Qt/6.7.2/gcc_64/lib/cmake/
#COPY /home/hamidreza/Qt/6.7.2/gcc_64/lib/cmake/* /home/hamidreza/Qt/6.7.2/gcc_64/lib/cmake/*
# RUN cmake -Bbuild -H. && cmake --build build --config Release
RUN cmake -G "Ninja" -Bbuild -H. && cmake --build build --config Release
# Run the Qt application
CMD ["./build/PlanetVsZombies"]
