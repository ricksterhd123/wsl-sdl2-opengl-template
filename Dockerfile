FROM ubuntu:latest
RUN apt update
RUN apt install -y build-essential cmake git wget
RUN apt install -y libxmu-dev libxi-dev dos2unix libgl-dev libglew-dev libsdl2-dev

WORKDIR /mdyn
COPY . .
