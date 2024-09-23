#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100

typedef struct Contact {
    int roll_number;
    char name[MAX_NAME_LENGTH];
    struct Contact *left, *right;
} Contact;

// Helper function to create a new contact
Contact* createContact(int roll_number, const char* name) {
    Contact* new_contact = (Contact*)malloc(sizeof(Contact));
    new_contact->roll_number = roll_number;
    strcpy(new_contact->name, name);
    new_contact->left = new_contact->right = NULL;
    return new_contact;
}

// Helper function to insert or update a contact in the BST
Contact* addContact(Contact* root, int roll_number, const char* name) {
    if (root == NULL) {
        return createContact(roll_number, name);
    }

    if (roll_number < root->roll_number) {
        root->left = addContact(root->left, roll_number, name);
    } else if (roll_number > root->roll_number) {
        root->right = addContact(root->right, roll_number, name);
    } else {
        // If the contact already exists, update the name
        strcpy(root->name, name);
    }
    return root;
}

// Helper function to find the minimum contact in a subtree
Contact* findMin(Contact* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Helper function to delete a contact in the BST
Contact* deleteContact(Contact* root, int roll_number) {
    if (root == NULL) {
        return NULL;
    }

    if (roll_number < root->roll_number) {
        root->left = deleteContact(root->left, roll_number);
    } else if (roll_number > root->roll_number) {
        root->right = deleteContact(root->right, roll_number);
    } else {
        // Node to be deleted found
        if (root->left == NULL) {
            Contact* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Contact* temp = root->left;
            free(root);
            return temp;
        }

        // If the node has two children, replace it with the minimum node from the right subtree
        Contact* temp = findMin(root->right);
        root->roll_number = temp->roll_number;
        strcpy(root->name, temp->name);
        root->right = deleteContact(root->right, temp->roll_number);
    }
    return root;
}

// Helper function to find a contact in the BST
void findContact(Contact* root, int roll_number) {
    if (root == NULL) {
        printf("NA\n");
        return;
    }

    if (roll_number < root->roll_number) {
        findContact(root->left, roll_number);
    } else if (roll_number > root->roll_number) {
        findContact(root->right, roll_number);
    } else {
        printf("%s\n", root->name);
    }
}

int main() {
    int n;
    scanf("%d", &n); // Read the number of operations

    Contact* root = NULL; // Initialize the root of the BST

    for (int i = 0; i < n; i++) {
        char operation;
        int roll_number;
        char name[MAX_NAME_LENGTH];

        scanf(" %c", &operation); // Read the operation

        if (operation == 'A') {
            // Add contact operation
            scanf("%d %s", &roll_number, name);
            root = addContact(root, roll_number, name);
        } else if (operation == 'D') {
            // Delete contact operation
            scanf("%d", &roll_number);
            root = deleteContact(root, roll_number);
        } else if (operation == 'F') {
            // Find contact operation
            scanf("%d", &roll_number);
            findContact(root, roll_number);
        }
    }

    return 0;
}
