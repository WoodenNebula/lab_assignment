#pragma once
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_LEVEL 0

#if (LOG_LEVEL == 1)
#define LOG(x) printf("\n[LOG] : %s\n", x)
#else
#define LOG(x)
#endif

void footer() {
  printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
#ifdef __linux__
  // system("read -p \"Press any key to continue...\"");
#else
  system("pause");
#endif
}

#ifdef MISC
#define MAX_SIZE 100
#define FLOAT_TOLERANCE 0.000001

typedef struct {
  int *arr;
  size_t size;
} Array;

Array promptArray() {
  Array array = {NULL, 0};
  printf("Size of Array: ");
  scanf("%zu", &array.size);

  array.arr = (int *)malloc(array.size * sizeof(int));
  if (array.arr == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  printf("Enter %zu elements:\n", array.size);
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

void abortOnError(const char *msg) {
  printf("\e[1;91m");
  printf(msg);
  printf("\e[0m");
  footer();
  exit(EXIT_FAILURE);
}

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

/// Numerical Method
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
#endif // MISC

/// OS related stuffs
#ifdef SURAB_OS

typedef struct {
  int count;
  int *burstTime;
  int *waitTime;
  int *turnAroundTime;
} Metrics;

typedef struct {
  float waitTime;
  float turnAroundTime;
} AvgMetrics;

Metrics init(int processCount) {
  Metrics m;
  m.count = processCount;
  m.burstTime = (int *)malloc(sizeof(int) * processCount);
  m.waitTime = (int *)malloc(sizeof(int) * processCount);
  m.turnAroundTime = (int *)malloc(sizeof(int) * processCount);

  LOG("Initialized Metric struct");
  return m;
}

void deinit(Metrics *m) {
  if (m->count <= 0) {
    printf("ERROR: Metric not initialized");
    return;
  }

  m->count = -1;
  free(m->burstTime);
  free(m->waitTime);
  free(m->turnAroundTime);

  LOG("Cleaned up Metric struct");
}

Metrics inputMetrics() {
  int n;
  printf("Enter number of processes: ");
  scanf("%d", &n);

  Metrics m = init(n);

  printf("Enter burst time for each process:\n");
  for (int i = 0; i < n; i++) {
    printf("P[%d]: ", i + 1);
    scanf("%d", m.burstTime + i);
  }

  // initialize first waiting time to be zero
  m.waitTime[0] = 0;
  return m;
}

void printMetrics(const Metrics *m, const AvgMetrics *avg) {
  // formatted output
  printf("\nProcess\tBT\tWT\tTAT\n");
  for (int i = 0; i < m->count; i++) {
    printf("P%d\t%d\t%d\t%d\n", i + 1, m->burstTime[i], m->waitTime[i],
           m->turnAroundTime[i]);
  }
  printf("Average Waiting Time = %f, Average Turn Around Time = %f",
         avg->waitTime, avg->turnAroundTime);
}
#endif // SURAB_OS
