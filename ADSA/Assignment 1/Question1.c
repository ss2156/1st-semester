#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = RED; 
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

void leftRotate(Node **root, Node *pt) {
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
}

void rightRotate(Node **root, Node *pt) {
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
}

void fixViolation(Node **root, Node *pt) {
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
            } else {
                if (pt == parent_pt->right) {
                    leftRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rightRotate(root, grand_parent_pt);
                Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        } else {
            Node *uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != NULL) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->left) {
                    rightRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                leftRotate(root, grand_parent_pt);
                Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }
    }

    (*root)->color = BLACK;
}

void insert(Node **root, int data) {
    Node *pt = createNode(data);
    *root = insertBST(*root, pt);
    fixViolation(root, pt);
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteBST(Node* root, int data) {
    if (root == NULL)
        return root;

    if (data < root->data)
        root->left = deleteBST(root->left, data);
    else if (data > root->data)
        root->right = deleteBST(root->right, data);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteBST(root->right, temp->data);
        }
    }

    if (root == NULL)
        return root;

    return root;
}

Node* search(Node* root, int data) {
    if (root == NULL || root->data == data)
        return root;

    if (data < root->data)
        return search(root->left, data);

    return search(root->right, data);
}

void inorderTraversal(Node* root) {
    if (root == NULL)
        return;
    inorderTraversal(root->left);
    printf("%d ", root->data);
    inorderTraversal(root->right);
}

int main() {
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    
    Node *root = NULL;

    int elements[] = {20, 15, 25, 10, 5, 1, 30, 35, 40, 50};
    int n = sizeof(elements) / sizeof(elements[0]);

    for (int i = 0; i < n; i++) {
        insert(&root, elements[i]);
    }

    printf("Inorder Traversal of Created Tree: \n");
    inorderTraversal(root);
    printf("\n\n");

    int searchKey = 25;
    Node* found = search(root, searchKey);
    if (found)
        printf("Element %d found in tree.\n\n", searchKey);
    else
        printf("Element %d not found in tree.\n\n", searchKey);

    root = deleteBST(root, 25);

    printf("Inorder Traversal after Deletion of 25: \n");
    inorderTraversal(root);
    printf("\n");

    return 0;
}
