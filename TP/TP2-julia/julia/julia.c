#include "julia.h"


int main(int argc, char** argv) {
   FILE *out=fopen("outfile.bmp","w");
   if (out == NULL) {
      perror("Cannot open outfile");
      exit(1);
   }
   unsigned char* img = NULL;
   
   // YOUR CODE HERE
   write_bmp_header(out, width, height);
   fwrite(img, width*height*3, 1, out);
   fclose(out);
   
   return 0;
}
