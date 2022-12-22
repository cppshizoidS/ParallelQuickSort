#include <omp.h>
#include <algorithm>
#include <stdio.h>
#include <time.h>

//#define DEBUG

/*!
* quickSort - реализация алгоритма быстрой сортировки
* @param num номер вызова функции (для отладки)
* @param a сортируемый массив
* @param n индекс последнего элемента массива 
*/
void quickSort(int num, float* a, const long n) {
  long i = 0, j = n;
  float  pivot = a[n / 2]; // опорный элемент
  
#ifdef DEBUG
  #pragma omp critical
    {
      printf("enter: %d\n", num);
    }
#endif

  do {
    while (a[i] < pivot) i++;
    while (a[j] > pivot) j--;

    if (i <= j) {
      std::swap(a[i], a[j]);
      i++; j--;
    }
  } while (i <= j);

#ifdef DEBUG
  #pragma omp critical
    {
      printf("divide #%d by %3.2f [%d] -> (%d|%d) : %d\n", num, pivot, n/2, j, n - i, n);
    }
#endif

  if (n < 100) {
    if (j > 0) quickSort(num * 2 + 1, a, j);
    if (n > i) quickSort(num * 2 + 2, a + i, n - i);
    return;
  }
  
#pragma omp task shared(a)
  {
    if (j > 0) quickSort(num * 2 + 1, a, j);
  } // #pragma omp task
#pragma omp task shared(a)
  {
    if (n > i) quickSort(num * 2 + 2, a + i, n - i);
  } // #pragma omp task
#pragma omp taskwait
}
int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("error arg\n");
    return 1;
  }

  float *a;
  long n;

  sscanf(argv[1], "%d", &n);

  a = new float[n];

  srand(time(NULL));
  for (int i = 0; i < n; ++i)
    a[i] = rand() % 10;
  
#ifdef DEBUG
  for (int i = 0; i < n; ++i)
    printf("%3.2f ", a[i]);
  printf("\n");
#endif
  
#pragma omp parallel shared(a)
  {
    #pragma omp single nowait 
    {
      quickSort(0, a, n - 1);
    } // #pragma omp single
  } // #pragma omp parallel
  

  for (int i = 1; i < n; ++i) {
    if (a[i] < a[i-1])
      printf("error\n");
  }
  

  delete []a;
}