#include "cpgplot.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>


#define PI 3.1415926535897932384
#define true 1
#define false 0

void callPython(int n);


int main()
{
  int N = 3911;
  FILE  *inputFile = fopen("data.dat", "r");
  FILE  *fftFile;
  
  if(inputFile == NULL) {                        // check the file opebned
      perror("Cannot open file");
      exit(1);
  }
  static float X[3911], Y[3911];
  char c[1000];

  for(int i = 0; i < N; i++)
  {
    //printf("%d\n", i);
    if(i == 0 || i == 1)
    {
      fscanf(inputFile, "%[^\n]\n", &c[0]);
      printf("%s\n", c);
    }
    else
    {
      fscanf(inputFile, "  %f     %f", &X[i], &Y[i]);
      
      //printf("%f", Y[i]);
    }
      
    
  }


 
  fclose(inputFile);


  callPython(500);
  static float X1[3911-2], Y2[3911-2];
  
  fftFile = fopen("fft.dat", "r"); 
  
  for(int i = 0; i < N-3; i++)
  {
    
    fscanf(fftFile, "%f %f", &X1[i], &Y2[i]);
    printf("%f\n", X1[i]);
      
  }
  fclose(fftFile);


  if(cpgbeg(0, "?", 1, 1) != 1)
    return EXIT_FAILURE;
  cpgenv(5250, 5328.5800, 0.8, 1.2, 0, 1);
  cpglab("(x)", "(y)", "PGPLOT Example 1: y = x\\u2\\d");
  //cpgline(N, X, Y);
  cpgline(N-3, X1, Y2);
  //cpgpt(N, x1, y1, 1);

  cpgend();
  return EXIT_SUCCESS;
}

void callPython(int n)
{
  char strNum[10];
  char command[100] = "python3 fft.py ";
  sprintf(strNum, "%d", n);
  strcat(command, strNum);
  system (command);

}



