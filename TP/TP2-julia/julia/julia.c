#define _GNU_SOURCE

#define ceild(n, d) (((n) < 0) ? -((-(n)) / (d)) : ((n) + (d)-1) / (d))
#define min(x, y) ((x) < (y) ? (x) : (y))

#include "julia.h"
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <sched.h>

int cpucount() {
  cpu_set_t cpuset;
  sched_getaffinity(0, sizeof(cpuset), &cpuset);
  return CPU_COUNT(&cpuset);
}

struct img {
  int width;
  int height;
  unsigned char* pixel;
};

struct thread_args {
  struct img *img;
  int th;
  int lby;
};

void *threadCalc(void *ptr) {
  struct thread_args *args = (struct thread_args *)ptr;
  
  for (int y=(args->th * args->lby); y<min(args->img->height, ((args->th+1) * args->lby)); ++y) {
    for (int x=0; x<args->img->width; ++x) {
      compute_julia_pixel(x, y, args->img->width, args->img->height, 1.0, &args->img->pixel[(y * args->img->width + x) * 3]);
    }
  }

  free(args);
  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2)
  return 1;
  int n = atoi(argv[1]);

  if (n < 1)
    return 0;

  struct img* img = malloc(sizeof(struct img));
  img->width = 2 * n;
  img->height = n; 
  img->pixel = malloc(img->width * img->height * 3 * sizeof(unsigned char));

  FILE *out = fopen("outfile.bmp", "w");
  if (out == NULL) {
    perror("Cannot open outfile");
    exit(1);
  }

  int cpu_count = cpucount();

  int lby = ceild(img->height, cpu_count);
  pthread_t *pth_t = malloc(cpu_count * sizeof(pthread_t));

  for (int th=0; th<cpu_count; ++th) {
    struct thread_args *args = malloc(sizeof(struct thread_args));
    args->th = th;
    args->lby = lby;
    args->img = img;
    pthread_create(&pth_t[th], NULL, threadCalc, (void *)args);
  }
  
  for (unsigned th = 0; th<cpu_count; ++th)
    pthread_join(pth_t[th], NULL);
  free(pth_t);

  write_bmp_header(out, img->width, img->height);
  fwrite(img->pixel, img->width * img->height * 3, 1, out);
  fclose(out);

  free(img->pixel);
  free(img);
  return 0;
}
