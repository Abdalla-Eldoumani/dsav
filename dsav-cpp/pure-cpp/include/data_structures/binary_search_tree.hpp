/**
 * @file binary_search_tree.hpp
 * @brief Binary Search Tree data structure implementation
 *
 * A template-based binary search tree with visualization-friendly interface.
 * Maintains BST property: left child < parent < right child.
 */

#pragma once

#include <memory>
#include <optional>
#include <functional>
#include <vector>

namespace dsav {

/**
 * @brief Node structure for binary search tree
 *
 * @tparam T Type of data stored in the node
 */
template<typename T>
struct TreeNode {
    T data;
    std::shared_ptr<TreeNode<T>> left;
    std::shared_ptr<TreeNode<T>> right;

    explicit TreeNode(const T& value)
        : data(value), left(nullptr), right(nullptr) {}
};

/**
 * @brief Binary Search Tree data structure
 *
 * Template parameter:
 * - T: Type of elements stored in the tree
 *
 * Maintains BST property and provides operations for insertion, deletion, and searching.
 */
template<typename T>
class BinarySearchTree {
public:
    /**
     * @brief Construct an empty BST
     */
    BinarySearchTree() = default;

    /**
     * @brief Insert a value into the BST
     *
     * @param value Value to insert
     */
    void insert(const T& value) {
        m_root = insertRecursive(m_root, value);
        m_size++;
    }

    /**
     * @brief Delete a value from the BST
     *
     * @param value Value to delete
     * @return true if value was found and deleted, false otherwise
     */
    bool remove(const T& value) {
        size_t oldSize = m_size;
        m_root = removeRecursive(m_root, value);
        return m_size < oldSize;
    }

    /**
     * @brief Search for a value in the BST
     *
     * @param value Value to search for
     * @return true if found, false otherwise
     */
    bool search(const T& value) const {
        return searchRecursive(m_root, value) != nullptr;
    }

    /**
     * @brief Find a node with specific value (for visualization)
     *
     * @param value Value to find
     * @return Shared pointer to node if found, nullptr otherwise
     */
    std::shared_ptr<TreeNode<T>> find(const T& value) const {
        return searchRecursive(m_root, value);
    }

    /**
     * @brief Check if tree is empty
     *
     * @return true if empty
     */
    bool isEmpty() const {
        return m_root == nullptr;
    }

    /**
     * @brief Get current number of nodes
     *
     * @return Number of nodes
     */
    size_t size() const {
        return m_size;
    }

    /**
     * @brief Clear all nodes
     */
    void clear() {
        m_root = nullptr;
        m_size = 0;
    }

    /**
     * @brief Get root node (for visualization)
     *
     * @return Shared pointer to root node
     */
    std::shared_ptr<TreeNode<T>> root() const {
        return m_root;
    }

    /**
     * @brief Inorder traversal (Left-Root-Right)
     *
     * @param func Function to apply to each node's data
     */
    void inorderTraversal(std::function<void(const T&)> func) const {
        inorderRecursive(m_root, func);
    }

    /**
     * @brief Preorder traversal (Root-Left-Right)
     *
     * @param func Function to apply to each node's data
     */
    void preorderTraversal(std::function<void(const T&)> func) const {
        preorderRecursive(m_root, func);
    }

    /**
     * @brief Postorder traversal (Left-Right-Root)
     *
     * @param func Function to apply to each node's data
     */
    void postorderTraversal(std::function<void(const T&)> func) const {
        postorderRecursive(m_root, func);
    }

    /**
     * @brief Level-order traversal (breadth-first)
     *
     * @return Vector of values in level-order
     */
    std::vector<T> levelOrderTraversal() const {
        std::vector<T> result;
        if (!m_root) return result;

        std::vector<std::shared_ptr<TreeNode<T>>> queue;
        queue.push_back(m_root);

        while (!queue.empty()) {
            auto node = queue.front();
            queue.erase(queue.begin());

            result.push_back(node->data);

            if (node->left) queue.push_back(node->left);
            if (node->right) queue.push_back(node->right);
        }

        return result;
    }

    /**
     * @brief Get height of the tree
     *
     * @return Height (number of edges from root to deepest leaf)
     */
    int height() const {
        return heightRecursive(m_root);
    }

private:
    std::shared_ptr<TreeNode<T>> insertRecursive(std::shared_ptr<TreeNode<T>> node, const T& value) {
        if (!node) {
            return std::make_shared<TreeNode<T>>(value);
        }

        if (value < node->data) {
            node->left = insertRecursive(node->left, value);
        } else if (value > node->data) {
            node->right = insertRecursive(node->right, value);
        }
        // If value == node->data, don't insert duplicates

        return node;
    }

    std::shared_ptr<TreeNode<T>> removeRecursive(std::shared_ptr<TreeNode<T>> node, const T& value) {
        if (!node) {
            return nullptr;
        }

        if (value < node->data) {
            node->left = removeRecursive(node->left, value);
        } else if (value > node->data) {
            node->right = removeRecursive(node->right, value);
        } else {
            // Node found, delete it
            m_size--;

            // Case 1: No children or one child
            if (!node->left) {
                return node->right;
            } else if (!node->right) {
                return node->left;
            }

            // Case 2: Two children
            // Find inorder successor (smallest in right subtree)
            auto successor = findMin(node->right);
            node->data = successor->data;
            node->right = removeRecursive(node->right, successor->data);
            m_size++; // Compensate for double decrement
        }

        return node;
    }

    std::shared_ptr<TreeNode<T>> searchRecursive(std::shared_ptr<TreeNode<T>> node, const T& value) const {
        if (!node || node->data == value) {
            return node;
        }

        if (value < node->data) {
            return searchRecursive(node->left, value);
        } else {
            return searchRecursive(node->right, value);
        }
    }

    std::shared_ptr<TreeNode<T>> findMin(std::shared_ptr<TreeNode<T>> node) const {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    void inorderRecursive(std::shared_ptr<TreeNode<T>> node, std::function<void(const T&)> func) const {
        if (!node) return;
        inorderRecursive(node->left, func);
        func(node->data);
        inorderRecursive(node->right, func);
    }

    void preorderRecursive(std::shared_ptr<TreeNode<T>> node, std::function<void(const T&)> func) const {
        if (!node) return;
        func(node->data);
        preorderRecursive(node->left, func);
        preorderRecursive(node->right, func);
    }

    void postorderRecursive(std::shared_ptr<TreeNode<T>> node, std::function<void(const T&)> func) const {
        if (!node) return;
        postorderRecursive(node->left, func);
        postorderRecursive(node->right, func);
        func(node->data);
    }

    int heightRecursive(std::shared_ptr<TreeNode<T>> node) const {
        if (!node) return -1;
        return 1 + std::max(heightRecursive(node->left), heightRecursive(node->right));
    }

    std::shared_ptr<TreeNode<T>> m_root = nullptr;  ///< Root of the tree
    size_t m_size = 0;                              ///< Number of nodes
};

} // namespace dsav
