# Benchmark
Cross platform benchmark tools  
License is Public Domain unless specified otherwise  

## Simple CPU Benchmark
| Model | OS | FPU | Multi | Speedup | INT | Multi | Speedup | Threads |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| MBA M4 | 24.5.0 | 5.51 | 28.24 | 5.12 | 2.18 | 15.80 | 7.26 | 10 |
| i7 13700k | 6.6.87 | 2.45 | 25.36 | 10.35 | 2.05 | 43.80 | 21.35 | 24 |
| i7 13700k | 10.0-19045 | 2.30 | 29.91 | 13.01 | 2.06 | 42.55 | 20.67 | 24 |
| Pi 5 | 6.12.34 | 0.88 | 3.51 | 3.99 | 1.19 | 4.74 | 4.00 | 4 |


## Dhrystone Single Core
``i7 13700k:  53887 MIPS``  
``MBA M4:     41443 MIPS``  
``Pi 5:       20400 MIPS``  
``PBP RK3399:  9696 MIPS``  
``Star64 U74:  3303 MIPS``  

## Bonnie (Block R/W Per Second) (bonnie++ -b -f -s 2000M)
``ASUS Z790M-Plus FireCuda 530: 1.4 G / 3.9 G``  
``Star64 EMMC:                   39 M / 825 M``  
``Star64 FanXiang S500 Pro:     133 M / 806 M``  

