FROM --platform=linux/amd64 ubuntu:23.04
RUN apt-get update && apt-get install -y cmake g++

WORKDIR /app
EXPOSE 5678
