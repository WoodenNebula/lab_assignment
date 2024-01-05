/* Input two number and any one operator (+ - * / %) and perform the operation
using a function and output it */

#include <stdio.h>

typedef enum{
    ERROR = -1, INT = 0, FLOAT
}Type;

typedef struct{
    Type data_type;
    int i_data;
    float f_data;
} Result;


Result Calculate(int a, int b, char operation) {
    Result result;
    switch (operation) {
        case '+':
            result.data_type = INT;
            result.i_data = a + b;
            break;
        case '-':
            result.data_type = INT;
            result.i_data = a - b;
            break;
        case '*':
            result.data_type = INT;
            result.i_data = a * b;
            break;
        case '/':
            if(b == 0){
                result.data_type = ERROR;
                return result;
            }
            result.data_type = FLOAT;
            result.f_data = (float)a / b;
            break;
        case '%':
            if(b == 0){
                result.data_type = ERROR;
                return result;
            }
            result.data_type = INT;
            result.i_data = a % b;
            break;
        default:
            puts("BAD OPERATOR!");
            break;
    }
    return result;
}


int main(int argc, char *argv[]) {
    int operandA = 0, operandB = 0;
    char operation = '\0';
    puts("Enter a simple binary operation(2+2) [without any space] to perform:");
    // This here somehow neglects any space and works as intended which idk
    // how, so this is magic code
    scanf("%d %c %d", &operandA, &operation, &operandB);

    Result result = Calculate(operandA, operandB, operation);

    if (result.data_type == INT) {
        printf("%d %c %d = %d", operandA, operation, operandB, result.i_data);
    } else if(result.data_type == FLOAT){
        printf("%d %c %d = %.2f", operandA, operation, operandB, result.f_data);
    } else {
        puts("Divide by 0!");
    }

    return 0;
}