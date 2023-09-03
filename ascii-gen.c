/* 
   A program that converts an input image to its equivalent in ASCII characters. 
   It takes in the name of the input image (in bitmap format) and the name of the 
   output text file to be generated.

   Author: Manimegalai Vaiyapuri
   Date:   April 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[]) {

  // Validate arguments
  if (argc != 3) {
    printf("Usage: ./ascii-gen  <inputfile> <outputfile>\n");
    exit(1);
  }

  char *infile = argv[1];
  char *outfile = argv[2];

  // Load the bitmap from input file
  Bitmap *bm = bm_load(infile);
  if (bm == NULL) {
    printf("[error] could not load %s\n", infile);
    exit(1);
  }
  
  // Convert bitmap to grayscale (RGB will have same values)
  bm_grayscale(bm);

  // Save the width and height
  int width = bm_width(bm);
  int height = bm_height(bm);

  // ASCII characters of decreasing intensity. (from darkest to lightest)
  char map[10] = "@#%xo;:,. ";
  // Bucket size to place color values from 0-255 in 10 buckets
  int bucket = 255 / 10;

  // Get the raw pixel data from the bitmap
  unsigned char *data = bm_raw_data(bm);

  // Open output file for writing ASCII text image
  FILE *fptr = fopen(outfile, "w");


  //  We loop through the pixel array for height x width pixels
  //  *  take the RED value of each pixel, convert it to a value between 0 and 9 
  //     (to index into the ASCII character array)
  //  *  print the ASCII character at that index
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // Get the array index of RED value of next pixel
      int s = (i * width * 4) + (j * 4);
      int idx  = (data[s] / bucket) - 1;
      if (idx < 0) idx = 0;
      if (idx > 9) idx = 9;
      // Print the equivalent ASCII character
      fprintf(fptr, "%c", map[idx]);
    }
    // We finished a row, emit a new line for next pixel row
    fprintf(fptr, "\n");
  }
  // Close output file
  fclose(fptr);
};
