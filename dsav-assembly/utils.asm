// ============================================================================
// utils.asm - Utility Functions
// ============================================================================
// Provides common utility functions used throughout the DSAV project:
// - Delay functions for animation timing
// - Input reading and parsing
// - Wait for user keypress
// - Random number generation
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Format Strings
// ----------------------------------------------------------------------------
int_fmt:            .string "%d"
press_enter_msg:    .string "\x1b[33mPress Enter to continue...\x1b[0m"
invalid_input_msg:  .string "\x1b[31mInvalid input! Please try again.\x1b[0m\n"
input_prompt:       .string "> "

// ----------------------------------------------------------------------------
// Buffer for input operations
// ----------------------------------------------------------------------------
input_buffer:       .skip 64                // 64-byte buffer for user input

    .text
    .balign 4

// ============================================================================
// FUNCTION: delay_ms
// Pauses execution for specified milliseconds (for animation)
// Parameters:
//   w0 = milliseconds to delay
// Returns: none
// ============================================================================
    .global delay_ms
delay_ms:
    stp     x29, x30, [sp, -16]!            // Save fp and lr
    mov     x29, sp                          // Set frame pointer

    // Convert milliseconds to microseconds (multiply by 1000)
    mov     w1, 1000
    mul     w0, w0, w1                      // w0 = ms * 1000 = microseconds

    // Call usleep (C library function)
    bl      usleep

    ldp     x29, x30, [sp], 16              // Restore fp and lr
    ret

// ============================================================================
// FUNCTION: delay_us
// Pauses execution for specified microseconds
// Parameters:
//   w0 = microseconds to delay
// Returns: none
// ============================================================================
    .global delay_us
delay_us:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    bl      usleep                          // usleep takes microseconds

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: read_int
// Reads an integer from standard input
// Parameters: none
// Returns:
//   w0 = parsed integer value
//   w1 = 1 if successful, 0 if failed
// ============================================================================
    .global read_int
read_int:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp

    // Allocate space on stack for scanf result
    sub     sp, sp, 16
    mov     x1, sp                          // x1 = pointer to stack space

    // Call scanf("%d", &value)
    adrp    x0, int_fmt
    add     x0, x0, :lo12:int_fmt
    bl      scanf

    // Check return value (number of items successfully read)
    cmp     w0, 1
    b.ne    read_int_failed

    // Success: load parsed value
    ldr     w0, [sp]                        // w0 = parsed integer
    mov     w1, 1                            // w1 = success flag
    b       read_int_done

read_int_failed:
    // Clear input buffer on failure
    bl      clear_input_buffer
    mov     w0, 0                            // w0 = 0 (default value)
    mov     w1, 0                            // w1 = failure flag

read_int_done:
    add     sp, sp, 16                      // Deallocate stack space
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: read_int_range
// Reads an integer within a specified range
// Parameters:
//   w0 = minimum value (inclusive)
//   w1 = maximum value (inclusive)
// Returns:
//   w0 = validated integer value within range
// ============================================================================
    .global read_int_range
read_int_range:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]              // Save callee-saved registers

    mov     w19, w0                          // Save min value
    mov     w20, w1                          // Save max value

read_int_range_loop:
    // Print prompt
    adrp    x0, input_prompt
    add     x0, x0, :lo12:input_prompt
    bl      printf

    // Read integer
    bl      read_int
    mov     w21, w0                          // Save read value
    mov     w22, w1                          // Save success flag

    // Check if read was successful
    cmp     w22, 0
    b.eq    read_int_range_invalid

    // Check if value is in range
    cmp     w21, w19
    b.lt    read_int_range_invalid          // value < min
    cmp     w21, w20
    b.gt    read_int_range_invalid          // value > max

    // Valid input
    mov     w0, w21
    b       read_int_range_done

read_int_range_invalid:
    // Print error message
    adrp    x0, invalid_input_msg
    add     x0, x0, :lo12:invalid_input_msg
    bl      printf
    b       read_int_range_loop             // Try again

read_int_range_done:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: wait_for_enter
// Waits for user to press Enter key
// Parameters: none
// Returns: none
// ============================================================================
    .global wait_for_enter
wait_for_enter:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Position cursor at bottom of screen (row 23, column 1)
    mov     w0, 23
    mov     w1, 1
    bl      ansi_move_cursor

    // Print message
    adrp    x0, press_enter_msg
    add     x0, x0, :lo12:press_enter_msg
    bl      printf

    // Clear any buffered input
    bl      clear_input_buffer

    // Wait for Enter key
    bl      getchar

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: clear_input_buffer
// Clears the input buffer (consumes characters until newline)
// Parameters: none
// Returns: none
// ============================================================================
    .global clear_input_buffer
clear_input_buffer:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

clear_input_loop:
    bl      getchar                         // Read one character
    cmp     w0, '\n'                        // Check if newline
    b.eq    clear_input_done                // Exit if newline
    cmp     w0, -1                          // Check for EOF
    b.ne    clear_input_loop                // Continue if not EOF

clear_input_done:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: print_string
// Prints a null-terminated string
// Parameters:
//   x0 = pointer to string
// Returns: none
// ============================================================================
    .global print_string
print_string:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    bl      printf                          // printf handles null-terminated strings

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: print_int
// Prints an integer value
// Parameters:
//   w0 = integer to print
// Returns: none
// ============================================================================
    .global print_int
print_int:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    mov     w1, w0                          // Move value to w1 (2nd arg)
    adrp    x0, int_fmt
    add     x0, x0, :lo12:int_fmt
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: print_newline
// Prints a newline character
// Parameters: none
// Returns: none
// ============================================================================
    .global print_newline
print_newline:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, .Lnewline
    add     x0, x0, :lo12:.Lnewline
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

    .section .rodata
.Lnewline: .string "\n"
    .text

// ============================================================================
// FUNCTION: get_random
// Generates a pseudo-random number using rand() from C library
// Parameters:
//   w0 = maximum value (exclusive)
// Returns:
//   w0 = random number in range [0, max)
// ============================================================================
    .global get_random
get_random:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]                   // Save x19

    mov     w19, w0                          // Save max value

    bl      rand                            // Call C rand() function

    // Calculate: random_value % max
    udiv    w1, w0, w19                     // w1 = rand() / max
    msub    w0, w1, w19, w0                 // w0 = rand() - (w1 * max) = rand() % max

    ldr     x19, [sp, 16]                   // Restore x19
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: seed_random
// Seeds the random number generator with current time
// Parameters: none
// Returns: none
// ============================================================================
    .global seed_random
seed_random:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Call time(NULL) to get current time
    mov     x0, 0                            // NULL argument
    bl      time

    // Use time as seed for srand()
    bl      srand

    ldp     x29, x30, [sp], 16
    ret
