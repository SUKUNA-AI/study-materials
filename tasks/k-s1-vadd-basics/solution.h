#pragma once
#include <cuda_runtime.h>
#include <cstddef>
__global__ void vadd_basics(const float* a,const float* b,float* c,std::size_t n);
