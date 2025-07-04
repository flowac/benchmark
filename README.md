# Benchmark
Cross platform benchmark tools  
License is Public Domain unless specified otherwise  

## Simple FPU
| Model | OS | Single | Multi | Speedup | Threads |
| --- | --- | --- | --- | --- | --- |
| MBP M4 Pro | Mac | 10.4943 | 105.097 | 10.0147 | 14 |
| Intel i7 13700k | Cygwin | 3.47788 | 40.9097 | 11.7628 | 24 |
| AWS c7i | AL2 | 2.0742 | 112.212 | 54.0991 | 96 |
| AWS c8g | AL2 | 2.99381 | 11.7789 | 3.93442 | 4 |


## Dhrystone Single Core
``Intel i7 13700k 5.7Ghz performance core: 53887 MIPS``  
``Pinebook Pro RK3399 A72:                  9696 MIPS``  
``Star64 SiFive U74:                        3303 MIPS``  

## Bonnie (Block R/W Per Second) (bonnie++ -b -f -s 2000M)
``ASUS Z790M-Plus FireCuda 530: 1.4 G / 3.9 G``  
``Star64 EMMC:                   39 M / 825 M``  
``Star64 FanXiang S500 Pro:     133 M / 806 M``  

