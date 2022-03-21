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
  int fftSmooth = 170;
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
  int fttFlag = true, dataFlag = true, scaleFlag = false, fftPointFlag = true, dataPointFlag = true;
  float cursorX, cursorY, yScale = 0.1, xScale = 5;
  char command;


  if(cpgopen("/XWINDOW") != 1)
    return EXIT_FAILURE;
  
  cpgask(false);
  
  cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
  cpglab("Wavelength", "Intensity", "Sirius Chiron 8 Feb 2021 100 exposures");
  cpgmtxt("T",2.0,0.0,0.0,"Q=QUIT, H=Hardcopy");
  cpgmtxt("T",1.0,0.0,0.0,"r=Reset View, Left-Click=Go To, u=Toggle FTT Line, i=Toggle FTT Points, s=Toggle Scale");
  cpgmtxt("T",0.2,0.0,0.0,"o=Toggle Data Line, p=Toggle Data Points, [=Smooth FTT , ]=Rough FTT");
  cpgmtxt("B",4,0.0,0.0,"z=Zoom In X, x=Zoom Out X, c=Zoom In Y, v=Zoom Out Y");
  cpgsls(2);
  cpgline(N-3, X1, Y2);
  cpgsls(1);
  cpgline(N, X, Y);
  cpgpt(N, X, Y, 1);

  //Commands from the cursor
  while(true)
  {
    int cursorFlag = cpgcurs( &cursorX, &cursorY, &command);
    
    if(command == 'q' || command == 'Q')
    {
      break;
    }
    else if(command == 'A')
    {
      xStart = cursorX - xScale;
      xEnd = cursorX + xScale;
      yStart = cursorY - yScale;
      yEnd = cursorY + yScale;
    }
    else if(command == 'c')
    {
      if(yScale > 0.02)
        yScale -= 0.02;
      
      yStart = cursorY - yScale;
      yEnd = cursorY + yScale;
    }
    else if(command == 'v')
    {
     
      yScale += 0.02;
      
      yStart = cursorY - yScale;
      yEnd = cursorY + yScale;
    }
    else if(command == 'z')
    {
      if(xScale > 1)
        xScale -= 1;
      
      xStart = cursorX - xScale;
      xEnd = cursorX + xScale;
    }
    else if(command == 'x')
    {
     
      xScale += 1;
      
      xStart = cursorX - xScale;
      xEnd = cursorX + xScale;
    }
    else if(command == 'r')
    {
      xStart = Xmin;
      xEnd = Xmax;
      yStart = Ymin;
      yEnd = Ymax;
      yScale = 0.1;
      xScale = 5;

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
    }
    else if(command == '[')
    {
      if(fftSmooth > 180)
        fftSmooth -= 10;
      callPython(fftSmooth);
      fftFile = fopen("fft.dat", "r"); 
      
      for(int i = 0; i < N-3; i++)
      {
        
        fscanf(fftFile, "%f %f", &X1[i], &Y2[i]);
        //printf("%f\n", X1[i]);
          
      }
      fclose(fftFile);
    }
    
    //Toggle Data Commands
    else if(command == 'u')
    {
      //Show the data
      if(fttFlag == false)
      {
        fttFlag = true;
      }
      else
      {
        fttFlag = false;
      }
   
      
    }
    else if(command == 'o')
    {
      if(dataFlag == false)
      {
        
        dataFlag = true;
      }
      else
      {
        dataFlag = false;
      }
      
    }
    else if(command == 'i')
    {
      if(fftPointFlag == false)
      {
        
        fftPointFlag = true;
      }
      else
      {
        fftPointFlag = false;
      }
    }
    else if(command == 'p')
    {
      if(dataPointFlag == false)
      {
        
        dataPointFlag = true;
      }
      else
      {
        dataPointFlag = false;
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
      
    }
    else if(command == 'h')
    {
      cpgopen("/CPS");
      cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
      cpglab("Wavelength", "Intensity", "Spectrum");
      cpgline(N, X, Y);
      cpgpt(N, X, Y, 1);
      cpgline(N-3, X1, Y2);
      cpgclos();
      cpgslct(1);
    }


    cpgenv(xStart, xEnd , yStart, yEnd, 0, 1);
    cpglab("Wavelength", "Intensity", "Sirius Chiron 8 Feb 2021 100 exposures");
    cpgmtxt("T",2.0,0.0,0.0,"Q=QUIT, H=Hardcopy");
    cpgmtxt("T",1.0,0.0,0.0,"r=Reset View, Left-Click=Go To, u=Toggle FTT Line, i=Toggle FTT Points");
    cpgmtxt("T",0.2,0.0,0.0,"o=Toggle Data Line, p=Toggle Data Points, [=Smooth FTT , ]=Rough FTT");
    cpgmtxt("B",4,0.0,0.0,"z=Zoom In X, x=Zoom Out X, c=Zoom In Y, v=Zoom Out Y");
    if(fttFlag)
    {
      cpgsls(2);
      cpgline(N-3, X1, Y2);
      cpgsls(1);
    }
    if(fftPointFlag)
    {
      cpgpt(N-3, X1, Y2, 1);
    }
    if(dataPointFlag)
    {
      cpgpt(N, X, Y, 1);
    } 
    if(dataFlag)
    {
      cpgline(N, X, Y);
      
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



