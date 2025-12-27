// ============================================================================
// search_viz.asm - Search Algorithm Visualization Module
// ============================================================================
// Implements animated visualizations for:
//   - Linear Search (sequential scanning)
//   - Binary Search (divide-and-conquer on sorted arrays)
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// Search array storage (separate from main array)
search_array:       .skip 40                // Space for 10 integers (4 bytes each)
search_size:        .word 0                 // Current number of elements
search_delay:       .word 200               // Animation delay in milliseconds
search_target:      .word 0                 // Target value to search for

// Visualization state
current_idx:        .word -1                // Currently checking index (yellow)
found_idx:          .word -1                // Found index (green)
checked_up_to:      .word -1                // Elements checked so far (gray)
low_idx:            .word -1                // Binary search low pointer (cyan)
mid_idx:            .word -1                // Binary search mid pointer (yellow)
high_idx:           .word -1                // Binary search high pointer (cyan)

// Menu strings
menu_prompt:        .string "Enter your choice: "
menu_title:         .string "\n╔══════════════════════════════════════════════════╗\n"
menu_title2:        .string "║         SEARCH ALGORITHM VISUALIZER              ║\n"
menu_line:          .string "╠══════════════════════════════════════════════════╣\n"
menu_opt1:          .string "║   [1] Linear Search                              ║\n"
menu_opt2:          .string "║   [2] Binary Search (requires sorted array)      ║\n"
menu_opt3:          .string "║   [3] Display Array                              ║\n"
menu_opt4:          .string "║   [4] Initialize Random Array                    ║\n"
menu_opt5:          .string "║   [5] Sort Array (for Binary Search)             ║\n"
menu_opt0:          .string "║   [0] Back to Main Menu                          ║\n"
menu_bottom:        .string "╚══════════════════════════════════════════════════╝\n"

// Input prompts
prompt_target:      .string "Enter target value to search for: "
prompt_size:        .string "Enter array size (3-10): "
prompt_speed:       .string "Enter animation speed in ms (100-2500): "

// Status messages
msg_found:          .string "\x1b[32mTarget %d found at index %d!\x1b[0m\n"
msg_not_found:      .string "\x1b[31mTarget %d not found in array.\x1b[0m\n"
msg_initialized:    .string "\x1b[32mArray initialized with %d random elements.\x1b[0m\n"
msg_sorted:         .string "\x1b[32mArray sorted successfully.\x1b[0m\n"
msg_sort_prompt:    .string "\x1b[33mBinary search requires a sorted array. Sort it now? (y/n): \x1b[0m"
msg_sorting:        .string "\x1b[36mSorting array...\x1b[0m\n"
msg_empty:          .string "\x1b[31mError: Array is empty. Initialize first.\x1b[0m\n"
msg_press_enter:    .string "\x1b[36mPress Enter to start searching...\x1b[0m"
msg_checking:       .string "Checking index %d..."
msg_comparing:      .string "Comparing with mid element..."
msg_eliminating:    .string "Eliminating %s half..."

str_left:           .string "left"
str_right:          .string "right"

// Display formatting
display_header:     .string "SEARCH ARRAY VISUALIZATION"
display_index_label: .string "Index:"
display_value_label: .string "Value:"
display_target:     .string "\n  Target: %d\n"
index_fmt:          .string "%3d"
value_fmt:          .string "%3d"

// ANSI color codes for visualization
ansi_reset:         .string "\x1b[0m"
ansi_yellow_bg:     .string "\x1b[43m\x1b[30m"      // Yellow background, black text (checking)
ansi_green_bg:      .string "\x1b[42m\x1b[30m"      // Green background, black text (found)
ansi_gray_bg:       .string "\x1b[100m\x1b[37m"     // Gray background, white text (checked)
ansi_cyan_bg:       .string "\x1b[46m\x1b[30m"      // Cyan background, black text (bounds)
ansi_clear:         .string "\x1b[2J"
ansi_home:          .string "\x1b[H"

// Printf format strings
int_fmt:            .string "%d"
scan_fmt:           .string "%d"
char_fmt:           .string "%c"

    .text
    .balign 4

    .section .rodata
.Lclear_line:       .string "\x1b[2K"       // Clear entire line
    .text

// ============================================================================
// search_menu - Main search algorithm menu
// ============================================================================
    .global search_menu
search_menu:
    define(choice, w19)

    stp     x29, x30, [sp, -32]!            // Save frame pointer and link register
    mov     x29, sp                         // Set up frame pointer
    str     x19, [sp, 16]                   // Save x19 (callee-saved)

search_menu_loop:
    // Clear screen and display menu
    adrp    x0, ansi_clear                  // Load clear screen ANSI code
    add     x0, x0, :lo12:ansi_clear
    bl      printf

    adrp    x0, ansi_home                   // Move cursor to home
    add     x0, x0, :lo12:ansi_home
    bl      printf

    // Print menu
    adrp    x0, menu_title                  // Print title
    add     x0, x0, :lo12:menu_title
    bl      printf

    adrp    x0, menu_title2
    add     x0, x0, :lo12:menu_title2
    bl      printf

    adrp    x0, menu_line
    add     x0, x0, :lo12:menu_line
    bl      printf

    adrp    x0, menu_opt1                   // Print options
    add     x0, x0, :lo12:menu_opt1
    bl      printf

    adrp    x0, menu_opt2
    add     x0, x0, :lo12:menu_opt2
    bl      printf

    adrp    x0, menu_opt3
    add     x0, x0, :lo12:menu_opt3
    bl      printf

    adrp    x0, menu_opt4
    add     x0, x0, :lo12:menu_opt4
    bl      printf

    adrp    x0, menu_opt5
    add     x0, x0, :lo12:menu_opt5
    bl      printf

    adrp    x0, menu_opt0
    add     x0, x0, :lo12:menu_opt0
    bl      printf

    adrp    x0, menu_line
    add     x0, x0, :lo12:menu_line
    bl      printf

    adrp    x0, menu_bottom
    add     x0, x0, :lo12:menu_bottom
    bl      printf

    // Get user choice
    adrp    x0, menu_prompt                 // Print prompt
    add     x0, x0, :lo12:menu_prompt
    bl      printf

    mov     w0, 0                           // Min value
    mov     w1, 5                           // Max value
    bl      read_int_range                  // Read choice with validation
    mov     choice, w0                      // Save choice

    // Process choice
    cmp     choice, 0
    b.eq    search_menu_exit                // Exit if 0

    cmp     choice, 1
    b.eq    search_menu_linear              // Linear search

    cmp     choice, 2
    b.eq    search_menu_binary              // Binary search

    cmp     choice, 3
    b.eq    search_menu_display             // Display array

    cmp     choice, 4
    b.eq    search_menu_init                // Initialize array

    cmp     choice, 5
    b.eq    search_menu_sort                // Sort array

    b       search_menu_loop                // Invalid choice, loop

search_menu_linear:
    bl      search_run_linear               // Run linear search
    bl      wait_for_enter                  // Wait for user
    b       search_menu_loop

search_menu_binary:
    bl      search_run_binary               // Run binary search
    bl      wait_for_enter                  // Wait for user
    b       search_menu_loop

search_menu_display:
    // Check if array is initialized first
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    search_menu_display_empty

    bl      search_display_array            // Display array
    bl      wait_for_enter                  // Wait for user
    b       search_menu_loop

search_menu_display_empty:
    // Clear screen
    adrp    x0, ansi_clear
    add     x0, x0, :lo12:ansi_clear
    bl      printf

    // Position cursor and show message
    mov     w0, 10
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf

    bl      wait_for_enter
    b       search_menu_loop

search_menu_init:
    bl      search_initialize_array         // Initialize random array
    bl      wait_for_enter                  // Wait for user
    b       search_menu_loop

search_menu_sort:
    bl      search_sort_array               // Sort array for binary search
    bl      wait_for_enter                  // Wait for user
    b       search_menu_loop

search_menu_exit:
    ldr     x19, [sp, 16]                   // Restore x19
    ldp     x29, x30, [sp], 32              // Restore frame pointer and link register
    ret

    undefine(`choice')

// ============================================================================
// search_initialize_array - Initialize array with random values
// ============================================================================
search_initialize_array:
    define(size, w19)
    define(counter, w20)
    define(array_ptr, x21)

    stp     x29, x30, [sp, -48]!            // Save registers
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    // Get array size from user
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, prompt_size
    add     x0, x0, :lo12:prompt_size
    bl      printf

    mov     w0, 3                           // Min size
    mov     w1, 10                          // Max size
    bl      read_int_range
    mov     size, w0                        // Save size

    // Store size
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    str     size, [x0]

    // Get array pointer
    adrp    array_ptr, search_array
    add     array_ptr, array_ptr, :lo12:search_array

    // Initialize counter
    mov     counter, 0

search_init_loop:
    cmp     counter, size                   // Check if done
    b.ge    search_init_done

    // Generate random value (1-99)
    bl      rand                            // Get random number
    mov     w1, 99                          // Modulo 99
    udiv    w2, w0, w1                      // w2 = rand / 99
    msub    w0, w2, w1, w0                  // w0 = rand % 99
    add     w0, w0, 1                       // w0 = (rand % 99) + 1

    // Store in array
    str     w0, [array_ptr, counter, SXTW 2] // array[i] = random

    add     counter, counter, 1             // Increment counter
    b       search_init_loop

search_init_done:
    // Display success message
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_initialized
    add     x0, x0, :lo12:msg_initialized
    mov     w1, size
    bl      printf

    ldr     x21, [sp, 32]                   // Restore registers
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

    undefine(`size')
    undefine(`counter')
    undefine(`array_ptr')

// ============================================================================
// search_sort_array - Sort array using simple bubble sort
// ============================================================================
search_sort_array:
    define(size, w19)
    define(outer, w20)
    define(inner, w21)
    define(array_ptr, x22)
    define(temp, w23)

    stp     x29, x30, [sp, -64]!            // Save registers
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    str     x23, [sp, 48]

    // Check if array is initialized
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    ldr     size, [x0]

    cmp     size, 0
    b.le    search_sort_empty

    // Get array pointer
    adrp    array_ptr, search_array
    add     array_ptr, array_ptr, :lo12:search_array

    // Bubble sort implementation
    mov     outer, 0                        // i = 0

search_sort_outer:
    sub     w0, size, 1                     // n - 1
    cmp     outer, w0
    b.ge    search_sort_done                // Done if i >= n-1

    mov     inner, 0                        // j = 0

search_sort_inner:
    sub     w0, size, outer                 // n - i
    sub     w0, w0, 1                       // n - i - 1
    cmp     inner, w0
    b.ge    search_sort_inner_done          // Done if j >= n-i-1

    // Compare arr[j] and arr[j+1]
    ldr     w0, [array_ptr, inner, SXTW 2]  // arr[j]
    add     w1, inner, 1
    ldr     w1, [array_ptr, w1, SXTW 2]     // arr[j+1]

    cmp     w0, w1
    b.le    search_sort_no_swap             // Skip if arr[j] <= arr[j+1]

    // Swap arr[j] and arr[j+1]
    mov     temp, w0                        // temp = arr[j]
    str     w1, [array_ptr, inner, SXTW 2]  // arr[j] = arr[j+1]
    add     w1, inner, 1
    str     temp, [array_ptr, w1, SXTW 2]   // arr[j+1] = temp

search_sort_no_swap:
    add     inner, inner, 1                 // j++
    b       search_sort_inner

search_sort_inner_done:
    add     outer, outer, 1                 // i++
    b       search_sort_outer

search_sort_done:
    // Display success message
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_sorted
    add     x0, x0, :lo12:msg_sorted
    bl      printf

    b       search_sort_exit

search_sort_empty:
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf

search_sort_exit:
    ldr     x23, [sp, 48]                   // Restore registers
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

    undefine(`size')
    undefine(`outer')
    undefine(`inner')
    undefine(`array_ptr')
    undefine(`temp')

// ============================================================================
// search_get_speed - Prompts user to enter animation speed
// ============================================================================
search_get_speed:
    define(speed, w19)

    stp     x29, x30, [sp, -32]!            // Save registers
    mov     x29, sp
    str     x19, [sp, 16]

    // Position cursor for prompt
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Prompt for speed
    adrp    x0, prompt_speed
    add     x0, x0, :lo12:prompt_speed
    bl      printf

    // Read speed with range validation (100-2500 ms)
    mov     w0, 100                         // min
    mov     w1, 2500                        // max
    bl      read_int_range
    mov     speed, w0                       // Save speed

    // Store in search_delay
    adrp    x0, search_delay
    add     x0, x0, :lo12:search_delay
    str     speed, [x0]

    // Clear the line to avoid overlap with next prompt
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, .Lclear_line
    add     x0, x0, :lo12:.Lclear_line
    bl      printf

    ldr     x19, [sp, 16]                   // Restore registers
    ldp     x29, x30, [sp], 32
    ret

    undefine(`speed')

// ============================================================================
// search_check_if_sorted - Check if array is sorted in ascending order
// Returns: w0 = 1 if sorted, 0 if not sorted
// ============================================================================
search_check_if_sorted:
    define(size, w19)
    define(index, w20)
    define(array_ptr, x21)

    stp     x29, x30, [sp, -48]!            // Save registers
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    // Get array size
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    ldr     size, [x0]

    // If size <= 1, consider it sorted
    cmp     size, 1
    b.le    search_is_sorted

    // Get array pointer
    adrp    array_ptr, search_array
    add     array_ptr, array_ptr, :lo12:search_array

    // Check if each element <= next element
    mov     index, 0

search_check_loop:
    add     w0, index, 1
    cmp     w0, size
    b.ge    search_is_sorted                // Reached end, it's sorted

    // Compare array[i] with array[i+1]
    ldr     w0, [array_ptr, index, SXTW 2]
    add     w1, index, 1
    ldr     w1, [array_ptr, w1, SXTW 2]

    cmp     w0, w1
    b.gt    search_not_sorted               // array[i] > array[i+1], not sorted

    add     index, index, 1
    b       search_check_loop

search_is_sorted:
    mov     w0, 1                           // Return 1 (sorted)
    b       search_check_done

search_not_sorted:
    mov     w0, 0                           // Return 0 (not sorted)

search_check_done:
    ldr     x21, [sp, 32]                   // Restore registers
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

    undefine(`size')
    undefine(`index')
    undefine(`array_ptr')

// ============================================================================
// search_run_linear - Run linear search with visualization
// ============================================================================
search_run_linear:
    define(size, w19)
    define(target, w20)
    define(index, w21)
    define(array_ptr, x22)
    define(delay, w23)

    stp     x29, x30, [sp, -64]!            // Save registers
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    str     x23, [sp, 48]

    // Check if array is initialized
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    ldr     size, [x0]

    cmp     size, 0
    b.le    search_linear_empty

    // Get speed from user
    bl      search_get_speed

    // Get target from user
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, prompt_target
    add     x0, x0, :lo12:prompt_target
    bl      printf

    bl      read_int
    mov     target, w0                      // Save target

    // Store target
    adrp    x0, search_target
    add     x0, x0, :lo12:search_target
    str     target, [x0]

    // Reset visualization state
    adrp    x0, current_idx
    add     x0, x0, :lo12:current_idx
    mov     w1, -1
    str     w1, [x0]

    adrp    x0, found_idx
    add     x0, x0, :lo12:found_idx
    str     w1, [x0]

    adrp    x0, checked_up_to
    add     x0, x0, :lo12:checked_up_to
    str     w1, [x0]

    // Wait for user to start
    mov     w0, 23                          // Position cursor at row 23
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_press_enter
    add     x0, x0, :lo12:msg_press_enter
    bl      printf

    bl      clear_input_buffer
    bl      getchar

    // Get array pointer and delay
    adrp    array_ptr, search_array
    add     array_ptr, array_ptr, :lo12:search_array

    adrp    x0, search_delay
    add     x0, x0, :lo12:search_delay
    ldr     delay, [x0]

    // Linear search loop
    mov     index, 0                        // Start at index 0

search_linear_loop:
    cmp     index, size                     // Check if done
    b.ge    search_linear_not_found

    // Update current index
    adrp    x0, current_idx
    add     x0, x0, :lo12:current_idx
    str     index, [x0]

    // Display array with highlighting
    bl      search_display_array

    // Delay for animation
    mov     w0, delay
    bl      delay_ms

    // Compare current element with target
    ldr     w0, [array_ptr, index, SXTW 2]  // arr[i]
    cmp     w0, target
    b.eq    search_linear_found             // Found!

    // Mark as checked
    adrp    x0, checked_up_to
    add     x0, x0, :lo12:checked_up_to
    str     index, [x0]

    // Clear current index
    adrp    x0, current_idx
    add     x0, x0, :lo12:current_idx
    mov     w1, -1
    str     w1, [x0]

    add     index, index, 1                 // Move to next index
    b       search_linear_loop

search_linear_found:
    // Mark as found
    adrp    x0, found_idx
    add     x0, x0, :lo12:found_idx
    str     index, [x0]

    // Clear current index
    adrp    x0, current_idx
    add     x0, x0, :lo12:current_idx
    mov     w1, -1
    str     w1, [x0]

    // Display final state
    bl      search_display_array

    // Display success message
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_found
    add     x0, x0, :lo12:msg_found
    mov     w1, target
    mov     w2, index
    bl      printf

    b       search_linear_exit

search_linear_not_found:
    // Clear current index
    adrp    x0, current_idx
    add     x0, x0, :lo12:current_idx
    mov     w1, -1
    str     w1, [x0]

    // Display final state
    bl      search_display_array

    // Display not found message
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_not_found
    add     x0, x0, :lo12:msg_not_found
    mov     w1, target
    bl      printf

    b       search_linear_exit

search_linear_empty:
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf

search_linear_exit:
    ldr     x23, [sp, 48]                   // Restore registers
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

    undefine(`size')
    undefine(`target')
    undefine(`index')
    undefine(`array_ptr')
    undefine(`delay')

// ============================================================================
// search_run_binary - Run binary search with visualization
// ============================================================================
search_run_binary:
    define(size, w19)
    define(target, w20)
    define(low, w21)
    define(high, w22)
    define(mid, w23)
    define(array_ptr, x24)
    define(delay, w25)

    stp     x29, x30, [sp, -80]!            // Save registers
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]
    str     x25, [sp, 64]

    // Check if array is initialized
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    ldr     size, [x0]

    cmp     size, 0
    b.le    search_binary_empty

    // Get speed from user
    bl      search_get_speed

    // Get target from user
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, prompt_target
    add     x0, x0, :lo12:prompt_target
    bl      printf

    bl      read_int
    mov     target, w0                      // Save target

    // Store target
    adrp    x0, search_target
    add     x0, x0, :lo12:search_target
    str     target, [x0]

    // Reset visualization state
    adrp    x0, found_idx
    add     x0, x0, :lo12:found_idx
    mov     w1, -1
    str     w1, [x0]

    // Check if array is sorted, offer to sort if not
    bl      search_check_if_sorted
    cmp     w0, 0
    b.ne    search_binary_array_sorted

    // Array is not sorted - ask user if they want to sort
    mov     w0, 23                          // Position cursor at row 23
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_sort_prompt
    add     x0, x0, :lo12:msg_sort_prompt
    bl      printf

    // Read y/n response
    bl      clear_input_buffer
    bl      getchar

    // Check if user said yes
    cmp     w0, 'y'
    b.eq    search_binary_do_sort
    cmp     w0, 'Y'
    b.eq    search_binary_do_sort

    // User said no, continue anyway
    b       search_binary_continue

search_binary_do_sort:
    // Clear line and show sorting message
    mov     w0, 23
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, .Lclear_line
    add     x0, x0, :lo12:.Lclear_line
    bl      printf

    mov     w0, 23
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_sorting
    add     x0, x0, :lo12:msg_sorting
    bl      printf

    // Sort the array
    bl      search_sort_array

    b       search_binary_continue

search_binary_array_sorted:
    // Array is already sorted, just show press enter
    mov     w0, 23                          // Position cursor at row 23
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_press_enter
    add     x0, x0, :lo12:msg_press_enter
    bl      printf

    bl      clear_input_buffer
    bl      getchar
    b       search_binary_start

search_binary_continue:
    // Clear line
    mov     w0, 23
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, .Lclear_line
    add     x0, x0, :lo12:.Lclear_line
    bl      printf

    // Show press enter
    mov     w0, 23
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_press_enter
    add     x0, x0, :lo12:msg_press_enter
    bl      printf

    bl      clear_input_buffer
    bl      getchar

search_binary_start:

    // Get array pointer and delay
    adrp    array_ptr, search_array
    add     array_ptr, array_ptr, :lo12:search_array

    adrp    x0, search_delay
    add     x0, x0, :lo12:search_delay
    ldr     delay, [x0]

    // Initialize binary search
    mov     low, 0                          // low = 0
    sub     high, size, 1                   // high = size - 1

search_binary_loop:
    cmp     low, high                       // Check if low > high
    b.gt    search_binary_not_found

    // Calculate mid = (low + high) / 2
    add     mid, low, high
    lsr     mid, mid, 1                     // Divide by 2

    // Update visualization state
    adrp    x0, low_idx
    add     x0, x0, :lo12:low_idx
    str     low, [x0]

    adrp    x0, mid_idx
    add     x0, x0, :lo12:mid_idx
    str     mid, [x0]

    adrp    x0, high_idx
    add     x0, x0, :lo12:high_idx
    str     high, [x0]

    // Display array with highlighting
    bl      search_display_array

    // Delay for animation
    mov     w0, delay
    bl      delay_ms

    // Compare arr[mid] with target
    ldr     w0, [array_ptr, mid, SXTW 2]    // arr[mid]
    cmp     w0, target
    b.eq    search_binary_found             // Found!
    b.lt    search_binary_go_right          // arr[mid] < target, go right

    // arr[mid] > target, go left
    sub     high, mid, 1                    // high = mid - 1
    b       search_binary_loop

search_binary_go_right:
    add     low, mid, 1                     // low = mid + 1
    b       search_binary_loop

search_binary_found:
    // Mark as found
    adrp    x0, found_idx
    add     x0, x0, :lo12:found_idx
    str     mid, [x0]

    // Clear search pointers
    adrp    x0, low_idx
    add     x0, x0, :lo12:low_idx
    mov     w1, -1
    str     w1, [x0]

    adrp    x0, mid_idx
    add     x0, x0, :lo12:mid_idx
    str     w1, [x0]

    adrp    x0, high_idx
    add     x0, x0, :lo12:high_idx
    str     w1, [x0]

    // Display final state
    bl      search_display_array

    // Display success message
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_found
    add     x0, x0, :lo12:msg_found
    mov     w1, target
    mov     w2, mid
    bl      printf

    b       search_binary_exit

search_binary_not_found:
    // Clear search pointers
    adrp    x0, low_idx
    add     x0, x0, :lo12:low_idx
    mov     w1, -1
    str     w1, [x0]

    adrp    x0, mid_idx
    add     x0, x0, :lo12:mid_idx
    str     w1, [x0]

    adrp    x0, high_idx
    add     x0, x0, :lo12:high_idx
    str     w1, [x0]

    // Display final state
    bl      search_display_array

    // Display not found message
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_not_found
    add     x0, x0, :lo12:msg_not_found
    mov     w1, target
    bl      printf

    b       search_binary_exit

search_binary_empty:
    mov     w0, 22                          // Position cursor at row 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf

search_binary_exit:
    ldr     x25, [sp, 64]                   // Restore registers
    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 80
    ret

    undefine(`size')
    undefine(`target')
    undefine(`low')
    undefine(`high')
    undefine(`mid')
    undefine(`array_ptr')
    undefine(`delay')

// ============================================================================
// search_display_array - Display array with color-coded highlighting (table format)
// ============================================================================
search_display_array:
    define(size, w19)
    define(counter, w20)
    define(array_ptr, x21)
    define(current, w22)
    define(found, w23)
    define(checked, w24)
    define(low, w25)
    define(mid, w26)
    define(high, w27)
    define(column, w28)

    stp     x29, x30, [sp, -96]!            // Save registers
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]
    stp     x25, x26, [sp, 64]
    stp     x27, x28, [sp, 80]

    // Clear screen and go home
    adrp    x0, ansi_clear
    add     x0, x0, :lo12:ansi_clear
    bl      printf

    adrp    x0, ansi_home
    add     x0, x0, :lo12:ansi_home
    bl      printf

    // Get array size
    adrp    x0, search_size
    add     x0, x0, :lo12:search_size
    ldr     size, [x0]

    cmp     size, 0
    b.le    search_display_exit             // Exit if empty

    // Get array pointer
    adrp    array_ptr, search_array
    add     array_ptr, array_ptr, :lo12:search_array

    // Load visualization state
    adrp    x0, current_idx
    add     x0, x0, :lo12:current_idx
    ldr     current, [x0]

    adrp    x0, found_idx
    add     x0, x0, :lo12:found_idx
    ldr     found, [x0]

    adrp    x0, checked_up_to
    add     x0, x0, :lo12:checked_up_to
    ldr     checked, [x0]

    adrp    x0, low_idx
    add     x0, x0, :lo12:low_idx
    ldr     low, [x0]

    adrp    x0, mid_idx
    add     x0, x0, :lo12:mid_idx
    ldr     mid, [x0]

    adrp    x0, high_idx
    add     x0, x0, :lo12:high_idx
    ldr     high, [x0]

    // Draw box
    mov     w0, 3
    mov     w1, 2
    mov     w2, 80
    mov     w3, 10
    mov     w4, 0                            // single-line
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 4
    bl      ansi_move_cursor
    adrp    x0, display_header
    add     x0, x0, :lo12:display_header
    mov     w1, 76
    bl      print_centered

    // Print "Index:" label
    mov     w0, 6
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, display_index_label
    add     x0, x0, :lo12:display_index_label
    bl      printf

    // Print indices
    mov     counter, 0
    mov     column, 20                       // Starting column

search_display_indices_loop:
    cmp     counter, size
    b.ge    search_display_values_start

    mov     w0, 6
    mov     w1, column
    bl      ansi_move_cursor

    adrp    x0, index_fmt
    add     x0, x0, :lo12:index_fmt
    mov     w1, counter
    bl      printf

    add     column, column, 6                // Next column
    add     counter, counter, 1
    b       search_display_indices_loop

search_display_values_start:
    // Print "Value:" label
    mov     w0, 7
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, display_value_label
    add     x0, x0, :lo12:display_value_label
    bl      printf

    // Print values with color coding
    mov     counter, 0
    mov     column, 20                       // Starting column

search_display_values_loop:
    cmp     counter, size
    b.ge    search_display_footer

    mov     w0, 7
    mov     w1, column
    bl      ansi_move_cursor

    // Determine color based on state
    // Priority: found > mid > low/high > current > checked

    cmp     counter, found                  // Is it found index?
    b.eq    search_display_found

    cmp     counter, mid                    // Is it mid index?
    b.eq    search_display_mid

    cmp     counter, low                    // Is it low index?
    b.eq    search_display_low_high

    cmp     counter, high                   // Is it high index?
    b.eq    search_display_low_high

    cmp     counter, current                // Is it current index?
    b.eq    search_display_current

    cmp     counter, checked                // Has it been checked?
    b.le    search_display_checked

    // Default: no color
    b       search_display_normal

search_display_found:
    mov     w0, 42                           // Green background
    bl      ansi_set_color_bg
    b       search_display_value

search_display_mid:
    mov     w0, 43                           // Yellow background
    bl      ansi_set_color_bg
    b       search_display_value

search_display_low_high:
    mov     w0, 46                           // Cyan background
    bl      ansi_set_color_bg
    b       search_display_value

search_display_current:
    mov     w0, 43                           // Yellow background
    bl      ansi_set_color_bg
    b       search_display_value

search_display_checked:
    mov     w0, 100                          // Gray background
    bl      ansi_set_color_bg
    b       search_display_value

search_display_normal:
    // No color change

search_display_value:
    // Print value
    ldr     w1, [array_ptr, counter, SXTW 2]
    adrp    x0, value_fmt
    add     x0, x0, :lo12:value_fmt
    bl      printf

    // Reset color
    bl      ansi_reset_attributes

    add     column, column, 6                // Next column
    add     counter, counter, 1
    b       search_display_values_loop

search_display_footer:
    // Print target value
    mov     w0, 9
    mov     w1, 10
    bl      ansi_move_cursor

    adrp    x0, search_target
    add     x0, x0, :lo12:search_target
    ldr     w1, [x0]

    adrp    x0, display_target
    add     x0, x0, :lo12:display_target
    bl      printf

search_display_exit:
    ldp     x27, x28, [sp, 80]               // Restore registers
    ldp     x25, x26, [sp, 64]
    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 96
    ret
