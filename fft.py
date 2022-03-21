import numpy as np
import sys



if __name__ == "__main__":
    
    n = int(sys.argv[1])
    x = []
    y = []
    hanning = []

    ymax = -100
    ymin = 100
    with open("data.dat") as f:
        i = 0
        j = 0
        for line in f:
            if i > 2:
                a = line.split()
                x.append(float(a[0]))
                y.append(float(a[1]))
            i+=1

    T = 0.02
    N = j
    # sample spacing

    
    rft = np.fft.rfft(y)
    
    
    rft[n:] = np.hanning(n)[0]
    
    y_smooth = np.fft.irfft(rft)

    with open("fft.dat", "w") as f:
        for i in range(len(x)):
            f.write(str(x[i]) + " " + str(round(y_smooth[i], 7)) + "\n")
            
            

