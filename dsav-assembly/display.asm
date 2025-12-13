// ============================================================================
// display.asm - Display and Box Drawing Functions
// ============================================================================
// Provides generic display functions for creating visual elements:
// - Box drawing with Unicode characters
// - Horizontal and vertical lines
// - Centered text printing
// - Title headers
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Box Drawing Characters (Unicode UTF-8 byte sequences)
// ----------------------------------------------------------------------------
// Single-line box characters (UTF-8 encoded)
char_top_left:      .string "\xe2\x94\x8c"        // ┌ (U+250C)
char_top_right:     .string "\xe2\x94\x90"        // ┐ (U+2510)
char_bottom_left:   .string "\xe2\x94\x94"        // └ (U+2514)
char_bottom_right:  .string "\xe2\x94\x98"        // ┘ (U+2518)
char_horizontal:    .string "\xe2\x94\x80"        // ─ (U+2500)
char_vertical:      .string "\xe2\x94\x82"        // │ (U+2502)
char_t_down:        .string "\xe2\x94\xac"        // ┬ (U+252C)
char_t_up:          .string "\xe2\x94\xb4"        // ┴ (U+2534)
char_t_right:       .string "\xe2\x94\x9c"        // ├ (U+251C)
char_t_left:        .string "\xe2\x94\xa4"        // ┤ (U+2524)
char_cross:         .string "\xe2\x94\xbc"        // ┼ (U+253C)

// Double-line box characters (UTF-8 encoded)
char_d_top_left:    .string "\xe2\x95\x94"        // ╔ (U+2554)
char_d_top_right:   .string "\xe2\x95\x97"        // ╗ (U+2557)
char_d_bottom_left: .string "\xe2\x95\x9a"        // ╚ (U+255A)
char_d_bottom_right:.string "\xe2\x95\x9d"        // ╝ (U+255D)
char_d_horizontal:  .string "\xe2\x95\x90"        // ═ (U+2550)
char_d_vertical:    .string "\xe2\x95\x91"        // ║ (U+2551)
char_d_t_down:      .string "\xe2\x95\xa6"        // ╦ (U+2566)
char_d_t_up:        .string "\xe2\x95\xa9"        // ╩ (U+2569)
char_d_t_right:     .string "\xe2\x95\xa0"        // ╠ (U+2560)
char_d_t_left:      .string "\xe2\x95\xa3"        // ╣ (U+2563)

// Space and other characters
char_space:         .string " "

    .text
    .balign 4

// ============================================================================
// FUNCTION: draw_box
// Draws a box at specified position with given dimensions
// Parameters:
//   w0 = row (top-left corner)
//   w1 = column (top-left corner)
//   w2 = width (in characters)
//   w3 = height (in lines)
//   w4 = style (0 = single line, 1 = double line)
// Returns: none
// ============================================================================
    .global draw_box
draw_box:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]              // Save callee-saved registers
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]

    mov     w19, w0                          // row
    mov     w20, w1                          // column
    mov     w21, w2                          // width
    mov     w22, w3                          // height
    mov     w23, w4                          // style

    // Draw top border
    mov     w0, w19                          // row
    mov     w1, w20                          // column
    mov     w2, w21                          // width
    mov     w3, w23                          // style
    bl      draw_horizontal_border_top

    // Draw middle lines (sides only)
    mov     w24, 1                           // line counter
draw_box_middle_loop:
    cmp     w24, w22
    b.ge    draw_box_middle_done
    sub     w0, w22, 1                      // height - 1
    cmp     w24, w0
    b.ge    draw_box_middle_done

    // Move to position for this line
    add     w0, w19, w24                    // row + line_num
    mov     w1, w20                          // column
    bl      ansi_move_cursor

    // Draw left border
    cmp     w23, 0
    b.eq    draw_box_single_left
    adrp    x0, char_d_vertical
    add     x0, x0, :lo12:char_d_vertical
    b       draw_box_print_left
draw_box_single_left:
    adrp    x0, char_vertical
    add     x0, x0, :lo12:char_vertical
draw_box_print_left:
    bl      printf

    // Print spaces (interior)
    mov     w25, 2                           // space counter (skip 2 for borders)
draw_box_spaces_loop:
    cmp     w25, w21
    b.ge    draw_box_spaces_done
    adrp    x0, char_space
    add     x0, x0, :lo12:char_space
    bl      printf
    add     w25, w25, 1
    b       draw_box_spaces_loop
draw_box_spaces_done:

    // Draw right border
    cmp     w23, 0
    b.eq    draw_box_single_right
    adrp    x0, char_d_vertical
    add     x0, x0, :lo12:char_d_vertical
    b       draw_box_print_right
draw_box_single_right:
    adrp    x0, char_vertical
    add     x0, x0, :lo12:char_vertical
draw_box_print_right:
    bl      printf

    add     w24, w24, 1
    b       draw_box_middle_loop

draw_box_middle_done:
    // Draw bottom border
    add     w0, w19, w22                    // row + height
    sub     w0, w0, 1                        // -1 for 0-based
    mov     w1, w20                          // column
    mov     w2, w21                          // width
    mov     w3, w23                          // style
    bl      draw_horizontal_border_bottom

    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

// ============================================================================
// FUNCTION: draw_horizontal_border_top
// Draws top border of a box
// Parameters:
//   w0 = row
//   w1 = column
//   w2 = width
//   w3 = style (0 = single, 1 = double)
// Returns: none
// ============================================================================
    .global draw_horizontal_border_top
draw_horizontal_border_top:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    mov     w19, w0                          // row
    mov     w20, w1                          // column
    mov     w21, w2                          // width
    mov     w22, w3                          // style

    // Move cursor to position
    mov     w0, w19
    mov     w1, w20
    bl      ansi_move_cursor

    // Print top-left corner
    cmp     w22, 0
    b.eq    draw_htop_single_tl
    adrp    x0, char_d_top_left
    add     x0, x0, :lo12:char_d_top_left
    b       draw_htop_print_tl
draw_htop_single_tl:
    adrp    x0, char_top_left
    add     x0, x0, :lo12:char_top_left
draw_htop_print_tl:
    bl      printf

    // Print horizontal line
    mov     w23, 2                           // counter (skip 2 for corners)
draw_htop_loop:
    cmp     w23, w21
    b.ge    draw_htop_done
    cmp     w22, 0
    b.eq    draw_htop_single_h
    adrp    x0, char_d_horizontal
    add     x0, x0, :lo12:char_d_horizontal
    b       draw_htop_print_h
draw_htop_single_h:
    adrp    x0, char_horizontal
    add     x0, x0, :lo12:char_horizontal
draw_htop_print_h:
    bl      printf
    add     w23, w23, 1
    b       draw_htop_loop

draw_htop_done:
    // Print top-right corner
    cmp     w22, 0
    b.eq    draw_htop_single_tr
    adrp    x0, char_d_top_right
    add     x0, x0, :lo12:char_d_top_right
    b       draw_htop_print_tr
draw_htop_single_tr:
    adrp    x0, char_top_right
    add     x0, x0, :lo12:char_top_right
draw_htop_print_tr:
    bl      printf

    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: draw_horizontal_border_bottom
// Draws bottom border of a box
// Parameters:
//   w0 = row
//   w1 = column
//   w2 = width
//   w3 = style (0 = single, 1 = double)
// Returns: none
// ============================================================================
    .global draw_horizontal_border_bottom
draw_horizontal_border_bottom:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    mov     w19, w0                          // row
    mov     w20, w1                          // column
    mov     w21, w2                          // width
    mov     w22, w3                          // style

    // Move cursor to position
    mov     w0, w19
    mov     w1, w20
    bl      ansi_move_cursor

    // Print bottom-left corner
    cmp     w22, 0
    b.eq    draw_hbot_single_bl
    adrp    x0, char_d_bottom_left
    add     x0, x0, :lo12:char_d_bottom_left
    b       draw_hbot_print_bl
draw_hbot_single_bl:
    adrp    x0, char_bottom_left
    add     x0, x0, :lo12:char_bottom_left
draw_hbot_print_bl:
    bl      printf

    // Print horizontal line
    mov     w23, 2                           // counter
draw_hbot_loop:
    cmp     w23, w21
    b.ge    draw_hbot_done
    cmp     w22, 0
    b.eq    draw_hbot_single_h
    adrp    x0, char_d_horizontal
    add     x0, x0, :lo12:char_d_horizontal
    b       draw_hbot_print_h
draw_hbot_single_h:
    adrp    x0, char_horizontal
    add     x0, x0, :lo12:char_horizontal
draw_hbot_print_h:
    bl      printf
    add     w23, w23, 1
    b       draw_hbot_loop

draw_hbot_done:
    // Print bottom-right corner
    cmp     w22, 0
    b.eq    draw_hbot_single_br
    adrp    x0, char_d_bottom_right
    add     x0, x0, :lo12:char_d_bottom_right
    b       draw_hbot_print_br
draw_hbot_single_br:
    adrp    x0, char_bottom_right
    add     x0, x0, :lo12:char_bottom_right
draw_hbot_print_br:
    bl      printf

    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: print_centered
// Prints text centered within a specified width
// Parameters:
//   x0 = pointer to string
//   w1 = total width for centering
// Returns: none
// ============================================================================
    .global print_centered
print_centered:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    mov     x19, x0                          // Save string pointer
    mov     w20, w1                          // Save width

    // Calculate string length
    bl      strlen                          // Returns length in x0
    mov     w21, w0                          // Save length

    // Calculate padding: (width - length) / 2
    sub     w0, w20, w21                    // width - length
    lsr     w0, w0, 1                        // divide by 2
    mov     w22, w0                          // Save padding

    // Print leading spaces
    mov     w23, 0
print_centered_pad_loop:
    cmp     w23, w22
    b.ge    print_centered_pad_done
    adrp    x0, char_space
    add     x0, x0, :lo12:char_space
    bl      printf
    add     w23, w23, 1
    b       print_centered_pad_loop

print_centered_pad_done:
    // Print the string
    mov     x0, x19
    bl      printf

    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: print_title
// Prints a centered title with decorative borders
// Parameters:
//   x0 = pointer to title string
//   w1 = row position
// Returns: none
// ============================================================================
    .global print_title
print_title:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    mov     x19, x0                          // Save title string
    mov     w20, w1                          // Save row

    // Move cursor and print title centered
    mov     w0, w20
    mov     w1, 1
    bl      ansi_move_cursor

    mov     x0, x19
    mov     w1, 60                           // Center within 60 chars
    bl      print_centered

    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret
