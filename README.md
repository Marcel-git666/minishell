# Minishell

A simple shell implementation as part of the 42 curriculum.

## Project Overview

Minishell is a simplified version of a shell (command-line interpreter) that implements basic shell functionality:
- Command execution
- Built-in commands
- Environment variable expansion
- Redirections and pipes
- Signal handling

## Team Members

- Marcel - Lexer, parser, memory management
- Lada - Command execution, built-ins, environment handling

## Current Status

- [x] Lexical analysis (tokenization)
- [x] Memory management
- [ ] Command execution
- [ ] Built-in commands
- [ ] Redirections
- [ ] Pipes
- [ ] Environment variables
- [x] Signal handling

## Memory Management Notes

### Valgrind Suppression

The project uses a Valgrind suppression file to ignore "still reachable" memory associated with the readline library. According to the subject requirements, we are not required to fix readline memory leaks.

### Known "Still Reachable" Memory

There is a 5-byte "still reachable" memory block from a static variable `last_executed` in the `handle_input` function (shell_loop.c). This is not a true leak as it's intentionally kept alive for the duration of the program to track command history behavior, and will be reclaimed by the OS at program termination.

## Roadmap

### Marcel's Tasks
1. **Complete Lexical Analysis**
   - [x] Basic tokenization
   - [x] Handle quotes (single and double)
   - [x] Extract environment variables
   - [x] Fix memory leaks in token parsing

2. **Parser Implementation**
   - [x] Create abstract syntax tree (AST)
   - [x] Handle command separation
   - [ ] Implement syntax validation

3. **Error Handling**
   - [x] Implement error messages
   - [x] Add syntax error detection
   - [ ] Create graceful error recovery

4. **History Management**
   - [x] Implement command history
   - [ ] Handle history file operations

5. **Signal Handling Support**
   - [x] Set up signal handlers
   - [x] Implement behavior for Ctrl+C, Ctrl+D, Ctrl+\

6. **Environment Variable Management**
   - [ ] Environment variable storage
   - [ ] Variable expansion
   - [ ] Handle $? special variable

### Lada's Tasks
1. **Command Execution**
   - [ ] Implement path resolution
   - [ ] Handle command execution (execve)
   - [ ] Manage process forking

2. **Built-in Commands**
   - [x] echo with -n option
   - [x] cd with relative/absolute paths
   - [x] pwd
   - [x] export
   - [x] unset
   - [x] env
   - [x] exit

3. **Redirection Implementation**
   - [ ] Input redirection (<)
   - [ ] Output redirection (>)
   - [ ] Append output (>>)
   - [ ] Heredoc (<<)

4. **Pipes**
   - [ ] Implement pipe functionality
   - [ ] Handle multiple pipes

## How to Run

### Normal Mode

```bash
./run-minishell.sh normal
```

### Valgrind Mode (For Memory Checking)

```bash
./run-minishell.sh valgrind
```

### Valgrind with Readline Suppression

```bash
./run-minishell.sh suppressed
```

## Useful Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Writing Your Own Shell](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [AST-based Parsing](https://en.wikipedia.org/wiki/Abstract_syntax_tree)
