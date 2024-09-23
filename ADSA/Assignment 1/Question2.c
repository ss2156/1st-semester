#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

Node* createNode(int data, Color color) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = color;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

void inorderTraversal(Node* root) {
    if (root == NULL)
        return;
    inorderTraversal(root->left);
    printf("%d(%s) ", root->data, root->color == RED ? "R" : "B");
    inorderTraversal(root->right);
}

int isRedBlackTree(Node* root);
int isRedBlackTreeHelper(Node* node, int blackCount, int* pathBlackCount);
int checkRedProperty(Node* node);

int isRedBlackTree(Node* root) {
    if (root == NULL) {
        return 1; // An empty tree is a valid Red-Black tree
    }

    if (root->color != BLACK) {
        printf("Root is not black.\n");
        return 0; // Root must be black
    }

    int blackCount = 0;
    int isValid = isRedBlackTreeHelper(root, blackCount, &blackCount);
    if (!isValid) {
        printf("Tree violates Red-Black properties.\n");
    }

    return isValid;
}

int isRedBlackTreeHelper(Node* node, int blackCount, int* pathBlackCount) {
    if (node == NULL) {
        if (*pathBlackCount == 0) {
            *pathBlackCount = blackCount;
        } else if (blackCount != *pathBlackCount) {
            return 0; // Black height mismatch
        }
        return 1; 
    }

    if (node->color == BLACK) {
        blackCount++;
    }

    if (!checkRedProperty(node)) {
        return 0; // Red property violated
    }

    return isRedBlackTreeHelper(node->left, blackCount, pathBlackCount) &&
           isRedBlackTreeHelper(node->right, blackCount, pathBlackCount);
}

int checkRedProperty(Node* node) {
    if (node == NULL) {
        return 1;
    }

    if (node->color == RED) {
        if ((node->left != NULL && node->left->color == RED) ||
            (node->right != NULL && node->right->color == RED)) {
            printf("Red node with red child found at node with data: %d\n", node->data);
            return 0;
        }
    }

    return 1;
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    // Valid Red-Black Tree
    Node* root = createNode(10, BLACK);
    root->left = createNode(5, RED);
    root->right = createNode(15, RED);

    printf("Inorder Traversal of the Red-Black Tree: ");
    inorderTraversal(root);
    printf("\n");

    if (isRedBlackTree(root)) {
        printf("The tree is a valid Red-Black Tree.\n\n");
    } else {
        printf("The tree is NOT a valid Red-Black Tree.\n\n");
    }

    // Invalid Red-Black Tree
    Node* invalidRoot = createNode(10, BLACK);
    invalidRoot->left = createNode(5, RED);
    invalidRoot->right = createNode(15, RED);
    invalidRoot->left->left = createNode(1, RED);  // Invalid because both parent and child are red

    printf("Inorder Traversal of the Red-Black Tree: ");
    inorderTraversal(invalidRoot);
    printf("\n");

    if (isRedBlackTree(invalidRoot)) {
        printf("The tree is a valid Red-Black Tree.\n");
    } else {
        printf("The tree is NOT a valid Red-Black Tree.\n");
    }

    return 0;
}
