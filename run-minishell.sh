#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Define suppression file
SUPPRESSION_FILE="readline-suppressions.supp"

# Function to create suppression file if it doesn't exist
create_suppression_file() {
    if [ ! -f "$SUPPRESSION_FILE" ]; then
        echo -e "${BLUE}Creating Valgrind suppression file...${NC}"
        cat > "$SUPPRESSION_FILE" << 'EOF'
{
   readline_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   ...
   obj:*/libreadline.so*
   ...
}
{
   readline_realloc_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:realloc
   ...
   obj:*/libreadline.so*
   ...
}
{
   tinfo_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   ...
   obj:*/libtinfo.so*
   ...
}
{
   tinfo_realloc_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:realloc
   ...
   obj:*/libtinfo.so*
   ...
}
{
   tinfo_calloc_leak
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:calloc
   ...
   obj:*/libtinfo.so*
   ...
}
EOF
        echo -e "${GREEN}Suppression file created.${NC}"
    fi
}

# Function to start Podman
start_podman() {
    echo -e "${BLUE}Starting Podman machine...${NC}"
    podman machine start || echo -e "${YELLOW}Machine already running or failed to start${NC}"

    echo -e "${BLUE}Building container image...${NC}"
    podman build -t minishell-fedora -f Containerfile .
}

# Function to run minishell normally
run_normal() {
    echo -e "${BLUE}Running minishell normally in container...${NC}"
    podman run --rm -it \
        -v "$(pwd):/app" \
        minishell-fedora \
        -c "cd /app && make clean && make re && ./minishell"
}

# Function to run minishell with Valgrind (no suppression)
run_valgrind() {
    echo -e "${BLUE}Running minishell with Valgrind in container...${NC}"
    podman run --rm -it \
        -v "$(pwd):/app" \
        minishell-fedora \
        -c "cd /app && make clean && make re && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell"
}

# Function to run minishell with Valgrind and suppression
run_valgrind_suppressed() {
    create_suppression_file

    echo -e "${BLUE}Running minishell with Valgrind and readline suppressions in container...${NC}"
    podman run --rm -it \
        -v "$(pwd):/app" \
        minishell-fedora \
        -c "cd /app && make clean && make re && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=$SUPPRESSION_FILE ./minishell"
}

# Show usage
show_usage() {
    echo -e "${BLUE}Usage:${NC} $0 [OPTION]"
    echo
    echo "Options:"
    echo "  normal     - Run minishell normally (default)"
    echo "  valgrind   - Run with Valgrind memory checking"
    echo "  suppressed - Run with Valgrind and readline suppressions"
    echo "  help       - Show this help message"
}

# Start Podman machine
start_podman

# Process options
case "$1" in
    normal)
        run_normal
        ;;
    valgrind)
        run_valgrind
        ;;
    suppressed)
        run_valgrind_suppressed
        ;;
    help|--help|-h)
        show_usage
        ;;
    *)
        # If no option or invalid option, ask the user
        echo -e "${BLUE}How would you like to run minishell?${NC}"
        echo "1) Normal mode"
        echo "2) Valgrind memory check"
        echo "3) Valgrind with readline suppressions"
        read -p "Enter your choice (1-3): " choice

        case $choice in
            1)
                run_normal
                ;;
            2)
                run_valgrind
                ;;
            3)
                run_valgrind_suppressed
                ;;
            *)
                echo -e "${RED}Invalid choice. Running in normal mode.${NC}"
                run_normal
                ;;
        esac
        ;;
esac
