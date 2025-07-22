## 🛠️ How to Compile Using Ubuntu WSL

### ✅ 1. Compile with Normal C++ Compiler (CPU Sequential Version)
```bash
g++ Normal.cpp -o Normal
```
### ✅ 2. Compile with OpenACC for Host (CPU Parallel Version)
```bash
nvc++ -acc=host Kernel.cpp -o Kernel
```
### ✅ 3. Compile with OpenACC for GPU Acceleration
```
nvc++ -acc=gpu -gpu=cc86 GPU.cpp -o Gpu
```
