/* Algorithm:
1. Start
2. Define an `ENQUEUE` function:
   a. Check if the queue is full (rear == MAX - 1). If yes, print "Queue Overflow" and return.
   b. Otherwise, increment `rear` and add the element to the queue.
3. Define a `DEQUEUE` function:
   a. Check if the queue is empty (front > rear). If yes, print "Queue Underflow" and return.
   b. Otherwise, remove the element at `front` and increment `front`.
4. Define a `printQueue` function:
   a. If the queue is empty (front > rear), print "Queue is empty".
   b. Otherwise, iterate from `front` to `rear` and print each element.
5. In the `main` function:
   a. Display a menu with options for ENQUEUE, DEQUEUE, PRINT QUEUE, and EXIT.
   b. Take the user's choice as input.
   c. Perform the corresponding operation based on the choice.
   d. Repeat until the user chooses EXIT.
6. End
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int queue[MAX];
int front = 0, rear = -1;

void enqueue(int value) {
    if (rear == MAX - 1) {
        printf("Queue Overflow\n");
    }
    else {
        queue[++rear] = value;
        printf("Queued: %d\n", value);
    }
}

void dequeue() {
    if (front > rear) {
        printf("Queue Underflow\n");
    }
    else {
        printf("Dequeued: %d\n", queue[front++]);
    }
}

void printQueue() {
    if (front > rear) {
        printf("Queue is empty\n");
    }
    else {
        printf("Current Queue:\n");
        for (int i = front; i < rear; i++) {
            printf("%d <- ", queue[i]);
        }
        printf("%d\n", queue[rear]);
    }
}

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