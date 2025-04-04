/* Algorithm:
1. Start
2. Define a `PUSH` function:
   a. Check if the stack is full (top == MAX - 1). If yes, print "Stack Overflow" and return.
   b. Otherwise, increment `top` and add the element to the stack.
3. Define a `POP` function:
   a. Check if the stack is empty (top == -1). If yes, print "Stack Underflow" and return.
   b. Otherwise, remove the top element and decrement `top`.
4. Define a `printStack` function:
   a. If the stack is empty (top == -1), print "Stack is empty".
   b. Otherwise, iterate from `top` to 0 and print each element.
5. In the `main` function:
   a. Display a menu with options for PUSH, POP, PRINT STACK, and EXIT.
   b. Take the user's choice as input.
   c. Perform the corresponding operation based on the choice.
   d. Repeat until the user chooses EXIT.
6. End
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int stack[MAX];
int top = -1;

void push(int value) {
    if (top == MAX - 1) {
        printf("Stack Overflow\n");
    }
    else {
        stack[++top] = value;
        printf("Pushed: %d\n", value);
    }
}

void pop() {
    if (top == -1) {
        printf("Stack Underflow\n");
    }
    else {
        printf("Popped: %d\n", stack[top--]);
    }
}

void printStack() {
    if (top == -1) {
        printf("Stack is empty\n");
    }
    else {
        printf("Current Stack:\n");
        for (int i = top; i > 0; i--) {
            printf("%d -> ", stack[i]);
        }
        printf("%d\n", stack[0]);
    }
}

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