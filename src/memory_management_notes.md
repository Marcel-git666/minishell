# Minishell Memory Management Notes

## Current Memory Status

Our Valgrind tests show the project has good memory management with:
- No "definitely lost" memory (true memory leaks)
- A small amount (5 bytes) of "still reachable" memory from a static variable
- Readline-related allocations that are permitted to remain "still reachable" per project requirements

## Memory Management Strategy

### Token Management

1. **Token Creation**
   - Each token is dynamically allocated
   - Token values are duplicated strings (using `ft_strdup`)
   - Tokens are linked together in a linked list

2. **Token Cleanup**
   - The `free_tokens` function properly traverses the list
   - Each token's value string is freed
   - The token structure itself is freed

### String Handling

1. **String Extraction**
   - Fixed potential leak in `extract_double_quoted_string`
   - Implemented proper cleanup in `extract_text_before_env_var`
   - All temporary strings now correctly freed

2. **Command History**
   - Uses `last_executed` static variable in `handle_input`
   - This causes the 5-byte "still reachable" memory
   - This is acceptable as it's reclaimed by OS at exit

### Suppression Strategy

We're using Valgrind suppressions for:
- Readline library allocations
- Terminfo library memory
- Other system libraries used for terminal handling

This allows us to focus on our own memory management without noise from third-party libraries.

## Memory Leaks Prevention

### Best Practices Implemented

1. **Ownership Rules**
   - Clear ownership transitions for allocated memory
   - Functions that allocate memory are responsible for freeing it
   - When ownership transfers, original owner no longer frees

2. **Consistent Allocation Patterns**
   - Using matching allocation/deallocation functions
   - Following "allocate early, free late" principle
   - Ensuring error paths also free resources

3. **Cleanup on Exit**
   - Free lexer tokens after processing
   - Free command history strings when appropriate
   - Properly terminate readline resources

## Remaining Memory Considerations

1. **Static Last Executed Variable**
   - 5 bytes "still reachable" in `shell_loop.c`
   - Required for command history deduplication
   - Not a true leak as program maintains reference until exit

2. **Readline Memory**
   - Per project spec, we don't need to fix readline memory leaks
   - Suppressed in Valgrind output for clarity

3. **Error Recovery**
   - All error paths should include proper cleanup
   - When adding new features, ensure cleanup happens even in error cases

## Memory Testing Methodology

1. **Regular Valgrind Checks**
   - Run with `./run-minishell.sh suppressed`
   - Watch for any "definitely lost" memory
   - Monitor "possibly lost" and "still reachable" blocks

2. **Edge Cases to Test**
   - Syntax errors in command input
   - Incomplete quotes or parentheses
   - Interrupted commands (Ctrl+C)
   - Large input processing

## Conclusion

The memory management is currently robust with no actual leaks. The small amount of "still reachable" memory is acceptable according to project requirements and standard C programming practices. When extending the codebase, maintain the current careful allocation/deallocation patterns.
