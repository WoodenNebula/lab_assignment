#include <stdio.h>
#include <stdlib.h>

// Node structure for the singly linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL; // Head of the linked list

// Function to add a node to the linked list
void addNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->data = value;
    newNode->next = NULL;

    if (head == NULL) {
        // If the list is empty, set head to the new node
        head = newNode;
    }
    else {
     // Traverse to the last node
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        // Add the new node at the end
        temp->next = newNode;
    }
    printf("Added: %d\n", value);
}

// Function to delete a node from the linked list
void deleteNode(int value) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* temp = head;
    Node* prev = NULL;

    // Check if the head node contains the value
    if (temp != NULL && temp->data == value) {
        head = temp->next; // Update head to the next node
        free(temp);        // Free the old head
        printf("Deleted: %d\n", value);
        return;
    }

    // Traverse the list to find the node to delete
    while (temp != NULL && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    // If the value was not found
    if (temp == NULL) {
        printf("Value not found in the list\n");
        return;
    }

    // Unlink the node and free its memory
    prev->next = temp->next;
    free(temp);
    printf("Deleted: %d\n", value);
}

// Function to print the linked list
void printList() {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }

    printf("Current List:\n");
    Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Function to display the menu
void printMenu() {
    system("cls");
    printf("1. ADD NODE\n");
    printf("2. DELETE NODE\n");
    printf("3. PRINT LIST\n");
    printf("4. EXIT\n");
    printf("Enter your choice: ");
}

int main() {
    int choice, value;

    while (1) {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter the value to add: ");
            scanf("%d", &value);
            addNode(value);
            break;
        case 2:
            printList();
            printf("Enter the value to delete: ");
            scanf("%d", &value);
            deleteNode(value);
            break;
        case 3:
            printList();
            break;
        case 4:
            printf("Exiting...\n");
            printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
            system("pause");
            return 0;
        default:
            printf("Invalid choice! Please try again.\n");
        }
        system("pause");
    }
}