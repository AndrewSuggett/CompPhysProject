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
  float Xmax = -1000000, Xmin = 1000000, Ymax = -1000000, Ymin = 1000000, 
  xStart, xEnd, yStart, yEnd;
  
  int N = 3911;
  int fftSmooth = 100;
  FILE  *inputFile = fopen("data.dat", "r");
  FILE  *fftFile;
  
  if(inputFile == NULL) {                        // check the file opebned
      perror("Cannot open file");
      exit(1);
  }
  static float X[3911], Y[3911];
  static float X1[3911-2], Y2[3911-2];
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
      //Get max x and min x values
      if(X[i] > Xmax && X[i] > 0)
        Xmax = X[i];
      if(X[i] < Xmin)
        Xmin = X[i];

      //Get max y and min y values
      if(Y[i] > Ymax)
        Ymax = Y[i];
      if(Y[i] < Ymin)
        Ymin = Y[i];
      
    
      //printf("%f", Y[i]);
    }

    
    
  }
  fclose(inputFile);

  Ymax += 0.01;
  Xmax += 0.01;
  Ymin -= 0.01;
  Ymin -= 0.01;

  xStart = Xmin;
  xEnd = Xmax;
  yStart = Ymin;
  yEnd = Ymax;

  //FFT From Python saves data to file
  callPython(fftSmooth);
  fftFile = fopen("fft.dat", "r"); 
  
  for(int i = 0; i < N-3; i++)
  {
    
    fscanf(fftFile, "%f %f", &X1[i], &Y2[i]);
    //printf("%f\n", X1[i]);
      
  }
  fclose(fftFile);


  //PG Plot Settings
  
  int fttFlag = true, dataFlag = true, scaleFlag = false;
  float cursorX, cursorY;
  char command;

  if(cpgopen("/XWINDOW") != 1)
    return EXIT_FAILURE;
  
  cpgask(false);
  cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
  cpglab("Wavelength", "Intensity", "Spectrum");
  
  cpgline(N, X, Y);
  cpgline(N-3, X1, Y2);
  
  cpgpt(N, X, Y, 1);
  //Commands from the cursor
  while(true)
  {
    int cursorFlag = cpgcurs( &cursorX, &cursorY, &command);
    
    if(command == 'q' || command == 'q')
    {
      break;
    }
    else if(command == 'z')
    {
      xStart = cursorX - 5;
      xEnd = cursorX + 5;
      cpgenv(xStart, xEnd, yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      if(fttFlag)
        cpgline(N-3, X1, Y2);
      if (dataFlag)
        cpgline(N, X, Y);
      
    }
    else if(command == 'r')
    {
      xStart = Xmin;
      xEnd = Xmax;
      cpgenv(xStart, xEnd, yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      if(fttFlag)
        cpgline(N-3, X1, Y2);
      if (dataFlag)
        cpgline(N, X, Y);
      
    }
    else if(command == ']')
    {
      if(fftSmooth < 1000)
        fftSmooth += 10;
      callPython(fftSmooth);
      fftFile = fopen("fft.dat", "r"); 
      
      for(int i = 0; i < N-3; i++)
      {
        
        fscanf(fftFile, "%f %f", &X1[i], &Y2[i]);
        //printf("%f\n", X1[i]);
          
      }
      fclose(fftFile);
      cpgeras();
      cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      cpgsls(2);
      cpgline(N-3, X1, Y2);
      cpgsls(1);
      if(dataFlag == true)
      {
        cpgline(N, X, Y);
      }
    }
    else if(command == '[')
    {
      if(fftSmooth > 20)
        fftSmooth -= 10;
      callPython(fftSmooth);
      fftFile = fopen("fft.dat", "r"); 
      
      for(int i = 0; i < N-3; i++)
      {
        
        fscanf(fftFile, "%f %f", &X1[i], &Y2[i]);
        //printf("%f\n", X1[i]);
          
      }
      fclose(fftFile);
      cpgeras();
      cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      cpgsls(2);
      cpgline(N-3, X1, Y2);
      cpgsls(1);
      if(dataFlag == true)
      {
        cpgline(N, X, Y);
      }
    }
    
    
    
    //Toggle Data Commands
    else if(command == 't')
    {
      //Show the data
      if(fttFlag == false)
      {
        cpgeras();
        cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
        cpglab("Wavelength", "Intensity", "Spectrum");
        cpgsls(2);
        cpgline(N-3, X1, Y2);
        cpgsls(1);

        if(dataFlag == true)
        {
          cpgline(N, X, Y);
        }
        fttFlag = true;
      }
      else
      {
        cpgeras();
        cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
        cpglab("Wavelength", "Intensity", "Spectrum");
        if(dataFlag == true)
        {
          cpgline(N, X, Y);
        }
        fttFlag = false;
      }
      
    }
    else if(command == 'y')
    {
      if(dataFlag == false)
      {
        cpgeras();
        cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
        cpglab("Wavelength", "Intensity", "Spectrum");
        
        cpgline(N, X, Y);
        
        if(fttFlag == true)
        {
          cpgsls(2);
          cpgline(N-3, X1, Y2);
          cpgsls(1);
        }
        dataFlag = true;
      }
      else
      {
        cpgeras();
        cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
        cpglab("Wavelength", "Intensity", "Spectrum");
        if(fttFlag == true)
        {
          cpgsls(2);
          cpgline(N-3, X1, Y2);
          cpgsls(1);
        }
        dataFlag = false;
      }
    
    }
    else if(command == 's')
    {
      if(scaleFlag)
      {
        yStart = 0;
        scaleFlag = false;
      }
      else
      {
        yStart = Ymin;
        scaleFlag = true;
      }
      cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      if(fttFlag)
        cpgline(N-3, X1, Y2);
      if (dataFlag)
        cpgline(N, X, Y);
    }
    else if(command == 'h')
    {
      cpgopen("/CPS");
      cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      cpgline(N, X, Y);
      cpgline(N-3, X1, Y2);
      cpgclos();
      cpgslct(1);
    }

  }
  
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



