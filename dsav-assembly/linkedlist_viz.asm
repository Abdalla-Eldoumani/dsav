// ============================================================================
// linkedlist_viz.asm - Linked List Visualization Module
// ============================================================================
// Implements singly linked list with dynamic memory allocation:
// - Dynamic node creation using malloc
// - Insert at front/back
// - Delete by value
// - Search functionality
// - Memory cleanup with free
// - Visual representation with arrows
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Linked List Data
// ----------------------------------------------------------------------------
// Node structure: [data: 8 bytes | next: 8 bytes] = 16 bytes total
node_data_offset = 0
node_next_offset = 8
node_size = 16

list_head:      .quad 0                     // Pointer to first node (NULL = empty)
list_count:     .word 0                     // Number of nodes in list

// ----------------------------------------------------------------------------
// UI Strings
// ----------------------------------------------------------------------------
list_title:         .string "LINKED LIST VISUALIZATION"
list_menu_title:    .string "LINKED LIST OPERATIONS MENU"

menu_opt_1:         .string "[1] Insert at Front"
menu_opt_2:         .string "[2] Insert at Back"
menu_opt_3:         .string "[3] Delete by Value"
menu_opt_4:         .string "[4] Search Value"
menu_opt_5:         .string "[5] Display List"
menu_opt_6:         .string "[6] Clear List"
menu_opt_0:         .string "[0] Back to Main Menu"

menu_prompt:        .string "Enter your choice: "
prompt_value:       .string "Enter value: "
prompt_delete:      .string "Enter value to delete: "
prompt_search:      .string "Enter value to search: "

msg_empty:          .string "\x1b[33mList is empty!\x1b[0m"
msg_inserted_front: .string "\x1b[32mInserted %d at front.\x1b[0m"
msg_inserted_back:  .string "\x1b[32mInserted %d at back.\x1b[0m"
msg_deleted:        .string "\x1b[32mDeleted %d from list.\x1b[0m"
msg_not_found:      .string "\x1b[33mValue %d not found in list.\x1b[0m"
msg_found:          .string "\x1b[32mValue %d found in list!\x1b[0m"
msg_cleared:        .string "\x1b[32mList cleared. All nodes freed.\x1b[0m"
msg_alloc_fail:     .string "\x1b[31mMemory allocation failed!\x1b[0m"

label_head:         .string "HEAD"
label_null:         .string "NULL"
label_nodes:        .string "Nodes: %d"

// Highlight color for search animation
highlight_color:    .string "\x1b[43;30m"       // Yellow background, black text
color_reset:        .string "\x1b[0m"

    .text
    .balign 4

// ============================================================================
// FUNCTION: linkedlist_menu
// Main menu for linked list operations
// Parameters: none
// Returns: none
// ============================================================================
    .global linkedlist_menu
linkedlist_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

linkedlist_menu_loop:
    // Clear screen and display menu
    bl      ansi_clear_screen
    bl      display_linkedlist_menu

    // Get user choice
    mov     w0, 0                            // min
    mov     w1, 6                            // max
    bl      read_int_range

    // Dispatch based on choice
    cmp     w0, 0
    b.eq    linkedlist_menu_exit

    cmp     w0, 1
    b.eq    linkedlist_menu_insert_front

    cmp     w0, 2
    b.eq    linkedlist_menu_insert_back

    cmp     w0, 3
    b.eq    linkedlist_menu_delete

    cmp     w0, 4
    b.eq    linkedlist_menu_search

    cmp     w0, 5
    b.eq    linkedlist_menu_display

    cmp     w0, 6
    b.eq    linkedlist_menu_clear

    b       linkedlist_menu_loop

linkedlist_menu_insert_front:
    bl      list_insert_front_interactive
    bl      wait_for_enter
    b       linkedlist_menu_loop

linkedlist_menu_insert_back:
    bl      list_insert_back_interactive
    bl      wait_for_enter
    b       linkedlist_menu_loop

linkedlist_menu_delete:
    bl      list_delete_interactive
    bl      wait_for_enter
    b       linkedlist_menu_loop

linkedlist_menu_search:
    bl      list_search_interactive
    bl      wait_for_enter
    b       linkedlist_menu_loop

linkedlist_menu_display:
    bl      list_display
    bl      wait_for_enter
    b       linkedlist_menu_loop

linkedlist_menu_clear:
    bl      list_free_all

    // Position cursor for status message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_cleared
    add     x0, x0, :lo12:msg_cleared
    bl      printf
    bl      print_newline
    bl      wait_for_enter
    b       linkedlist_menu_loop

linkedlist_menu_exit:
    // Clean up before exit
    bl      list_free_all
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: display_linkedlist_menu
// Displays the linked list operations menu
// Parameters: none
// Returns: none
// ============================================================================
    .global display_linkedlist_menu
display_linkedlist_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Draw menu box
    mov     w0, 3                            // row
    mov     w1, 15                           // column
    mov     w2, 50                           // width
    mov     w3, 15                           // height
    mov     w4, 1                            // double-line style
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 17
    bl      ansi_move_cursor
    adrp    x0, list_menu_title
    add     x0, x0, :lo12:list_menu_title
    mov     w1, 46
    bl      print_centered

    // Print separator
    mov     w0, 5
    mov     w1, 15
    mov     w2, 50
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Print menu options
    mov     w0, 7
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_1
    add     x0, x0, :lo12:menu_opt_1
    bl      printf

    mov     w0, 8
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_2
    add     x0, x0, :lo12:menu_opt_2
    bl      printf

    mov     w0, 9
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_3
    add     x0, x0, :lo12:menu_opt_3
    bl      printf

    mov     w0, 10
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_4
    add     x0, x0, :lo12:menu_opt_4
    bl      printf

    mov     w0, 11
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_5
    add     x0, x0, :lo12:menu_opt_5
    bl      printf

    mov     w0, 12
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_6
    add     x0, x0, :lo12:menu_opt_6
    bl      printf

    mov     w0, 13
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_0
    add     x0, x0, :lo12:menu_opt_0
    bl      printf

    // Print separator
    mov     w0, 15
    mov     w1, 15
    mov     w2, 50
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Position cursor for input
    mov     w0, 16
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_prompt
    add     x0, x0, :lo12:menu_prompt
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: list_create_node
// Creates a new node with malloc
// Parameters:
//   x0 = value (64-bit, but we use lower 32 bits)
// Returns:
//   x0 = pointer to new node (or NULL if allocation failed)
// ============================================================================
    .global list_create_node
list_create_node:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    mov     x19, x0                          // Save value

    // Allocate memory for node
    mov     x0, node_size
    bl      malloc

    // Check if allocation succeeded
    cmp     x0, 0
    b.eq    list_create_node_fail

    // Initialize node
    str     x19, [x0, node_data_offset]     // Store data
    mov     x1, 0
    str     x1, [x0, node_next_offset]      // Set next to NULL

    b       list_create_node_done

list_create_node_fail:
    mov     x0, 0                            // Return NULL

list_create_node_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_insert_front
// Inserts a node at the front of the list
// Parameters:
//   x0 = value
// Returns:
//   w0 = 1 on success, 0 on failure
// ============================================================================
    .global list_insert_front
list_insert_front:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    // Create new node
    bl      list_create_node
    cmp     x0, 0
    b.eq    list_insert_front_fail

    mov     x19, x0                          // Save new node pointer

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x2, [x1]

    // new_node->next = head
    str     x2, [x19, node_next_offset]

    // head = new_node
    str     x19, [x1]

    // Increment count
    adrp    x1, list_count
    add     x1, x1, :lo12:list_count
    ldr     w2, [x1]
    add     w2, w2, 1
    str     w2, [x1]

    mov     w0, 1                            // Success
    b       list_insert_front_done

list_insert_front_fail:
    mov     w0, 0                            // Failure

list_insert_front_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_insert_back
// Inserts a node at the back of the list
// Parameters:
//   x0 = value
// Returns:
//   w0 = 1 on success, 0 on failure
// ============================================================================
    .global list_insert_back
list_insert_back:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    mov     x19, x0                          // Save value

    // Create new node
    bl      list_create_node
    cmp     x0, 0
    b.eq    list_insert_back_fail

    mov     x20, x0                          // Save new node pointer

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x21, [x1]

    // Check if list is empty
    cmp     x21, 0
    b.eq    list_insert_back_empty

    // Traverse to last node
list_insert_back_traverse:
    ldr     x2, [x21, node_next_offset]
    cmp     x2, 0
    b.eq    list_insert_back_found_last
    mov     x21, x2
    b       list_insert_back_traverse

list_insert_back_found_last:
    // last->next = new_node
    str     x20, [x21, node_next_offset]
    b       list_insert_back_success

list_insert_back_empty:
    // head = new_node
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    str     x20, [x1]

list_insert_back_success:
    // Increment count
    adrp    x1, list_count
    add     x1, x1, :lo12:list_count
    ldr     w2, [x1]
    add     w2, w2, 1
    str     w2, [x1]

    mov     w0, 1                            // Success
    b       list_insert_back_done

list_insert_back_fail:
    mov     w0, 0                            // Failure

list_insert_back_done:
    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: list_delete
// Deletes the first node with specified value
// Parameters:
//   x0 = value to delete
// Returns:
//   w0 = 1 if deleted, 0 if not found
// ============================================================================
    .global list_delete
list_delete:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    mov     x19, x0                          // Save value to delete

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x20, [x1]

    // Check if list is empty
    cmp     x20, 0
    b.eq    list_delete_not_found

    // Check if head needs to be deleted
    ldr     x2, [x20, node_data_offset]
    cmp     x2, x19
    b.eq    list_delete_head

    // Traverse list to find node
    mov     x21, x20                         // prev = head
list_delete_traverse:
    ldr     x20, [x20, node_next_offset]    // current = current->next
    cmp     x20, 0
    b.eq    list_delete_not_found

    ldr     x2, [x20, node_data_offset]
    cmp     x2, x19
    b.eq    list_delete_found
    mov     x21, x20
    b       list_delete_traverse

list_delete_head:
    // Delete head node
    ldr     x2, [x20, node_next_offset]
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    str     x2, [x1]

    mov     x0, x20
    bl      free

    b       list_delete_success

list_delete_found:
    // Delete middle/last node
    ldr     x2, [x20, node_next_offset]
    str     x2, [x21, node_next_offset]

    mov     x0, x20
    bl      free

list_delete_success:
    // Decrement count
    adrp    x1, list_count
    add     x1, x1, :lo12:list_count
    ldr     w2, [x1]
    sub     w2, w2, 1
    str     w2, [x1]

    mov     w0, 1                            // Success
    b       list_delete_done

list_delete_not_found:
    mov     w0, 0                            // Not found

list_delete_done:
    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: list_search
// Searches for a value in the list
// Parameters:
//   x0 = value to search
// Returns:
//   x0 = pointer to node if found, NULL if not found
// ============================================================================
    .global list_search
list_search:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    mov     x19, x0                          // Save search value

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x0, [x1]

list_search_loop:
    cmp     x0, 0
    b.eq    list_search_not_found

    ldr     x2, [x0, node_data_offset]
    cmp     x2, x19
    b.eq    list_search_found

    ldr     x0, [x0, node_next_offset]
    b       list_search_loop

list_search_found:
    // x0 already contains node pointer
    b       list_search_done

list_search_not_found:
    mov     x0, 0                            // Return NULL

list_search_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_free_all
// Frees all nodes in the list
// Parameters: none
// Returns: none
// ============================================================================
    .global list_free_all
list_free_all:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x19, [x1]

list_free_all_loop:
    cmp     x19, 0
    b.eq    list_free_all_done

    // Save next pointer
    ldr     x20, [x19, node_next_offset]

    // Free current node
    mov     x0, x19
    bl      free

    // Move to next
    mov     x19, x20
    b       list_free_all_loop

list_free_all_done:
    // Reset head to NULL
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    mov     x2, 0
    str     x2, [x1]

    // Reset count to 0
    adrp    x1, list_count
    add     x1, x1, :lo12:list_count
    str     w2, [x1]

    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_insert_front_interactive
// Interactive insert at front
// Parameters: none
// Returns: none
// ============================================================================
    .global list_insert_front_interactive
list_insert_front_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Prompt for value
    adrp    x0, prompt_value
    add     x0, x0, :lo12:prompt_value
    bl      printf

    bl      read_int
    sxtw    x19, w0                          // Sign-extend to 64-bit

    // Insert at front
    mov     x0, x19
    bl      list_insert_front

    cmp     w0, 0
    b.eq    list_insert_front_int_fail

    // Display list
    bl      list_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_inserted_front
    add     x0, x0, :lo12:msg_inserted_front
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       list_insert_front_int_done

list_insert_front_int_fail:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_alloc_fail
    add     x0, x0, :lo12:msg_alloc_fail
    bl      printf
    bl      print_newline

list_insert_front_int_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_insert_back_interactive
// Interactive insert at back
// Parameters: none
// Returns: none
// ============================================================================
    .global list_insert_back_interactive
list_insert_back_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Prompt for value
    adrp    x0, prompt_value
    add     x0, x0, :lo12:prompt_value
    bl      printf

    bl      read_int
    sxtw    x19, w0                          // Sign-extend to 64-bit

    // Insert at back
    mov     x0, x19
    bl      list_insert_back

    cmp     w0, 0
    b.eq    list_insert_back_int_fail

    // Display list
    bl      list_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_inserted_back
    add     x0, x0, :lo12:msg_inserted_back
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       list_insert_back_int_done

list_insert_back_int_fail:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_alloc_fail
    add     x0, x0, :lo12:msg_alloc_fail
    bl      printf
    bl      print_newline

list_insert_back_int_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_delete_interactive
// Interactive delete by value
// Parameters: none
// Returns: none
// ============================================================================
    .global list_delete_interactive
list_delete_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if list is empty
    adrp    x0, list_count
    add     x0, x0, :lo12:list_count
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    list_delete_int_empty

    // Prompt for value
    adrp    x0, prompt_delete
    add     x0, x0, :lo12:prompt_delete
    bl      printf

    bl      read_int
    sxtw    x19, w0

    // Delete value
    mov     x0, x19
    bl      list_delete

    cmp     w0, 0
    b.eq    list_delete_int_not_found

    // Display list
    bl      list_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_deleted
    add     x0, x0, :lo12:msg_deleted
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       list_delete_int_done

list_delete_int_empty:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline
    b       list_delete_int_done

list_delete_int_not_found:
    // Display list
    bl      list_display

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_not_found
    add     x0, x0, :lo12:msg_not_found
    mov     w1, w19
    bl      printf
    bl      print_newline

list_delete_int_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: list_search_interactive
// Interactive search with animated traversal
// Parameters: none
// Returns: none
// ============================================================================
    .global list_search_interactive
list_search_interactive:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    bl      ansi_clear_screen

    // Check if list is empty
    adrp    x0, list_count
    add     x0, x0, :lo12:list_count
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    list_search_int_empty

    // Prompt for value
    adrp    x0, prompt_search
    add     x0, x0, :lo12:prompt_search
    bl      printf

    bl      read_int
    sxtw    x19, w0                          // x19 = search target

    // Show initial list
    bl      list_display

    // Wait for user to start
    mov     w0, 23
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, .Lmsg_press_enter
    add     x0, x0, :lo12:.Lmsg_press_enter
    bl      printf

    bl      clear_input_buffer
    bl      getchar

    // Animated search - Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x20, [x1]                        // x20 = current node
    mov     w21, 0                           // x21 = current index

list_search_anim_loop:
    cmp     x20, 0
    b.eq    list_search_int_not_found

    // Display list with current node highlighted
    bl      ansi_clear_screen
    bl      list_display_with_highlight      // Pass x20 as highlighted node

    // Position cursor for search message
    mov     w0, 15
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, .Lmsg_checking
    add     x0, x0, :lo12:.Lmsg_checking
    ldr     w1, [x20, node_data_offset]
    mov     w2, w21
    bl      printf

    // Delay for animation
    mov     w0, 500                          // 500ms delay
    bl      delay_ms

    // Check if current node matches target
    ldr     x2, [x20, node_data_offset]
    cmp     x2, x19
    b.eq    list_search_int_found

    // Move to next node
    ldr     x20, [x20, node_next_offset]
    add     w21, w21, 1
    b       list_search_anim_loop

list_search_int_found:
    // Display final state with found node highlighted
    bl      ansi_clear_screen
    bl      list_display_with_highlight

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_found
    add     x0, x0, :lo12:msg_found
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       list_search_int_done

list_search_int_empty:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline
    b       list_search_int_done

list_search_int_not_found:
    // Display final state
    bl      ansi_clear_screen
    bl      list_display

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_not_found
    add     x0, x0, :lo12:msg_not_found
    mov     w1, w19
    bl      printf
    bl      print_newline

list_search_int_done:
    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: list_display
// Displays the linked list with visual arrows
// Parameters: none
// Returns: none
// ============================================================================
    .global list_display
list_display:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    bl      ansi_clear_screen

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x19, [x1]

    // Check if empty
    cmp     x19, 0
    b.eq    list_display_empty

    // Draw box
    mov     w0, 3
    mov     w1, 5
    mov     w2, 70
    mov     w3, 12
    mov     w4, 0                            // single-line
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 7
    bl      ansi_move_cursor
    adrp    x0, list_title
    add     x0, x0, :lo12:list_title
    mov     w1, 66
    bl      print_centered

    // Draw separator
    mov     w0, 5
    mov     w1, 5
    mov     w2, 70
    mov     w3, 0
    bl      draw_horizontal_border_top

    // Print HEAD label
    mov     w0, 8
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, label_head
    add     x0, x0, :lo12:label_head
    bl      printf

    // Print arrow
    mov     w0, 9
    mov     w1, 11
    bl      ansi_move_cursor
    adrp    x0, .Larrow_down
    add     x0, x0, :lo12:.Larrow_down
    bl      printf

    // Display nodes
    mov     w20, 10                          // Starting row
    mov     w21, 10                          // Column position

list_display_loop:
    cmp     x19, 0
    b.eq    list_display_footer

    // Position cursor
    mov     w0, w20
    mov     w1, w21
    bl      ansi_move_cursor

    // Print node box and value
    ldr     x1, [x19, node_data_offset]
    adrp    x0, .Lnode_fmt
    add     x0, x0, :lo12:.Lnode_fmt
    bl      printf

    // Check if there's a next node
    ldr     x19, [x19, node_next_offset]
    cmp     x19, 0
    b.eq    list_display_last_node

    // Print arrow
    adrp    x0, .Larrow_right
    add     x0, x0, :lo12:.Larrow_right
    bl      printf

    add     w21, w21, 11                     // Move to next position
    b       list_display_loop

list_display_last_node:
    // Print NULL
    adrp    x0, .Larrow_right
    add     x0, x0, :lo12:.Larrow_right
    bl      printf

    adrp    x0, label_null
    add     x0, x0, :lo12:label_null
    bl      printf

list_display_footer:
    // Print count
    mov     w0, 13
    mov     w1, 10
    bl      ansi_move_cursor

    adrp    x1, list_count
    add     x1, x1, :lo12:list_count
    ldr     w1, [x1]

    adrp    x0, label_nodes
    add     x0, x0, :lo12:label_nodes
    bl      printf

    bl      print_newline
    b       list_display_done

list_display_empty:
    // Position cursor for error message
    mov     w0, 10
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

list_display_done:
    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

    .section .rodata
.Larrow_down:   .string "↓"
.Larrow_right:  .string "──>"
.Lnode_fmt:     .string "[%3ld]"
.Lmsg_press_enter: .string "\x1b[33mPress Enter to start searching...\x1b[0m"
.Lmsg_checking: .string "Checking node value %ld at index %d..."
    .text

// ============================================================================
// FUNCTION: list_display_with_highlight
// Displays the linked list with a specific node highlighted
// Parameters:
//   x20 = pointer to node to highlight (passed in x20)
// Returns: none
// ============================================================================
    .global list_display_with_highlight
list_display_with_highlight:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    // Save highlighted node pointer
    mov     x21, x20                         // x21 = highlight_node

    // Load head
    adrp    x1, list_head
    add     x1, x1, :lo12:list_head
    ldr     x19, [x1]

    // Check if empty
    cmp     x19, 0
    b.eq    list_display_hl_empty

    // Draw box
    mov     w0, 3
    mov     w1, 5
    mov     w2, 70
    mov     w3, 12
    mov     w4, 0                            // single-line
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 7
    bl      ansi_move_cursor
    adrp    x0, list_title
    add     x0, x0, :lo12:list_title
    mov     w1, 66
    bl      print_centered

    // Draw separator
    mov     w0, 5
    mov     w1, 5
    mov     w2, 70
    mov     w3, 0
    bl      draw_horizontal_border_top

    // Print HEAD label
    mov     w0, 8
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, label_head
    add     x0, x0, :lo12:label_head
    bl      printf

    // Print arrow
    mov     w0, 9
    mov     w1, 11
    bl      ansi_move_cursor
    adrp    x0, .Larrow_down
    add     x0, x0, :lo12:.Larrow_down
    bl      printf

    // Display nodes
    mov     w20, 10                          // Starting row
    mov     w22, 10                          // Column position

list_display_hl_loop:
    cmp     x19, 0
    b.eq    list_display_hl_footer

    // Position cursor
    mov     w0, w20
    mov     w1, w22
    bl      ansi_move_cursor

    // Check if this is the highlighted node
    cmp     x19, x21
    b.ne    list_display_hl_normal

    // Highlight this node
    adrp    x0, highlight_color
    add     x0, x0, :lo12:highlight_color
    bl      printf

list_display_hl_normal:
    // Print node box and value
    ldr     x1, [x19, node_data_offset]
    adrp    x0, .Lnode_fmt
    add     x0, x0, :lo12:.Lnode_fmt
    bl      printf

    // Reset color if it was highlighted
    cmp     x19, x21
    b.ne    list_display_hl_no_reset

    adrp    x0, color_reset
    add     x0, x0, :lo12:color_reset
    bl      printf

list_display_hl_no_reset:
    // Check if there's a next node
    ldr     x19, [x19, node_next_offset]
    cmp     x19, 0
    b.eq    list_display_hl_last_node

    // Print arrow
    adrp    x0, .Larrow_right
    add     x0, x0, :lo12:.Larrow_right
    bl      printf

    add     w22, w22, 11                     // Move to next position
    b       list_display_hl_loop

list_display_hl_last_node:
    // Print NULL
    adrp    x0, .Larrow_right
    add     x0, x0, :lo12:.Larrow_right
    bl      printf

    adrp    x0, label_null
    add     x0, x0, :lo12:label_null
    bl      printf

list_display_hl_footer:
    // Print count
    mov     w0, 13
    mov     w1, 10
    bl      ansi_move_cursor

    adrp    x1, list_count
    add     x1, x1, :lo12:list_count
    ldr     w1, [x1]

    adrp    x0, label_nodes
    add     x0, x0, :lo12:label_nodes
    bl      printf

    bl      print_newline
    b       list_display_hl_done

list_display_hl_empty:
    // Position cursor for error message
    mov     w0, 10
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

list_display_hl_done:
    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret
