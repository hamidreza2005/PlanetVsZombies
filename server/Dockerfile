FROM oldfrostdragon/qt6-ubuntu:v1.6
USER root
ENV QT_QPA_PLATFORM offscreen
ENV PATH /qt/6.7.0/mingw_64/bin:/qt/Tools/mingw1120_64/bin:$PATH
# Install necessary packages for running a GUI application
#RUN apt-get update && apt-get install -y \
#    dbus

WORKDIR /app
# Copy your Qt application into the container
COPY . /app

# RUN cmake -Bbuild -H. && cmake --build build --config Release
RUN cmake -Bbuild -H. && cmake --build build --config Release
# Run the Qt application
CMD ["./build/server"]
