// ============================================================================
// ansi.asm - ANSI Escape Code Utilities
// ============================================================================
// Provides functions for terminal control using ANSI escape sequences:
// - Screen clearing and cursor control
// - Text colors and attributes
// - Cursor positioning
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// ANSI Escape Sequence Strings (with _seq suffix to avoid name conflicts)
// ----------------------------------------------------------------------------

// Screen Control
seq_clear:              .string "\x1b[2J"           // Clear entire screen
seq_clear_line:         .string "\x1b[2K"           // Clear current line
seq_home:               .string "\x1b[H"            // Move cursor to home (1,1)
seq_reset:              .string "\x1b[0m"           // Reset all attributes

// Cursor Control
seq_hide_cursor:        .string "\x1b[?25l"         // Hide cursor
seq_show_cursor:        .string "\x1b[?25h"         // Show cursor
seq_save_cursor:        .string "\x1b[s"            // Save cursor position
seq_restore_cursor:     .string "\x1b[u"            // Restore cursor position

// Text Attributes
seq_bold:               .string "\x1b[1m"           // Bold text
seq_dim:                .string "\x1b[2m"           // Dim text
seq_underline:          .string "\x1b[4m"           // Underline text
seq_blink:              .string "\x1b[5m"           // Blinking text
seq_reverse:            .string "\x1b[7m"           // Reverse video

// Foreground Colors (30-37)
seq_fg_black:           .string "\x1b[30m"
seq_fg_red:             .string "\x1b[31m"
seq_fg_green:           .string "\x1b[32m"
seq_fg_yellow:          .string "\x1b[33m"
seq_fg_blue:            .string "\x1b[34m"
seq_fg_magenta:         .string "\x1b[35m"
seq_fg_cyan:            .string "\x1b[36m"
seq_fg_white:           .string "\x1b[37m"

// Bright Foreground Colors (90-97)
seq_fg_bright_black:    .string "\x1b[90m"
seq_fg_bright_red:      .string "\x1b[91m"
seq_fg_bright_green:    .string "\x1b[92m"
seq_fg_bright_yellow:   .string "\x1b[93m"
seq_fg_bright_blue:     .string "\x1b[94m"
seq_fg_bright_magenta:  .string "\x1b[95m"
seq_fg_bright_cyan:     .string "\x1b[96m"
seq_fg_bright_white:    .string "\x1b[97m"

// Background Colors (40-47)
seq_bg_black:           .string "\x1b[40m"
seq_bg_red:             .string "\x1b[41m"
seq_bg_green:           .string "\x1b[42m"
seq_bg_yellow:          .string "\x1b[43m"
seq_bg_blue:            .string "\x1b[44m"
seq_bg_magenta:         .string "\x1b[45m"
seq_bg_cyan:            .string "\x1b[46m"
seq_bg_white:           .string "\x1b[47m"

// Format strings
fmt_position:           .string "\x1b[%d;%dH"       // Cursor position format
fmt_color:              .string "\x1b[%dm"          // Color format

    .text
    .balign 4

// ============================================================================
// FUNCTION: ansi_clear_screen
// Clears the entire screen and moves cursor to home position
// Parameters: none
// Returns: none
// ============================================================================
    .global ansi_clear_screen
ansi_clear_screen:
    stp     x29, x30, [sp, -16]!            // Save fp and lr
    mov     x29, sp                          // Set frame pointer

    // Print clear screen sequence
    adrp    x0, seq_clear
    add     x0, x0, :lo12:seq_clear
    bl      printf

    // Move cursor to home position
    adrp    x0, seq_home
    add     x0, x0, :lo12:seq_home
    bl      printf

    ldp     x29, x30, [sp], 16              // Restore fp and lr
    ret

// ============================================================================
// FUNCTION: ansi_clear_line
// Clears the current line
// Parameters: none
// Returns: none
// ============================================================================
    .global ansi_clear_line
ansi_clear_line:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, seq_clear_line
    add     x0, x0, :lo12:seq_clear_line
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_reset_attributes
// Resets all text attributes to default
// Parameters: none
// Returns: none
// ============================================================================
    .global ansi_reset_attributes
ansi_reset_attributes:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, seq_reset
    add     x0, x0, :lo12:seq_reset
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_move_cursor
// Moves cursor to specified row and column (1-indexed)
// Parameters:
//   w0 = row (1-based)
//   w1 = column (1-based)
// Returns: none
// ============================================================================
    .global ansi_move_cursor
ansi_move_cursor:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    mov     w2, w1                          // col -> w2 (3rd arg for printf)
    mov     w1, w0                          // row -> w1 (2nd arg for printf)
    adrp    x0, fmt_position
    add     x0, x0, :lo12:fmt_position
    bl      printf                          // printf("\x1b[%d;%dH", row, col)

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_hide_cursor
// Hides the terminal cursor
// Parameters: none
// Returns: none
// ============================================================================
    .global ansi_hide_cursor
ansi_hide_cursor:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, seq_hide_cursor
    add     x0, x0, :lo12:seq_hide_cursor
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_show_cursor
// Shows the terminal cursor
// Parameters: none
// Returns: none
// ============================================================================
    .global ansi_show_cursor
ansi_show_cursor:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, seq_show_cursor
    add     x0, x0, :lo12:seq_show_cursor
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_set_color_fg
// Sets foreground color (30-37 for normal, 90-97 for bright)
// Parameters:
//   w0 = color code (e.g., 31 for red, 32 for green)
// Returns: none
// ============================================================================
    .global ansi_set_color_fg
ansi_set_color_fg:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    mov     w1, w0                          // color code -> w1
    adrp    x0, fmt_color
    add     x0, x0, :lo12:fmt_color
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_set_color_bg
// Sets background color (40-47 for normal, 100-107 for bright)
// Parameters:
//   w0 = color code (e.g., 41 for red, 42 for green)
// Returns: none
// ============================================================================
    .global ansi_set_color_bg
ansi_set_color_bg:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    mov     w1, w0                          // color code -> w1
    adrp    x0, fmt_color
    add     x0, x0, :lo12:fmt_color
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: ansi_print_color
// Prints a string in specified color then resets
// Parameters:
//   x0 = pointer to string
//   w1 = color code (30-37, 90-97)
// Returns: none
// ============================================================================
    .global ansi_print_color
ansi_print_color:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]              // Save callee-saved registers

    mov     x19, x0                          // Save string pointer
    mov     w20, w1                          // Save color code

    // Set color
    mov     w0, w20
    bl      ansi_set_color_fg

    // Print string
    mov     x0, x19
    bl      printf

    // Reset color
    bl      ansi_reset_attributes

    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret
