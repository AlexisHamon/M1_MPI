#define _GNU_SOURCE
#include "julia.h"
#include <stddef.h>
#include <stdlib.h>
#include <sched.h>

int cpucount() {
  cpu_set_t cpuset;
  sched_getaffinity(0, sizeof(cpuset), &cpuset);
  return CPU_COUNT(&cpuset);
}

int main(int argc, char **argv) {
   if (argc < 2)
    return 1;
   int n = atoi(argv[1]);

   if (n < 1)
      return 0;
   int width = 2 * n;
   int height = n; 
   
  FILE *out = fopen("outfile.bmp", "w");
  if (out == NULL) {
    perror("Cannot open outfile");
    exit(1);
  }
  unsigned char *img = malloc(width * height * 3 * sizeof(unsigned char));

  for (int y=0; y<height; ++y) {
    for (int x=0; x<width; ++x) {
      compute_julia_pixel(x, y, width, height, 1.0, &img[(y * width + x) * 3]);
    }
  }
  
  write_bmp_header(out, width, height);
  fwrite(img, width * height * 3, 1, out);
  fclose(out);

  return 0;
}
