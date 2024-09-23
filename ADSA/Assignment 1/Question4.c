#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *parent, *left, *right;
} Node;

// Function to create a new node
Node* createNode(int data, Color color) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->color = color;
    node->parent = node->left = node->right = NULL;
    return node;
}

// Function to Left rotate 
void leftRotate(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Function to Right rotate 
void rightRotate(Node **root, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

// Function to replace node u with node v
void replaceNode(Node **root, Node *u, Node *v) {
    if (u->parent == NULL)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

// Fixing violations after deletion
void deleteFix(Node **root, Node *x) {
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

// Function to delete a node 
void deleteNode(Node **root, Node *z) {
    Node *y = z;
    Node *x;
    Color yOriginalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        replaceNode(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        replaceNode(root, z, z->left);
    } else {
        y = z->right;
        while (y->left != NULL)
            y = y->left;

        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else {
            replaceNode(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        replaceNode(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (yOriginalColor == BLACK)
        deleteFix(root, x);
}

// Function to insert a node 
void insertNode(Node **root, Node *node) {
    Node *y = NULL;
    Node *x = *root;

    while (x != NULL) {
        y = x;
        if (node->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y == NULL)
        *root = node;
    else if (node->data < y->data)
        y->left = node;
    else
        y->right = node;

    node->left = node->right = NULL;
    node->color = RED;


}

void inorder(Node *root) {
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main() {
    
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    
    Node *root = NULL;

    // Inserting nodes into the Red-Black Tree
    insertNode(&root, createNode(20, BLACK));
    insertNode(&root, createNode(15, RED));
    insertNode(&root, createNode(25, RED));
    insertNode(&root, createNode(10, BLACK));
    insertNode(&root, createNode(5, RED));
    insertNode(&root, createNode(30, BLACK));

    printf("Inorder before deletion: ");
    inorder(root);
    printf("\n\n");

    // A. Node to be deleted is the root
    deleteNode(&root, root);

    printf("Inorder after deleting root: ");
    inorder(root);
    printf("\n\n");

    // B. Node to be deleted has two children 
    Node *nodeWithTwoChildren = root->left;  
    deleteNode(&root, nodeWithTwoChildren);

    printf("Inorder after deleting node with two children: ");
    inorder(root);
    printf("\n");

    return 0;
}
