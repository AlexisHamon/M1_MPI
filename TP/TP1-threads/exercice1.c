#include <pthread.h>
#include <stdio.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
long long count = 0;

void increment_count() {
  pthread_mutex_lock(&count_mutex);
  count = count + 1;
  pthread_mutex_unlock(&count_mutex);
}

long long get_count() {
  pthread_mutex_lock(&count_mutex);
  long long c = count;
  pthread_mutex_unlock(&count_mutex);
  return (c);
}
void* thread_funA(void* ignored) {
  int i;
  while ((i = get_count()) < 50000) {
    if (i%10000==0) printf("A: i = %d\n", i);
    increment_count();
  }
  return NULL;
}
void *thread_funB(void *ignored) {
  int i;
  while ((i = get_count()) < 100000) {
    if (i % 10000 == 0) printf("B: i = %d\n", i);
    increment_count();
  }
  return NULL;
}
int main(int argc, char **argv) {
  pthread_t pthA, pthB;
  pthread_create(&pthA, NULL, thread_funA, NULL);
  pthread_create(&pthB, NULL, thread_funB, NULL);
  pthread_join(pthA, NULL);
  pthread_join(pthB, NULL);
}
