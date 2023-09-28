#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct thread_args {
  int i;
  int *sum;
  pthread_mutex_t *mutex;
};

void *threadFun(void *ptr) {
  struct thread_args *args = (struct thread_args *)ptr;
  printf("%d: Hello World !\n", args->i);
  printf("%d: Pid: %d\n", args->i, getpid());
  printf("%d: %p\n", args->i, (void *)pthread_self());

  int sum = 0;
  for (unsigned i = 0; i < 1000000; ++i)
    sum += args->i;

  pthread_mutex_lock(args->mutex);
  *(args->sum) += sum;
  pthread_mutex_unlock(args->mutex);

  free(args);
  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2)
    return 1;
  int n = atoi(argv[1]);

  if (n < 1)
    return 0;

  pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;
  int sum = 0;

  pthread_t *pth_t = malloc(n * sizeof(pthread_t));

  for (int i = 0; i < n; ++i) {
    struct thread_args *arg = malloc(sizeof(struct thread_args));
    arg->i = i;
    arg->sum = &sum;
    arg->mutex = &sum_mutex;
    pthread_create(&pth_t[i], NULL, threadFun, (void *)arg);
  }
  for (unsigned i = 0; i < n; ++i)
    pthread_join(pth_t[i], NULL);

  free(pth_t);

  printf("Somme: %d\n", sum);
}
