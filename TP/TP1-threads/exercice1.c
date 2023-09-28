#include <pthread.h>
#include <stdio.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
long long count = 0;

long long incrementCount() {
  pthread_mutex_lock(&count_mutex);
  long long c = ++count;
  pthread_mutex_unlock(&count_mutex);
  return c;
}

void *threadFunA(void *ignored) {
  long long i;
  while ((i = incrementCount()) < 50000)
    if (i % 10000 == 0)
      printf("A: i = %lld\n", i);
  return NULL;
}
void *threadFunB(void *ignored) {
  long long i;
  while ((i = incrementCount()) < 100000)
    if (i % 10000 == 0)
      printf("B: i = %lld\n", i);
  return NULL;
}
int main(int argc, char **argv) {
  pthread_t pth_a, pth_b;
  pthread_create(&pth_a, NULL, threadFunA, NULL);
  pthread_create(&pth_b, NULL, threadFunB, NULL);
  pthread_join(pth_a, NULL);
  pthread_join(pth_b, NULL);
}
