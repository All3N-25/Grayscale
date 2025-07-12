How To Compile Using Ubuntu WSL.


Normal: g++ [filename.cpp] -o [filename]


Kernel: nvc++ -acc=host Kernel.cpp -o Kernel


GPU: nvc++ -acc=gpu -gpu=cc86 GPU.cpp -o Gpu
