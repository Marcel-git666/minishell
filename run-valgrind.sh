#!/bin/bash

# Exit on error
set -e

echo "Starting Podman machine..."
podman machine start || echo "Machine already running or failed to start"

echo "Building container image..."
podman build -t minishell-fedora -f Containerfile .

echo "Running minishell with Valgrind in container..."
podman run --rm -it \
  -v "$(pwd):/app" \
  minishell-fedora \
  -c "cd /app && make clean && make re && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell"

