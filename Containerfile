FROM fedora:latest

# Install required build tools and valgrind
RUN dnf -y update && \
    dnf -y install gcc make valgrind readline-devel && \
    dnf clean all

# Create working directory
WORKDIR /app

# We'll mount the source code at runtime
# This comment is for documentation purposes only

# Set the entrypoint to bash by default
ENTRYPOINT ["/bin/bash"]
