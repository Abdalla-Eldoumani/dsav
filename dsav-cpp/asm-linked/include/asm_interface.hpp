/**
 * @file asm_interface.hpp
 * @brief C++ interface to ARMv8 assembly data structure implementations
 *
 * This header declares all assembly functions with C linkage, allowing
 * C++ code to call the assembly implementations.
 *
 * The asm-linked version uses these assembly functions for core data structure
 * operations, while C++ handles visualization, animation, and UI.
 */

#pragma once

#include <cstdint>

// Declare assembly functions with C linkage
extern "C" {

// =============================================================================
// STACK OPERATIONS
// =============================================================================

/**
 * Push a value onto the stack
 * @param value Value to push
 * @return 1 on success, 0 on overflow (stack full)
 */
int stack_push(int value);

/**
 * Pop a value from the stack
 * @param out_value Pointer to store the popped value
 * @return 1 on success, 0 on underflow (stack empty)
 */
int stack_pop(int* out_value);

/**
 * Peek at the top value without removing it
 * @param out_value Pointer to store the top value
 * @return 1 on success, 0 if stack is empty
 */
int stack_peek(int* out_value);

/**
 * Check if stack is empty
 * @return 1 if empty, 0 otherwise
 */
int stack_is_empty();

/**
 * Check if stack is full
 * @return 1 if full, 0 otherwise
 */
int stack_is_full();

/**
 * Clear all elements from the stack
 */
void stack_clear();

// State accessors (need to be added to assembly)
/**
 * Get pointer to internal stack array
 * @return Pointer to stack data array
 */
int* stack_get_data();

/**
 * Get current top index
 * @return Top index (-1 if empty)
 */
int stack_get_top();

/**
 * Get stack capacity
 * @return Maximum number of elements
 */
int stack_get_capacity();


// =============================================================================
// QUEUE OPERATIONS
// =============================================================================

/**
 * Enqueue a value
 * @param value Value to enqueue
 * @return 1 on success, 0 on overflow (queue full)
 */
int queue_enqueue(int value);

/**
 * Dequeue a value
 * @param out_value Pointer to store the dequeued value
 * @return 1 on success, 0 on underflow (queue empty)
 */
int queue_dequeue(int* out_value);

/**
 * Peek at the front value without removing it
 * @param out_value Pointer to store the front value
 * @return 1 on success, 0 if queue is empty
 */
int queue_peek(int* out_value);

/**
 * Check if queue is empty
 * @return 1 if empty, 0 otherwise
 */
int queue_is_empty();

/**
 * Check if queue is full
 * @return 1 if full, 0 otherwise
 */
int queue_is_full();

/**
 * Clear all elements from the queue
 */
void queue_clear();

// State accessors (need to be added to assembly)
/**
 * Get pointer to internal queue array
 * @return Pointer to queue data array
 */
int* queue_get_data();

/**
 * Get current front index
 * @return Front index
 */
int queue_get_front();

/**
 * Get current rear index
 * @return Rear index
 */
int queue_get_rear();

/**
 * Get number of elements in queue
 * @return Element count
 */
int queue_get_count();

/**
 * Get queue capacity
 * @return Maximum number of elements
 */
int queue_get_capacity();


// =============================================================================
// LINKED LIST OPERATIONS
// =============================================================================

/**
 * Node structure matching assembly layout
 * 24 bytes: 8 bytes data + 8 bytes next pointer + 8 bytes padding
 */
struct AsmListNode {
    int64_t data;
    struct AsmListNode* next;
};

/**
 * Create a new linked list node
 * @param value Node value
 * @return Pointer to new node, or nullptr on allocation failure
 */
AsmListNode* list_create_node(int value);

/**
 * Insert a value at the front of the list
 * @param head Pointer to head pointer
 * @param value Value to insert
 */
void list_insert_front(AsmListNode** head, int value);

/**
 * Insert a value at the back of the list
 * @param head Pointer to head pointer
 * @param value Value to insert
 */
void list_insert_back(AsmListNode** head, int value);

/**
 * Delete a value from the list
 * @param head Pointer to head pointer
 * @param value Value to delete
 * @return 1 if deleted, 0 if not found
 */
int list_delete(AsmListNode** head, int value);

/**
 * Search for a value in the list
 * @param head List head
 * @param value Value to search for
 * @return Pointer to node if found, nullptr otherwise
 */
AsmListNode* list_search(AsmListNode* head, int value);

/**
 * Free all nodes in the list
 * @param head Pointer to head pointer (will be set to nullptr)
 */
void list_free_all(AsmListNode** head);


// =============================================================================
// BINARY SEARCH TREE OPERATIONS
// =============================================================================

/**
 * BST Node structure matching assembly layout
 * 24 bytes: 8 bytes data + 8 bytes left + 8 bytes right
 */
struct AsmBSTNode {
    int64_t data;
    struct AsmBSTNode* left;
    struct AsmBSTNode* right;
};

/**
 * Create a new BST node
 * @param value Node value
 * @return Pointer to new node, or nullptr on allocation failure
 */
AsmBSTNode* bst_create_node(int value);

/**
 * Insert a value into the BST
 * @param root Pointer to root pointer
 * @param value Value to insert
 */
void bst_insert(AsmBSTNode** root, int value);

/**
 * Delete a value from the BST
 * @param root Pointer to root pointer
 * @param value Value to delete
 * @return 1 if deleted, 0 if not found
 */
int bst_delete(AsmBSTNode** root, int value);

/**
 * Search for a value in the BST
 * @param root BST root
 * @param value Value to search for
 * @return Pointer to node if found, nullptr otherwise
 */
AsmBSTNode* bst_search(AsmBSTNode* root, int value);

/**
 * Find minimum value node
 * @param root BST root
 * @return Pointer to node with minimum value
 */
AsmBSTNode* bst_find_min(AsmBSTNode* root);

/**
 * Free all nodes in the BST
 * @param root Pointer to root pointer (will be set to nullptr)
 */
void bst_free_all(AsmBSTNode** root);

// Traversal callback type
typedef void (*BSTCallback)(int value);

/**
 * Perform inorder traversal
 * @param root BST root
 * @param callback Function to call for each node
 */
void bst_inorder(AsmBSTNode* root, BSTCallback callback);

/**
 * Perform preorder traversal
 * @param root BST root
 * @param callback Function to call for each node
 */
void bst_preorder(AsmBSTNode* root, BSTCallback callback);

/**
 * Perform postorder traversal
 * @param root BST root
 * @param callback Function to call for each node
 */
void bst_postorder(AsmBSTNode* root, BSTCallback callback);


// =============================================================================
// SORTING ALGORITHMS
// =============================================================================

/**
 * Bubble sort
 * @param arr Array to sort
 * @param size Array size
 */
void bubble_sort(int* arr, int size);

/**
 * Selection sort
 * @param arr Array to sort
 * @param size Array size
 */
void selection_sort(int* arr, int size);

/**
 * Insertion sort
 * @param arr Array to sort
 * @param size Array size
 */
void insertion_sort(int* arr, int size);

// Note: Merge sort and quick sort functions exist but use internal arrays
// We'll need to add versions that accept array pointers


// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

/**
 * Delay for specified milliseconds
 * @param ms Milliseconds to delay
 */
void delay_ms(int ms);

/**
 * Seed the random number generator
 */
void seed_random();

/**
 * Get a random number
 * @return Random 32-bit integer
 */
int get_random();

} // extern "C"
