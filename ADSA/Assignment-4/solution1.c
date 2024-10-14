#include <stdio.h>
#include <stdlib.h>

struct BinomialNode {
    int priority;
    int degree;
    struct BinomialNode *parent;
    struct BinomialNode *child;
    struct BinomialNode *sibling;
};

struct BinomialNode* newNode(int priority) {
    struct BinomialNode* node = (struct BinomialNode*)malloc(sizeof(struct BinomialNode));
    node->priority = priority;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->sibling = NULL;
    return node;
}

struct BinomialNode* mergeBinomialTrees(struct BinomialNode* b1, struct BinomialNode* b2) {
    if (b1->priority > b2->priority) {
        struct BinomialNode* temp = b1;
        b1 = b2;
        b2 = temp;
    }
    b2->parent = b1;
    b2->sibling = b1->child;
    b1->child = b2;
    b1->degree++;
    return b1;
}

struct BinomialNode* unionBinomialHeaps(struct BinomialNode* h1, struct BinomialNode* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    struct BinomialNode* res = NULL;
    struct BinomialNode* prev = NULL;
    struct BinomialNode* curr = NULL;
    struct BinomialNode* next = NULL;

    struct BinomialNode* h = (h1->degree <= h2->degree) ? h1 : h2;
    res = h;
    while (h1 && h2) {
        if (h1->degree < h2->degree) {
            curr = h1;
            h1 = h1->sibling;
        } else {
            curr = h2;
            h2 = h2->sibling;
        }

        if (!res) res = curr;
        else prev->sibling = curr;
        prev = curr;
    }

    while (h1) {
        prev->sibling = h1;
        prev = h1;
        h1 = h1->sibling;
    }

    while (h2) {
        prev->sibling = h2;
        prev = h2;
        h2 = h2->sibling;
    }

    curr = res;
    prev = NULL;
    next = curr->sibling;

    while (next) {
        if (curr->degree != next->degree || (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->priority <= next->priority) {
                curr->sibling = next->sibling;
                curr = mergeBinomialTrees(curr, next);
            } else {
                if (!prev) res = next;
                else prev->sibling = next;
                curr = mergeBinomialTrees(next, curr);
            }
        }
        next = curr->sibling;
    }

    return res;
}

struct BinomialNode* insertBinomialHeap(struct BinomialNode* head, int priority) {
    struct BinomialNode* node = newNode(priority);
    return unionBinomialHeaps(head, node);
}

struct BinomialNode* extractMinBinomialHeap(struct BinomialNode* head, int* minPriority) {
    if (!head) return NULL;

    struct BinomialNode* minNode = head;
    struct BinomialNode* prevMin = NULL;
    struct BinomialNode* curr = head;
    struct BinomialNode* prevCurr = NULL;

    while (curr->sibling) {
        if (curr->sibling->priority < minNode->priority) {
            minNode = curr->sibling;
            prevMin = curr;
        }
        prevCurr = curr;
        curr = curr->sibling;
    }

    if (prevMin && minNode->sibling) prevMin->sibling = minNode->sibling;
    else if (!prevMin) head = head->sibling;

    struct BinomialNode* child = minNode->child;
    struct BinomialNode* revChild = NULL;
    while (child) {
        struct BinomialNode* next = child->sibling;
        child->sibling = revChild;
        child->parent = NULL;
        revChild = child;
        child = next;
    }

    *minPriority = minNode->priority;
    free(minNode);

    return unionBinomialHeaps(head, revChild);
}

void displayBinomialHeap(struct BinomialNode* head) {
    if (!head) return;

    struct BinomialNode* curr = head;
    while (curr) {
        printf("%d ", curr->priority);
        displayBinomialHeap(curr->child);
        curr = curr->sibling;
    }
}

int main() {
    int n, operation, priority;
    struct BinomialNode* heap = NULL;
    
    scanf("%d", &n);  // number of operations

    for (int i = 0; i < n; i++) {
        scanf("%d", &operation);
        
        if (operation == 1) {
            // Insert operation
            scanf("%d", &priority);
            heap = insertBinomialHeap(heap, priority);
        } else if (operation == 2) {
            // Extract minimum operation
            int minPriority;
            if (heap) {
                heap = extractMinBinomialHeap(heap, &minPriority);
                printf("%d\n", minPriority);
            }
        } else if (operation == 3) {
            // Display operation
            displayBinomialHeap(heap);
            printf("\n");
        }
    }

    return 0;
}
