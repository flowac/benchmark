# Benchmark
Cross platform benchmark tools  
License is Public Domain unless specified otherwise  

## Simple CPU Benchmark
| Model | OS | FPU | Multi | Speedup | INT | Multi | Speedup | Threads |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MBA M4 | Mac | 13.32 | 69.68 | 5.23 | 5.44 | 37.07 | 6.81 | 10 |
| Intel i7 13700k | Cygwin | 5.67 | 72.83 | 12.85 | 5.13 | 104.02 | 20.26 | 24 |

=== FPU RESULT ===
Single: 750.965 ms = 13.3162
Multi:  14351.2 ms = 69.6807
Speedup: 5.23278x  Threads: 10
.....Single thread 0.....
Multi thread 0 0 0 0 0 0 0 0 0 0
=== INT RESULT ===
Single: 1837.17 ms = 5.44317
Multi:  26975 ms = 37.0713
Speedup: 6.81061x  Threads: 10

### Deprecated FPU Only
| Model | OS | Single | Multi | Speedup | Threads |
| --- | --- | --- | --- | --- | --- |
| MBP M4 Pro | Mac | 10.49 | 105.10 | 10.01 | 14 |
| MBA M4 | Mac | 10.30 | 53.80 | 5.22 | 10 |
| Intel i7 13700k | Cygwin | 3.48 | 40.91 | 11.76 | 24 |
| AWS c8g | AL2 | 2.99 | 11.78 | 3.93 | 4 |
| AWS c7i | AL2 | 2.07 | 112.21 | 54.10 | 96 |


## Dhrystone Single Core
``Intel i7 13700k 5.7Ghz performance core: 53887 MIPS``  
``Pinebook Pro RK3399 A72:                  9696 MIPS``  
``Star64 SiFive U74:                        3303 MIPS``  

## Bonnie (Block R/W Per Second) (bonnie++ -b -f -s 2000M)
``ASUS Z790M-Plus FireCuda 530: 1.4 G / 3.9 G``  
``Star64 EMMC:                   39 M / 825 M``  
``Star64 FanXiang S500 Pro:     133 M / 806 M``  

