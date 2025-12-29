/**
 * @file red_black_tree.hpp
 * @brief Red-Black Tree data structure implementation
 *
 * A self-balancing binary search tree with the following properties:
 * 1. Every node is either RED or BLACK
 * 2. Root is always BLACK
 * 3. All NIL (null) leaves are BLACK
 * 4. RED nodes have BLACK children (no two consecutive REDs)
 * 5. All paths from root to NIL have same number of BLACK nodes
 */

#pragma once

#include <memory>
#include <optional>
#include <functional>
#include <vector>

namespace dsav {

/**
 * @brief Node color enumeration
 */
enum class RBColor {
    RED,
    BLACK
};

/**
 * @brief Node structure for red-black tree
 *
 * @tparam T Type of data stored in the node
 */
template<typename T>
struct RBTreeNode {
    T data;
    RBColor color;
    std::shared_ptr<RBTreeNode<T>> left;
    std::shared_ptr<RBTreeNode<T>> right;
    std::shared_ptr<RBTreeNode<T>> parent;  // Required for RB tree operations

    explicit RBTreeNode(const T& value, RBColor nodeColor = RBColor::RED)
        : data(value), color(nodeColor), left(nullptr), right(nullptr), parent(nullptr) {}
};

/**
 * @brief Event types for RB tree operations (for visualization)
 */
enum class RBTreeEventType {
    InsertNode,          // Node inserted at position
    Recolor,             // Node color changed
    RotateLeft,          // Left rotation performed
    RotateRight,         // Right rotation performed
    Case1_UncleRed,      // Fixup case 1: uncle is RED
    Case2_Triangle,      // Fixup case 2: triangle configuration
    Case3_Line,          // Fixup case 3: line configuration
    SetRootBlack,        // Root forced to BLACK
    DeleteNode,          // Node deleted
    DeleteFixup          // Deletion fixup operation
};

/**
 * @brief Event record for RB tree operations
 */
template<typename T>
struct RBTreeEvent {
    RBTreeEventType type;
    T nodeValue;                    // Primary node (z)
    std::optional<T> parentValue;   // Parent (p)
    std::optional<T> grandparentValue;  // Grandparent (g)
    std::optional<T> uncleValue;    // Uncle (u)
    RBColor fromColor;              // Original color
    RBColor toColor;                // New color
    std::string explanation;        // Human-readable explanation

    RBTreeEvent(RBTreeEventType t, const T& val, const std::string& exp = "")
        : type(t), nodeValue(val), fromColor(RBColor::BLACK), toColor(RBColor::BLACK), explanation(exp) {}
};

/**
 * @brief Red-Black Tree data structure
 *
 * Template parameter:
 * - T: Type of elements stored in the tree
 *
 * Self-balancing BST maintaining RB properties through rotations and recoloring.
 */
template<typename T>
class RedBlackTree {
public:
    /**
     * @brief Construct an empty RB tree
     */
    RedBlackTree() = default;

    /**
     * @brief Insert a value into the RB tree
     *
     * @param value Value to insert
     */
    void insert(const T& value) {
        auto newNode = std::make_shared<RBTreeNode<T>>(value, RBColor::RED);

        if (!m_root) {
            m_root = newNode;
            m_root->color = RBColor::BLACK;  // Property 2: root is BLACK
            m_size++;
            return;
        }

        // Standard BST insertion
        auto parent = insertBST(m_root, newNode);
        newNode->parent = parent;

        m_size++;

        // Fix RB tree properties
        fixInsert(newNode);
    }

    /**
     * @brief Delete a value from the RB tree
     *
     * @param value Value to delete
     * @return true if value was found and deleted, false otherwise
     */
    bool remove(const T& value) {
        auto nodeToDelete = searchRecursive(m_root, value);
        if (!nodeToDelete) {
            return false;  // Value not found
        }

        deleteNode(nodeToDelete);
        m_size--;
        return true;
    }

    /**
     * @brief Search for a value in the RB tree
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
    std::shared_ptr<RBTreeNode<T>> find(const T& value) const {
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
    std::shared_ptr<RBTreeNode<T>> root() const {
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
     * @brief Level-order traversal (breadth-first)
     *
     * @return Vector of values in level-order
     */
    std::vector<T> levelOrderTraversal() const {
        std::vector<T> result;
        if (!m_root) return result;

        std::vector<std::shared_ptr<RBTreeNode<T>>> queue;
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

    /**
     * @brief Get black height of the tree (for verification)
     *
     * @return Black height (number of black nodes from root to any leaf)
     */
    int blackHeight() const {
        return blackHeightRecursive(m_root);
    }

    /**
     * @brief Verify RB tree properties (for debugging)
     *
     * @return true if all properties are satisfied
     */
    bool verifyProperties() const {
        if (!m_root) return true;

        // Property 2: Root must be black
        if (m_root->color != RBColor::BLACK) return false;

        // Check all other properties recursively
        int blackHeight = -1;
        return verifyPropertiesRecursive(m_root, 0, blackHeight);
    }

    /**
     * @brief Enable event recording for visualization
     */
    void enableEventRecording() {
        m_recordEvents = true;
        m_events.clear();
    }

    /**
     * @brief Disable event recording
     */
    void disableEventRecording() {
        m_recordEvents = false;
    }

    /**
     * @brief Get recorded events and clear the list
     *
     * @return Vector of events
     */
    std::vector<RBTreeEvent<T>> getAndClearEvents() {
        std::vector<RBTreeEvent<T>> result = std::move(m_events);
        m_events.clear();
        return result;
    }

    /**
     * @brief Check if events are being recorded
     */
    bool isRecordingEvents() const {
        return m_recordEvents;
    }

private:
    /**
     * @brief Insert node using BST rules, return parent
     */
    std::shared_ptr<RBTreeNode<T>> insertBST(std::shared_ptr<RBTreeNode<T>> root,
                                             std::shared_ptr<RBTreeNode<T>> newNode) {
        if (newNode->data < root->data) {
            if (root->left) {
                return insertBST(root->left, newNode);
            } else {
                root->left = newNode;
                return root;
            }
        } else if (newNode->data > root->data) {
            if (root->right) {
                return insertBST(root->right, newNode);
            } else {
                root->right = newNode;
                return root;
            }
        }
        // Duplicate value - don't insert
        return root;
    }

    /**
     * @brief Fix RB tree properties after insertion
     *
     * @param node Newly inserted node (starts RED)
     */
    void fixInsert(std::shared_ptr<RBTreeNode<T>> node) {
        while (node != m_root && node->parent && node->parent->color == RBColor::RED) {
            auto parent = node->parent;
            auto grandparent = parent->parent;

            if (!grandparent) break;

            // Parent is left child of grandparent
            if (parent == grandparent->left) {
                auto uncle = grandparent->right;

                // Case 1: Uncle is RED
                if (uncle && uncle->color == RBColor::RED) {
                    parent->color = RBColor::BLACK;
                    uncle->color = RBColor::BLACK;
                    grandparent->color = RBColor::RED;
                    node = grandparent;  // Move up and continue
                } else {
                    // Case 2: Node is right child (triangle case)
                    if (node == parent->right) {
                        node = parent;
                        rotateLeft(node);
                        parent = node->parent;
                        if (!parent) break;
                        grandparent = parent->parent;
                        if (!grandparent) break;
                    }

                    // Case 3: Node is left child (line case)
                    parent->color = RBColor::BLACK;
                    grandparent->color = RBColor::RED;
                    rotateRight(grandparent);
                }
            }
            // Parent is right child of grandparent (mirror cases)
            else {
                auto uncle = grandparent->left;

                // Case 1: Uncle is RED
                if (uncle && uncle->color == RBColor::RED) {
                    parent->color = RBColor::BLACK;
                    uncle->color = RBColor::BLACK;
                    grandparent->color = RBColor::RED;
                    node = grandparent;
                } else {
                    // Case 2: Node is left child (triangle case)
                    if (node == parent->left) {
                        node = parent;
                        rotateRight(node);
                        parent = node->parent;
                        if (!parent) break;
                        grandparent = parent->parent;
                        if (!grandparent) break;
                    }

                    // Case 3: Node is right child (line case)
                    parent->color = RBColor::BLACK;
                    grandparent->color = RBColor::RED;
                    rotateLeft(grandparent);
                }
            }
        }

        // Ensure root is always black
        m_root->color = RBColor::BLACK;
    }

    /**
     * @brief Rotate left around node
     *
     *     x                y
     *    / \              / \
     *   a   y     =>     x   c
     *      / \          / \
     *     b   c        a   b
     */
    void rotateLeft(std::shared_ptr<RBTreeNode<T>> x) {
        auto y = x->right;
        if (!y) return;

        x->right = y->left;
        if (y->left) {
            y->left->parent = x;
        }

        y->parent = x->parent;
        if (!x->parent) {
            m_root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    /**
     * @brief Rotate right around node
     *
     *       y            x
     *      / \          / \
     *     x   c   =>   a   y
     *    / \              / \
     *   a   b            b   c
     */
    void rotateRight(std::shared_ptr<RBTreeNode<T>> y) {
        auto x = y->left;
        if (!x) return;

        y->left = x->right;
        if (x->right) {
            x->right->parent = y;
        }

        x->parent = y->parent;
        if (!y->parent) {
            m_root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;
    }

    /**
     * @brief Delete a node from the tree (RB tree deletion)
     */
    void deleteNode(std::shared_ptr<RBTreeNode<T>> z) {
        if (!z) return;

        std::shared_ptr<RBTreeNode<T>> y = z;
        std::shared_ptr<RBTreeNode<T>> x = nullptr;
        RBColor yOriginalColor = y->color;

        if (!z->left) {
            // Case 1: No left child
            x = z->right;
            transplant(z, z->right);
        } else if (!z->right) {
            // Case 2: No right child
            x = z->left;
            transplant(z, z->left);
        } else {
            // Case 3: Two children - find successor
            y = findMin(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                if (x) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                if (y->right) y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            if (y->left) y->left->parent = y;
            y->color = z->color;
        }

        // If we deleted a BLACK node, we need to fix violations
        if (yOriginalColor == RBColor::BLACK && x) {
            fixDelete(x);
        }
        
        // Ensure root is BLACK
        if (m_root) {
            m_root->color = RBColor::BLACK;
        }
    }

    /**
     * @brief Replace subtree rooted at u with subtree rooted at v
     */
    void transplant(std::shared_ptr<RBTreeNode<T>> u, std::shared_ptr<RBTreeNode<T>> v) {
        if (!u->parent) {
            m_root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }

        if (v) {
            v->parent = u->parent;
        }
    }

    /**
     * @brief Fix RB tree properties after deletion
     */
    void fixDelete(std::shared_ptr<RBTreeNode<T>> x) {
        while (x != m_root && x && x->color == RBColor::BLACK) {
            if (x == x->parent->left) {
                auto w = x->parent->right;  // Sibling

                if (w && w->color == RBColor::RED) {
                    // Case 1: Sibling is RED
                    w->color = RBColor::BLACK;
                    x->parent->color = RBColor::RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }

                if (w && (!w->left || w->left->color == RBColor::BLACK) &&
                    (!w->right || w->right->color == RBColor::BLACK)) {
                    // Case 2: Sibling is BLACK, both children BLACK
                    w->color = RBColor::RED;
                    x = x->parent;
                } else {
                    if (w && (!w->right || w->right->color == RBColor::BLACK)) {
                        // Case 3: Sibling BLACK, left child RED, right child BLACK
                        if (w->left) w->left->color = RBColor::BLACK;
                        w->color = RBColor::RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    // Case 4: Sibling BLACK, right child RED
                    if (w) {
                        w->color = x->parent->color;
                        x->parent->color = RBColor::BLACK;
                        if (w->right) w->right->color = RBColor::BLACK;
                        rotateLeft(x->parent);
                    }
                    x = m_root;
                }
            } else {
                // Mirror cases (x is right child)
                auto w = x->parent->left;

                if (w && w->color == RBColor::RED) {
                    w->color = RBColor::BLACK;
                    x->parent->color = RBColor::RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }

                if (w && (!w->right || w->right->color == RBColor::BLACK) &&
                    (!w->left || w->left->color == RBColor::BLACK)) {
                    w->color = RBColor::RED;
                    x = x->parent;
                } else {
                    if (w && (!w->left || w->left->color == RBColor::BLACK)) {
                        if (w->right) w->right->color = RBColor::BLACK;
                        w->color = RBColor::RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    if (w) {
                        w->color = x->parent->color;
                        x->parent->color = RBColor::BLACK;
                        if (w->left) w->left->color = RBColor::BLACK;
                        rotateRight(x->parent);
                    }
                    x = m_root;
                }
            }
        }

        if (x) {
            x->color = RBColor::BLACK;
        }
    }

    /**
     * @brief Find minimum node in subtree
     */
    std::shared_ptr<RBTreeNode<T>> findMin(std::shared_ptr<RBTreeNode<T>> node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    /**
     * @brief Search for a value recursively
     */
    std::shared_ptr<RBTreeNode<T>> searchRecursive(std::shared_ptr<RBTreeNode<T>> node, const T& value) const {
        if (!node || node->data == value) {
            return node;
        }

        if (value < node->data) {
            return searchRecursive(node->left, value);
        } else {
            return searchRecursive(node->right, value);
        }
    }

    /**
     * @brief Inorder traversal helper
     */
    void inorderRecursive(std::shared_ptr<RBTreeNode<T>> node, std::function<void(const T&)> func) const {
        if (!node) return;
        inorderRecursive(node->left, func);
        func(node->data);
        inorderRecursive(node->right, func);
    }

    /**
     * @brief Height calculation helper
     */
    int heightRecursive(std::shared_ptr<RBTreeNode<T>> node) const {
        if (!node) return -1;
        return 1 + std::max(heightRecursive(node->left), heightRecursive(node->right));
    }

    /**
     * @brief Black height calculation helper
     */
    int blackHeightRecursive(std::shared_ptr<RBTreeNode<T>> node) const {
        if (!node) return 1;  // NIL nodes are black

        int leftBH = blackHeightRecursive(node->left);
        int blackIncrement = (node->color == RBColor::BLACK) ? 1 : 0;

        return leftBH + blackIncrement;
    }

    /**
     * @brief Verify RB tree properties recursively
     */
    bool verifyPropertiesRecursive(std::shared_ptr<RBTreeNode<T>> node, int blackCount, int& pathBlackHeight) const {
        if (!node) {
            // Reached NIL node
            if (pathBlackHeight == -1) {
                pathBlackHeight = blackCount;
            }
            // Property 5: All paths have same black height
            return pathBlackHeight == blackCount;
        }

        // Property 4: RED node must have BLACK children
        if (node->color == RBColor::RED) {
            if ((node->left && node->left->color == RBColor::RED) ||
                (node->right && node->right->color == RBColor::RED)) {
                return false;
            }
        }

        int newBlackCount = blackCount + (node->color == RBColor::BLACK ? 1 : 0);

        return verifyPropertiesRecursive(node->left, newBlackCount, pathBlackHeight) &&
               verifyPropertiesRecursive(node->right, newBlackCount, pathBlackHeight);
    }

    /**
     * @brief Record an event if recording is enabled
     */
    void recordEvent(RBTreeEvent<T> event) {
        if (m_recordEvents) {
            m_events.push_back(std::move(event));
        }
    }

    std::shared_ptr<RBTreeNode<T>> m_root = nullptr;  ///< Root of the tree
    size_t m_size = 0;                                ///< Number of nodes

    // Event recording for visualization
    bool m_recordEvents = false;                      ///< Enable/disable event recording
    std::vector<RBTreeEvent<T>> m_events;             ///< Recorded events
};

} // namespace dsav
