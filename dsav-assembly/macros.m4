dnl ============================================================================
dnl macros.m4 - Shared Macro Definitions for ARMv8 DSAV Project
dnl ============================================================================
dnl This file contains m4 macros used across all assembly modules.
dnl Include at the top of every .asm file with: include(`macros.m4')
dnl ============================================================================

dnl NOTE: Using standard m4 quoting (backtick ` and single quote ')
dnl to avoid conflicts with assembly syntax that uses square brackets

dnl ----------------------------------------------------------------------------
dnl REGISTER ALLOCATION MACROS
dnl Define semantic names for registers to improve code readability
dnl ----------------------------------------------------------------------------

dnl Common temporary registers (caller-saved)
define(`tmp1', `w9')
define(`tmp2', `w10')
define(`tmp3', `w11')
define(`tmp1_x', `x9')
define(`tmp2_x', `x10')
define(`tmp3_x', `x11')

dnl Common counter/index registers
define(`ctr_reg', `w12')
define(`idx_reg', `w13')
define(`idx_reg_x', `x13')

dnl ----------------------------------------------------------------------------
dnl STACK FRAME MACROS
dnl Helpers for function prologue/epilogue with proper alignment
dnl ----------------------------------------------------------------------------

dnl Calculate aligned stack allocation (must be 16-byte aligned)
define(`ALIGNED_ALLOC', `ifelse(eval(($1) % 16), 0, `-$1', `-(($1) + 16 - (($1) % 16))')')

dnl Standard function entry (saves fp and lr)
dnl Usage: FUNC_ENTRY(bytes_needed)
define(`FUNC_ENTRY', `
    stp     x29, x30, [sp, ALIGNED_ALLOC($1)]!
    mov     x29, sp
')

dnl Standard function exit (restores fp and lr)
dnl Usage: FUNC_EXIT(bytes_needed)
define(`FUNC_EXIT', `
    ldp     x29, x30, [sp], -ALIGNED_ALLOC($1)
    ret
')

dnl ----------------------------------------------------------------------------
dnl CONSTANT DEFINITIONS
dnl Common values used throughout the project
dnl ----------------------------------------------------------------------------

define(`TRUE', `1')
define(`FALSE', `0')
define(`NULL', `0')

dnl ANSI Color Codes (for reference, actual strings in ansi.asm)
define(`COLOR_RESET', `0')
define(`COLOR_RED', `31')
define(`COLOR_GREEN', `32')
define(`COLOR_YELLOW', `33')
define(`COLOR_BLUE', `34')
define(`COLOR_MAGENTA', `35')
define(`COLOR_CYAN', `36')
define(`COLOR_WHITE', `37')

dnl ----------------------------------------------------------------------------
dnl DATA STRUCTURE CONSTANTS
dnl ----------------------------------------------------------------------------

define(`ARRAY_MAX_SIZE', `20')
define(`STACK_MAX_SIZE', `8')
define(`QUEUE_MAX_SIZE', `8')

dnl Linked List Node Layout
define(`NODE_DATA_OFFSET', `0')
define(`NODE_NEXT_OFFSET', `8')
define(`NODE_SIZE', `16')

dnl ----------------------------------------------------------------------------
dnl UTILITY MACROS
dnl ----------------------------------------------------------------------------

dnl Print a string literal (for debugging)
dnl Usage: PRINT_STR("Hello, World!\n")
define(`PRINT_STR', `
    adrp    x0, .Lstr_$1
    add     x0, x0, :lo12:.Lstr_$1
    bl      printf
    .section .rodata
.Lstr_$1: .string "$1"
    .text
')