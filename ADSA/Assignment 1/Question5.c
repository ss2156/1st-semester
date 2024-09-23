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

// Queue for traversal
typedef struct QueueNode {
    Node* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

void enqueue(Queue* queue, Node* treeNode) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->treeNode = treeNode;
    temp->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = temp;
        return;
    }
    queue->rear->next = temp;
    queue->rear = temp;
}

Node* dequeue(Queue* queue) {
    if (queue->front == NULL)
        return NULL;
    QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL)
        queue->rear = NULL;
    Node* treeNode = temp->treeNode;
    free(temp);
    return treeNode;
}

int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Function to calculate the black-height of a node
int calculateBlackHeight(Node* node) {
    int blackHeight = 0;
    while (node != NULL) {
        if (node->color == BLACK)
            blackHeight++;
        node = node->left;
    }
    return blackHeight;
}

// Function for printing nodes level-by-level and their black-height
void printLevelOrder(Node* root) {
    if (root == NULL) return;

    Queue* queue = createQueue();
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        int levelSize = 0;
        QueueNode* tempQueue = queue->front;

        while (tempQueue) {
            levelSize++;
            tempQueue = tempQueue->next;
        }

        int blackHeight = -1;

        for (int i = 0; i < levelSize; i++) {
            Node* node = dequeue(queue);

            if (node->color == BLACK && blackHeight == -1)
                blackHeight = calculateBlackHeight(node);

            printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");

            if (node->left)
                enqueue(queue, node->left);
            if (node->right)
                enqueue(queue, node->right);
        }
        printf("| Black-Height: %d\n", blackHeight);
    }
}

int main() {
    
    printf("********** Sachin Singh **********\n");
    printf("************ M24CSE033 ***********\n");
    printf("____________________________________\n\n");
    
    Node *root = createNode(20, BLACK);
    root->left = createNode(15, RED);
    root->right = createNode(25, RED);
    root->left->left = createNode(10, BLACK);
    root->left->right = createNode(17, BLACK);
    root->right->right = createNode(30, BLACK);

    printf("Level Order Traversal with Black-Height:\n");
    printLevelOrder(root);

    return 0;
}
