# Use an Ubuntu base image
FROM ubuntu:latest

# Update package lists and install make, perf, gdb, and valgrind
RUN apt-get update && \
    apt-get install -y make \
    linux-tools-generic \
    gdb \
    valgrind \
    gcc

# Default command when the container starts
CMD ["bash"]
