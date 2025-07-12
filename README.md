How To Compile Using Ubuntu WSL. <br>
Normal: g++ [filename.cpp] -o [filename] <br>
Kernel: nvc++ -acc=host Kernel.cpp -o Kernel <br>
GPU: nvc++ -acc=gpu -gpu=cc86 GPU.cpp -o Gpu <br>
