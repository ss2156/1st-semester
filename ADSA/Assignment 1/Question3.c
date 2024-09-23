#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    int leftRotations;
    int rightRotations;
    int colorChanges;
} InsertionMetrics;

Node* createNode(int data, Color color) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = color;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

Node* insertBST(Node* root, Node* pt) {
    if (root == NULL)
        return pt;

    if (pt->data < root->data) {
        root->left = insertBST(root->left, pt);
        root->left->parent = root;
    } else if (pt->data > root->data) {
        root->right = insertBST(root->right, pt);
        root->right->parent = root;
    }

    return root;
}

void leftRotate(Node **root, Node *pt, InsertionMetrics* metrics) {
    Node *pt_right = pt->right;

    pt->right = pt_right->left;
    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;
    if (pt->parent == NULL)
        *root = pt_right;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;

    metrics->leftRotations++;
}

void rightRotate(Node **root, Node *pt, InsertionMetrics* metrics) {
    Node *pt_left = pt->left;

    pt->left = pt_left->right;
    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;
    if (pt->parent == NULL)
        *root = pt_left;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;

    metrics->rightRotations++;
}

void fixViolation(Node **root, Node *pt, InsertionMetrics* metrics) {
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;

    while ((pt != *root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left) {
            Node *uncle_pt = grand_parent_pt->right;

            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
                metrics->colorChanges += 3;
            } else {
                if (pt == parent_pt->right) {
                    leftRotate(root, parent_pt, metrics);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rightRotate(root, grand_parent_pt, metrics);
                Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
                metrics->colorChanges += 2;
            }
        } else {
            Node *uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != NULL) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
                metrics->colorChanges += 3;
            } else {
                if (pt == parent_pt->left) {
                    rightRotate(root, parent_pt, metrics);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                leftRotate(root, grand_parent_pt, metrics);
                Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
                metrics->colorChanges += 2;
            }
        }
    }

    (*root)->color = BLACK;
    metrics->colorChanges++;
}

void insert(Node **root, int data, InsertionMetrics* metrics) {
    Node *pt = createNode(data, RED);
    *root = insertBST(*root, pt);
    fixViolation(root, pt, metrics);
}

void inorderTraversal(Node* root) {
    if (root == NULL)
        return;
    inorderTraversal(root->left);
    printf("%d(%s) ", root->data, root->color == RED ? "R" : "B");
    inorderTraversal(root->right);
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    Node *root = NULL;
    InsertionMetrics metrics = {0, 0, 0};

    int elements[] = {21, 15, 25, 12, 5, 1, 30, 32, 44, 50};
    int n = sizeof(elements) / sizeof(elements[0]);

    for (int i = 0; i < n; i++) {
        // Store metrics before insertion
        int initialLeftRotations = metrics.leftRotations;
        int initialRightRotations = metrics.rightRotations;
        int initialColorChanges = metrics.colorChanges;

        insert(&root, elements[i], &metrics);

        int leftRotations = metrics.leftRotations - initialLeftRotations;
        int rightRotations = metrics.rightRotations - initialRightRotations;
        int colorChanges = metrics.colorChanges - initialColorChanges;

        printf("Inserted %d: Inorder Traversal: ", elements[i]);
        inorderTraversal(root);
        printf("\n");
        printf("Rotations and Color Changes for %d:\n", elements[i]);
        printf("  Left Rotations: %d\n", leftRotations);
        printf("  Right Rotations: %d\n", rightRotations);
        printf("  Color Changes: %d\n\n", colorChanges);
    }

    // Print the total number of rotations and color changes
    printf("\nTotal Rotations and Color Changes:\n");
    printf("  Total Left Rotations: %d\n", metrics.leftRotations);
    printf("  Total Right Rotations: %d\n", metrics.rightRotations);
    printf("  Total Color Changes: %d\n", metrics.colorChanges);

    return 0;
}
