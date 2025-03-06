# Minishell Lexer/Parser Implementation Roadmap

## Lexical Analysis (Tokenization) - Current Progress

- [x] Basic tokenization structure
- [x] Command identification
- [x] Argument handling
- [x] Special character detection (pipes, redirections)
- [x] Quote handling (single and double quotes)
- [x] Memory management for tokens
- [x] Environment variable recognition

## Next Steps for Lexer Completion

1. **Special Character Expansion**
   - [ ] Implement remaining special character handling
   - [ ] Handle edge cases for quotes within quotes
   - [ ] Add support for escaping quotes within strings

2. **Error Handling Improvements**
   - [ ] Better error messages for syntax errors
   - [ ] Recovery mechanisms for common errors
   - [ ] Line continuation for unclosed quotes

3. **Optimization**
   - [ ] Reduce function length to comply with Norm
   - [ ] Improve token creation efficiency
   - [ ] Enhance memory allocation pattern

## Parser Implementation

1. **Abstract Syntax Tree (AST) Design**
   - [ ] Define node structures for commands, pipes, redirections
   - [ ] Create tree building functions
   - [ ] Implement tree traversal for execution

2. **Command Parsing**
   - [ ] Group tokens into command structures
   - [ ] Handle command arguments and flags
   - [ ] Implement variable substitution

3. **Redirection Parsing**
   - [ ] Parse input redirection (<)
   - [ ] Parse output redirection (>)
   - [ ] Parse append redirection (>>)
   - [ ] Parse heredoc (<<)
   - [ ] Handle file name expansion

4. **Pipeline Parsing**
   - [ ] Connect commands with pipes
   - [ ] Handle multiple pipes in sequence
   - [ ] Manage file descriptors for piped commands

5. **Syntax Validation**
   - [ ] Check for valid command structure
   - [ ] Validate redirection syntax
   - [ ] Ensure pipes are properly formed

## Integration with Execution

1. **Executor Interface**
   - [ ] Define clear interfaces between parser and executor
   - [ ] Create command execution structures
   - [ ] Implement token-to-execution conversion

2. **Environment Variable Support**
   - [ ] Pass environment information to execution logic
   - [ ] Support variable substitution in executor
   - [ ] Handle special variables like $?

## Code Structure and Organization

1. **Module Separation**
   - [ ] Clearly separate lexer and parser code
   - [ ] Create distinct token handling functions
   - [ ] Implement clean interfaces between components

2. **Memory Management Plan**
   - [ ] Define ownership rules for tokens/AST nodes
   - [ ] Implement proper cleanup functions
   - [ ] Ensure no leaks during error conditions

3. **Testing Framework**
   - [ ] Create unit tests for lexer
   - [ ] Implement parser test cases
   - [ ] Add edge case tests for complex commands

## Implementation Timeline

### Week 1
- Complete lexer optimizations
- Begin AST structure design
- Implement basic command parsing

### Week 2
- Implement redirection parsing
- Add pipeline support
- Begin integration with execution logic

### Week 3
- Complete syntax validation
- Add error recovery
- Finalize integration with executor
- Memory management and testing

## Resources

1. **Compiler Design References**
   - "Compilers: Principles, Techniques, and Tools" (Dragon Book)
   - [Recursive Descent Parsing Tutorial](https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm)

2. **Shell Implementation Examples**
   - [Bash Parser Implementation](https://git.savannah.gnu.org/cgit/bash.git/tree/parse.y)
   - [Simple Shell Examples](https://github.com/brenns10/lsh)

3. **Memory Management**
   - [Valgrind Documentation](https://valgrind.org/docs/manual/mc-manual.html)
   - [C Memory Management Patterns](https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Debugging.html)
