# Benchmark
Cross platform benchmark tools  
License is Public Domain unless specified otherwise  

## Simple CPU Benchmark
| Model | OS | FPU | Multi | Speedup | INT | Multi | Speedup | Threads |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MBA M4 | Mac | 5.51 | 28.24 | 5.12 | 2.18 | 15.80 | 7.26 | 10 |
| Intel i7 13700k | WSL2 | 2.45 | 25.36 | 10.35 | 2.05 | 43.80 | 21.35 | 24 |
| Intel i7 13700k | Cygwin | 2.30 | 29.91 | 13.01 | 2.06 | 42.55 | 20.67 | 24 |


## Dhrystone Single Core
``Intel i7 13700k 5.7Ghz performance core: 53887 MIPS``  
``Pinebook Pro RK3399 A72:                  9696 MIPS``  
``Star64 SiFive U74:                        3303 MIPS``  

## Bonnie (Block R/W Per Second) (bonnie++ -b -f -s 2000M)
``ASUS Z790M-Plus FireCuda 530: 1.4 G / 3.9 G``  
``Star64 EMMC:                   39 M / 825 M``  
``Star64 FanXiang S500 Pro:     133 M / 806 M``  

