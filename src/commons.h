#pragma once
#include <stdio.h>
#include <stdlib.h>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESET "\033[0m"

#if (LOG_LEVEL == 1)
#define LOG(x) printf("\n[LOG] : %s\n", x)
#else
#define LOG(x)
#endif

void footer() {
  printf(
      "\nSurab Parajuli\nSection: A, 4th-Sem\nRoll: 34, Symbol No.: 80010139");
#ifndef __linux
  // syst("read -p \"Press any key to continue...\"");
  system("pause");
#endif
}

#define MAX_SIZE 100

void abortOnError(const char *msg) {
  printf(RED);
  puts(msg);
  printf(RESET);
  footer();
  exit(EXIT_FAILURE);
}

#ifdef SURAB_ARRAY
typedef struct {
  int *arr;
  size_t size;
} Array;

void array_init(Array *arr) {
  if (!arr) {
    abortOnError("CANT INITIALIZE NULL ARRAY");
  }

  arr->arr = (int *)malloc(arr->size * sizeof(int));
  if (arr->arr == NULL) {
    abortOnError("Memory allocation failed\n");
  }
  for (size_t i = 0; i < arr->size; i++) {
    arr->arr[i] = 0;
  }
  LOG("Initialized ARRAY");
}

void array_deinit(Array *arr) {
  if (arr->arr) {
    free(arr->arr);
  }
  arr->size = -1;
}

Array promptArray(const char *arraySizeMsg, const char *arrayElemMsg) {
  Array array = {NULL, 0};
  printf(arraySizeMsg);
  printf("\n");
  scanf("%zu", &array.size);

  array_init(&array);

  if (arrayElemMsg) {
    printf("%s\n", arrayElemMsg);
  } else {
    printf("Enter %zu elements:\n", array.size);
  }
  for (size_t i = 0; i < array.size; i++) {
    scanf("%d", &array.arr[i]);
  }

  return array;
}

void printArray(const Array *array) {
  printf("{ ");
  for (size_t i = 0; i < array->size - 1; i++) {
    printf("%d, ", array->arr[i]);
  }
  printf("%d }\n", array->arr[array->size - 1]);
}
#endif // SURAB_ARRAY

#ifdef SURAB_MATRIX
#define FLOAT_TOLERANCE 0.000001
typedef struct {
  int row;
  int col;
  double data[10][10];
} Matrix;

void inputMatrixDimensions(Matrix *o_mat, const char *msg) {
  printf(msg);
  printf("\n");
  scanf("%d%d", &(o_mat->row), &(o_mat->col));
  printf("\n");
}

void inputMatrix(Matrix *o_mat, const char *msg) {
  printf(msg);
  printf("\n");
  for (int r = 0; r < o_mat->row; r++) {
    printf("  row[%d] <- ", r + 1);
    for (int c = 0; c < o_mat->col; c++) {
      scanf("%lf", &(o_mat->data)[r][c]);
    }
  }
  printf("\n");
}

void printMatrix(const Matrix *matrix, const char *msg) {
  printf(msg);
  printf("\n");
  for (int r = 0; r < matrix->row; r++) {
    printf("| ");
    for (int c = 0; c < matrix->col; c++) {
      printf("%.4lf ", (matrix->data)[r][c]);
    }
    printf("|\n");
  }
  printf("\n");
}
#endif // SURAB_MATRIX

#ifdef SURAB_NM
#include <locale.h>
#include <math.h>
// /// Numerical Method
// Assuming a cubic equation i.e. f(x) = ax³ + bx² + cx + d
typedef struct {
  int a;
  int b;
  int c;
  int d;
} Coeff;

void promptEquation(Coeff *coeff) {
  setlocale(LC_CTYPE, "");
  wprintf(L"Enter coefficients for the equation: f(x) = ax³ + bx² + cx + d\n");
  printf("a = ");
  scanf("%d", &(coeff->a));
  printf("b = ");
  scanf("%d", &(coeff->b));
  printf("c = ");
  scanf("%d", &(coeff->c));
  printf("d = ");
  scanf("%d", &(coeff->d));
}

void promptOneGuess(float *x) {
  printf("Enter initial guess:\n");
  scanf("%f", x);
}

void promptTwoGuess(float *x_lower, float *x_upper) {
  printf("Enter two initial guesses:\n");
  printf("First = ");
  scanf("%f", x_lower);

  printf("Second = ");
  scanf("%f", x_upper);
}

char sign(float d) { return d < 0 ? '-' : '+'; }

void printEquation(Coeff *coeff) {
  printf("f(x) =");
  if (coeff->a != 0) {
    wprintf(L" %c %dx³", sign(coeff->a), abs(coeff->a));
  }
  if (coeff->b != 0) {
    wprintf(L" %c %dx²", sign(coeff->b), abs(coeff->b));
  }
  if (coeff->c != 0) {
    printf(" %c %dx", sign(coeff->c), abs(coeff->c));
  }
  if (coeff->d != 0) {
    printf(" %c %d", sign(coeff->d), abs(coeff->d));
  }
  printf("\n");
}

void trace(int i, float x_lower, float x_upper, float x_mid, float f_lower,
           float f_upper, float f_mid, float prod) {
  printf("----------TRACE (%d)---------\n", i);
  printf("Lower (%.3f, %.3f)\t", x_lower, f_lower);
  printf("Upper (%.3f, %.3f)\n", x_upper, f_upper);
  printf("\t\tMid (%.3f, %.3f)\n", x_mid, f_mid);

  printf("Prod = %c %.3f\n", sign(prod), prod);
  printf("==========TRACE=========\n\n", i);
}
float f(Coeff coeff, float x) {
  float y = coeff.a * powf(x, 3) + coeff.b * (x * x) + coeff.c * x + coeff.d;
  // printf("____(%d, %f)____\n", x, y);
  return y;
}
#endif // SURAB_NM

/// OS related stuffs
#ifdef SURAB_OS
typedef struct {
  int burstTime;
  int waitTime;
  int turnAroundTime;
  int remainingTime;
} Metrics;

typedef struct {
  Metrics *m;
  int count;
} Processes;

typedef struct {
  float waitTime;
  float turnAroundTime;
} AvgMetrics;

void processes_init(Processes *p) {
  if (!p) {
    printf("CANT INITIALIZE NULL PROCESS");
    return;
  }

  p->m = (Metrics *)malloc(sizeof(Metrics) * p->count);
  LOG("Initialized Process struct");
}

void processes_deinit(Processes *p) {
  if (!p) {
    printf("CANT DE-INITIALIZE NULL PROCESS");
    return;
  }

  free(p->m);
  LOG("De-Initialized Process struct");
}

Processes inputMetrics() {
  Processes p;
  printf("Enter number of processes: ");
  scanf("%d", &p.count);
  processes_init(&p);

  printf("Enter burst time for each process:\n");
  for (int i = 0; i < p.count; i++) {
    printf("P[%d]: ", i + 1);
    scanf("%d", &p.m[i].burstTime);
    p.m[i].remainingTime = p.m[i].burstTime;
  }

  LOG("Input Complete");
  // initialize first waiting time to be zero
  p.m[0].waitTime = 0;
  return p;
}

void printProcesses(const Processes *p, const AvgMetrics *avg) {
  // formatted output
  printf("\nProcess\tBT\tWT\tTAT\n");
  for (int i = 0; i < p->count; i++) {
    printf("P%d\t%d\t%d\t%d\n", i + 1, p->m[i].burstTime, p->m[i].waitTime,
           p->m[i].turnAroundTime);
  }
  printf("Average Waiting Time = %f, Average Turn Around Time = %f",
         avg->waitTime, avg->turnAroundTime);
}

AvgMetrics calculateAverageMetrics(const Processes *p) {
  AvgMetrics avg = {0, 0};
  for (int i = 0; i < p->count; i++) {
    avg.turnAroundTime += p->m[i].turnAroundTime;
    avg.waitTime += p->m[i].waitTime;
  }

  avg.waitTime /= p->count;
  avg.turnAroundTime /= p->count;
  return avg;
}
#endif // SURAB_OS
