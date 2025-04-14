/* Algorithm:
1. Start
2. Define a `Node` structure with:
   a. An integer `data` to store the value.
   b. A pointer `next` to point to the next node.
3. Define an `ENQUEUE` function:
   a. Create a new node.
   b. Assign the value to the node and set its `next` pointer to `NULL`.
   c. If the queue is empty (front == NULL), set both `front` and `rear` to the new node.
   d. Otherwise, set the `next` pointer of `rear` to the new node and update `rear`.
4. Define a `DEQUEUE` function:
   a. Check if the queue is empty (front == NULL). If yes, print "Queue Underflow" and return.
   b. Otherwise, remove the front node, update `front` to the next node, and free the removed node.
   c. If the queue becomes empty after the operation, set `rear` to `NULL`.
5. Define a `printQueue` function:
   a. If the queue is empty (front == NULL), print "Queue is empty".
   b. Otherwise, iterate through the linked list and print each node's data.
6. In the `main` function:
   a. Display a menu with options for ENQUEUE, DEQUEUE, PRINT QUEUE, and EXIT.
   b. Take the user's choice as input.
   c. Perform the corresponding operation based on the choice.
   d. Repeat until the user chooses EXIT.
7. End
*/

#include <stdio.h>
#include <stdlib.h>

// Node structure for the queue
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* front = NULL; // Front of the queue
Node* rear = NULL;  // Rear of the queue

// Function to enqueue an element into the queue
void enqueue(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed. Queue Overflow.\n");
        return;
    }
    newNode->data = value;
    newNode->next = NULL;

    if (rear == NULL) {
        // If the queue is empty, both front and rear point to the new node
        front = rear = newNode;
    }
    else {
     // Add the new node at the end of the queue and update rear
        rear->next = newNode;
        rear = newNode;
    }
    printf("Queued: %d\n", value);
}

// Function to dequeue an element from the queue
void dequeue() {
    if (front == NULL) {
        printf("Queue Underflow\n");
        return;
    }
    Node* temp = front;
    printf("Dequeued: %d\n", front->data);
    front = front->next;

    if (front == NULL) {
        // If the queue becomes empty, set rear to NULL
        rear = NULL;
    }
    free(temp);
}

// Function to print the queue
void printQueue() {
    if (front == NULL) {
        printf("Queue is empty\n");
        return;
    }
    printf("Current Queue:\n");
    Node* temp = front;
    while (temp->next != NULL) {
        printf("%d <- ", temp->data);
        temp = temp->next;
    }
    printf("%d\n", temp->data);
}

// Function to display the menu
void printMenu() {
    system("cls");
    printf("1. ENQUEUE\n");
    printf("2. DEQUEUE\n");
    printf("3. PRINT QUEUE\n");
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
            printf("Enter the value to enqueue: ");
            scanf("%d", &value);
            enqueue(value);
            break;
        case 2:
            dequeue();
            break;
        case 3:
            printQueue();
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