/* Algorithm:
1. Start
2. Define an `ENQUEUE` function:
   a. Check if the queue is full ((rear + 1) % MAX == front). If yes, print "Queue Overflow" and return.
   b. Otherwise, increment `rear` circularly using `(rear + 1) % MAX` and add the element to the queue.
3. Define a `DEQUEUE` function:
   a. Check if the queue is empty (front == -1). If yes, print "Queue Underflow" and return.
   b. Otherwise, remove the element at `front`.
   c. If the queue becomes empty after the operation (front == rear), reset `front` and `rear` to -1.
   d. Otherwise, increment `front` circularly using `(front + 1) % MAX`.
4. Define a `printQueue` function:
   a. If the queue is empty (front == -1), print "Queue is empty".
   b. Otherwise, iterate from `front` to `rear` circularly and print each element.
5. In the `main` function:
   a. Display a menu with options for ENQUEUE, DEQUEUE, PRINT QUEUE, and EXIT.
   b. Take the user's choice as input.
   c. Perform the corresponding operation based on the choice.
   d. Repeat until the user chooses EXIT.
6. End
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 5

int queue[MAX];
int front = -1, rear = -1;

int isFull() {
    return ((rear + 1) % MAX == front) || (front == 0 && rear == MAX - 1);
}

int isEmpty() {
    return front == -1;
}

void enqueue() {
    if (isFull()) {
        printf("Queue Overflow\n");
        return;
    }

    int value;
    printf("Enter the value to enqueue: ");
    scanf("%d", &value);

    if (isEmpty()) {
        front = 0;
    }
    rear = (rear + 1) % MAX;
    queue[rear] = value;
    printf("Queued: %d\n", value);
}

void dequeue() {
    if (isEmpty()) {
        printf("Queue Underflow\n");
    }
    else {
        printf("Dequeued: %d\n", queue[front]);
        if (front == rear) {
            front = -1;
            rear = -1;
        }
        else {
            front = (front + 1) % MAX;
        }
    }
}

void printQueue() {
    if (isEmpty()) {
        printf("Queue is empty\n");
    }
    else {
        printf("Current Queue:\n");
        int i = front;
        while (1) {
            printf("%d ", queue[i]);
            if (i == rear) {
                break;
            }
            i = (i + 1) % MAX;
        }
        printf("\n");
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
            enqueue();
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