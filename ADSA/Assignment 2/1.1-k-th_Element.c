#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } nodeColor; // Enum for node color (Red or Black)

typedef struct Node {
    int data;               // Value of the node
    int size;               // Size of the subtree rooted at this node
    nodeColor color;        // Color of the node (Red or Black)
    struct Node *left, *right, *parent; // Pointers to left child, right child, and parent
} Node;

typedef struct {
    Node *root; // Pointer to the root of the Red-Black Tree
} RedBlackTree;

/**
 * Create a new Red-Black Tree node with the given data
 * @param data: The integer value for the node
 * @return A pointer to the newly created node
 */
Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->size = 1; // Initial size of the node is 1
    newNode->color = RED; // New nodes are always red
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

/**
 * Perform a left rotation on the node 'x'
 * @param tree: Pointer to the Red-Black Tree
 * @param x: Node to be rotated
 */
void leftRotate(RedBlackTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;

    // Update subtree sizes
    y->size = x->size;
    x->size = 1 + (x->left ? x->left->size : 0) + (x->right ? x->right->size : 0);
}

/**
 * Perform a right rotation on the node 'y'
 * @param tree: Pointer to the Red-Black Tree
 * @param y: Node to be rotated
 */
void rightRotate(RedBlackTree *tree, Node *y) {
    Node *x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        tree->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;

    // Update subtree sizes
    x->size = y->size;
    y->size = 1 + (y->left ? y->left->size : 0) + (y->right ? y->right->size : 0);
}

/**
 * Fix the Red-Black Tree property violations after insertion
 * @param tree: Pointer to the Red-Black Tree
 * @param z: Node where the violation might occur
 */
void fixInsert(RedBlackTree *tree, Node *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK; // Ensure the root is always black
}

/**
 * Insert a new node with the given data into the Red-Black Tree
 * @param tree: Pointer to the Red-Black Tree
 * @param data: The data to be inserted
 */
void insert(RedBlackTree *tree, int data) {
    Node *z = createNode(data);
    Node *y = NULL;
    Node *x = tree->root;

    // Standard Binary Search Tree insertion
    while (x != NULL) {
        y = x;
        x->size++; // Increment the size of the current node
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == NULL)
        tree->root = z; // Tree was empty
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    // Fix the Red-Black Tree properties if violated
    fixInsert(tree, z);
}

/**
 * Find the k-th smallest element in the Red-Black Tree
 * @param root: Pointer to the root of the subtree
 * @param k: The rank of the element to be found (1-based index)
 * @return The node containing the k-th smallest element
 */
Node *findKth(Node *root, int k) {
    int leftSize = root->left ? root->left->size : 0;

    if (k == leftSize + 1) // The current node is the k-th smallest
        return root;
    else if (k <= leftSize) // Look in the left subtree
        return findKth(root->left, k);
    else // Look in the right subtree
        return findKth(root->right, k - leftSize - 1);
}

/**
 * Find the k-th smallest element in the Red-Black Tree
 * @param tree: Pointer to the Red-Black Tree
 * @param k: The rank of the element to be found (1-based index)
 * @return The value of the k-th smallest element
 */
int findKthSmallest(RedBlackTree *tree, int k) {
    Node *node = findKth(tree->root, k);
    return node ? node->data : -1;
}

/**
 * Find the k-th largest element in the Red-Black Tree
 * @param tree: Pointer to the Red-Black Tree
 * @param k: The rank of the element to be found (1-based index)
 * @return The value of the k-th largest element
 */
int findKthLargest(RedBlackTree *tree, int k) {
    int n = tree->root->size; // Total number of nodes
    return findKthSmallest(tree, n - k + 1); // K-th largest is the (n-k+1)-th smallest
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    
    RedBlackTree tree = {NULL};

    // Insert elements into the Red-Black Tree

    insert(&tree, 20);
    insert(&tree, 15);
    insert(&tree, 25);
    insert(&tree, 10);
    insert(&tree, 5);
    insert(&tree, 4);
    insert(&tree, 2);
    insert(&tree, 19);
    insert(&tree, 17);

    printf("Enter the vlue of k : ");
    int k;
    scanf("%d", &k); // input for finding the kth smallest and kth largest elements
    printf("k-th Smallest Element: %d\n", findKthSmallest(&tree, k));
    printf("K-th Largest Element: %d\n", findKthLargest(&tree, k));

    return 0;
}

