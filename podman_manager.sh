#!/bin/bash

# podman-manager.sh
# A script to help manage Podman for minishell testing

# Exit on error
set -e

# Color codes for better readability
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print usage information
show_usage() {
  echo -e "${BLUE}Usage:${NC} $0 [COMMAND]"
  echo
  echo "Commands:"
  echo "  start      - Start Podman machine"
  echo "  stop       - Stop Podman machine"
  echo "  status     - Check Podman machine status"
  echo "  test       - Test minishell with Valgrind"
  echo "  prune      - Remove unused containers and images to free space"
  echo "  cleanup    - Full cleanup (stop machine, prune, remove cached images)"
  echo "  diskusage  - Show disk usage of Podman machine"
  echo "  recreate   - Remove and recreate Podman machine with smaller size"
  echo "  help       - Show this help message"
}

# Function to start Podman machine
start_machine() {
  echo -e "${BLUE}Starting Podman machine...${NC}"
  podman machine start
  echo -e "${GREEN}Podman machine started successfully.${NC}"
}

# Function to stop Podman machine
stop_machine() {
  echo -e "${BLUE}Stopping Podman machine...${NC}"
  podman machine stop
  echo -e "${GREEN}Podman machine stopped.${NC}"
}

# Function to check status
check_status() {
  echo -e "${BLUE}Podman machine status:${NC}"
  podman machine list
}

# Function to test minishell with Valgrind
test_minishell() {
  echo -e "${BLUE}Testing minishell with Valgrind...${NC}"

  # Make sure machine is running
  if ! podman machine list | grep -q "Currently running"; then
    echo -e "${YELLOW}Podman machine is not running. Starting it now...${NC}"
    start_machine
  fi

  # Build container image
  echo -e "${BLUE}Building container image...${NC}"
  podman build -t minishell-fedora -f Containerfile .

  # Run minishell with Valgrind
  echo -e "${BLUE}Running minishell with Valgrind in container...${NC}"
  podman run --rm -it \
    -v "$(pwd):/app" \
    minishell-fedora \
    -c "cd /app && make clean && make re && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell"
}

# Function to prune unused resources
prune_resources() {
  echo -e "${BLUE}Removing unused containers and images...${NC}"
  podman container prune -f
  podman image prune -f
  echo -e "${GREEN}Unused resources removed.${NC}"
}

# Function to perform full cleanup
full_cleanup() {
  echo -e "${BLUE}Performing full cleanup...${NC}"

  # Check if machine is running
  if podman machine list | grep -q "Currently running"; then
    stop_machine
  fi

  # Prune resources
  prune_resources

  # Remove cache
  echo -e "${BLUE}Removing cached image data...${NC}"
  rm -rf ~/.local/share/containers/storage/overlay-containers
  rm -rf ~/.local/share/containers/storage/overlay-images

  echo -e "${GREEN}Full cleanup completed.${NC}"
}

# Function to show disk usage
show_disk_usage() {
  echo -e "${BLUE}Podman machine disk usage:${NC}"
  du -sh ~/.local/share/containers/podman/machine/

  echo -e "${BLUE}Podman storage breakdown:${NC}"
  du -sh ~/.local/share/containers/storage/*
}

# Function to recreate the machine with smaller size
recreate_machine() {
  echo -e "${YELLOW}WARNING: This will delete your existing Podman machine!${NC}"
  echo -e "All containers and images will be lost."
  read -p "Are you sure you want to continue? (y/n): " -n 1 -r
  echo
  if [[ $REPLY =~ ^[Yy]$ ]]; then
    # Stop and remove current machine
    if podman machine list | grep -q "Currently running"; then
      stop_machine
    fi

    echo -e "${BLUE}Removing Podman machine...${NC}"
    podman machine rm podman-machine-default -f

    # Create new machine with smaller size
    echo -e "${BLUE}Creating new Podman machine with smaller disk size...${NC}"
    podman machine init --cpus 2 --memory 2048 --disk-size 20

    echo -e "${GREEN}Podman machine recreated with 20GB disk size.${NC}"

    # Start the new machine
    start_machine
  else
    echo -e "${YELLOW}Operation cancelled.${NC}"
  fi
}

# Main case statement to handle commands
case "$1" in
  start)
    start_machine
    ;;
  stop)
    stop_machine
    ;;
  status)
    check_status
    ;;
  test)
    test_minishell
    ;;
  prune)
    prune_resources
    ;;
  cleanup)
    full_cleanup
    ;;
  diskusage)
    show_disk_usage
    ;;
  recreate)
    recreate_machine
    ;;
  help|--help|-h)
    show_usage
    ;;
  *)
    show_usage
    ;;
esac
