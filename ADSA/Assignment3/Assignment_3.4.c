#include <stdio.h>

#define MAX_USERS 100001

int parent[MAX_USERS];  // parent[i] points to the parent of node i
int size[MAX_USERS];    // size[i] stores the size of the set whose root is i

// Find function with path compression
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // Path compression
    }
    return parent[x];
}

// Union function with size tracking
void unionSets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX != rootY) {
        // Union by size: attach the smaller tree under the larger tree
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }
}

// Initialize the Union-Find structure
void initialize(int n) {
    for (int i = 1; i <= n; i++) {
        parent[i] = i;  // Each node is initially its own parent (self-grouped)
        size[i] = 1;    // Each group starts with size 1
    }
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    // Initialize the Union-Find data structure
    initialize(n);

    for (int i = 0; i < q; i++) {
        char query_type;
        int x, y;
        scanf(" %c", &query_type);

        if (query_type == 'F') {
            // Friendship Formation: Merge two users' groups
            scanf("%d %d", &x, &y);
            unionSets(x, y);
        } else if (query_type == 'C') {
            // Friendship Check: Are users x and y in the same group?
            scanf("%d %d", &x, &y);
            if (find(x) == find(y)) {
                printf("Yes\n");
            } else {
                printf("No\n");
            }
        } else if (query_type == 'S') {
            // Group Size Query: Print the size of the group containing user x
            scanf("%d", &x);
            printf("%d\n", size[find(x)]);
        }
    }

    return 0;
}
