#pragma once
#include <cuda_runtime.h>
__global__ void matmul_basics(const float* a,const float* b,float* c,int m,int n,int k_size);
