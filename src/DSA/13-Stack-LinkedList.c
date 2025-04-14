/* Algorithm:
1. Start
2. Define a `Node` structure with:
   a. An integer `data` to store the value.
   b. A pointer `next` to point to the next node.
3. Define a `PUSH` function:
   a. Create a new node.
   b. Assign the value to the node and make it point to the current top of the stack.
   c. Update the top of the stack to the new node.
4. Define a `POP` function:
   a. Check if the stack is empty (top == NULL). If yes, print "Stack Underflow" and return.
   b. Otherwise, remove the top node, update the top to the next node, and free the removed node.
5. Define a `printStack` function:
   a. If the stack is empty (top == NULL), print "Stack is empty".
   b. Otherwise, iterate through the linked list and print each node's data.
6. In the `main` function:
   a. Display a menu with options for PUSH, POP, PRINT STACK, and EXIT.
   b. Take the user's choice as input.
   c. Perform the corresponding operation based on the choice.
   d. Repeat until the user chooses EXIT.
7. End
*/

#include <stdio.h>
#include <stdlib.h>

// Node structure for the stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* top = NULL; // Top of the stack

// Function to push an element onto the stack
void push(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed. Stack Overflow.\n");
        return;
    }
    newNode->data = value;
    newNode->next = top;
    top = newNode;
    printf("Pushed: %d\n", value);
}

// Function to pop an element from the stack
void pop() {
    if (top == NULL) {
        printf("Stack Underflow\n");
        return;
    }
    Node* temp = top;
    printf("Popped: %d\n", top->data);
    top = top->next;
    free(temp);
}

// Function to print the stack
void printStack() {
    if (top == NULL) {
        printf("Stack is empty\n");
        return;
    }
    printf("Current Stack:\n");
    Node* temp = top;
    while (temp->next != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("%d\n", temp->data);
}

// Function to display the menu
void printMenu() {
    system("cls");
    printf("1. PUSH\n");
    printf("2. POP\n");
    printf("3. PRINT STACK\n");
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
            printf("Enter the value to push: ");
            scanf("%d", &value);
            push(value);
            break;
        case 2:
            pop();
            break;
        case 3:
            printStack();
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