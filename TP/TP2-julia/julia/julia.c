#include "julia.h"
#include <stddef.h>
#include <stdlib.h>

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
  unsigned char *img = malloc(width * height * sizeof(unsigned char));

  write_bmp_header(out, width, height);
  fwrite(img, width * height * 3, 1, out);
  fclose(out);

  return 0;
}
