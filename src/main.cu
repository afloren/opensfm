#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <conio.h>
#include "Timer.h"

__global__ void ProjectPoints(float* P, float3* X, float2* xh)
{
   int i = threadIdx.x;

   __shared__ float Ps[12];

   if(i<12)
   {
	   Ps[i] = P[i];
   }

   __syncthreads();

   //float x = Ps[0]*X[3*i]+Ps[1]*X[3*i+1]+Ps[2]*X[3*i+2]+Ps[3];
   //float y = Ps[4]*X[3*i]+Ps[5]*X[3*i+1]+Ps[6]*X[3*i+2]+Ps[7];
   //float z = Ps[8]*X[3*i]+Ps[9]*X[3*i+1]+Ps[10]*X[3*i+2]+Ps[11];
   //xh[2*i] = x/z;
   //xh[2*i+1] = y/z;

   float x = Ps[0]*X[i].x + Ps[1]*X[i].y + Ps[2]*X[i].z + Ps[3];
   float y = Ps[4]*X[i].x + Ps[5]*X[i].y + Ps[6]*X[i].z + Ps[7];
   float z = Ps[8]*X[i].x + Ps[9]*X[i].y + Ps[10]*X[i].z + Ps[11];

   xh[i].x = x/z;
   xh[i].y = y/z;
}

__global__ void ReprojectionError(float* P, float* X, float* x, float *r)
{
   int i = threadIdx.x;
   float xhx = P[0]*X[3*i]+P[1]*X[3*i+1]+P[2]*X[3*i+2]+P[3];
   float xhy = P[4]*X[3*i]+P[5]*X[3*i+1]+P[6]*X[3*i+2]+P[7];
   float xhz = P[8]*X[3*i]+P[9]*X[3*i+1]+P[10]*X[3*i+2]+P[11];
   r[2*i] = x[2*i]-xhx/xhz;
   r[2*i+1] = x[2*i+1]-xhy/xhz;
}

void printVector(float* A, int size)
{
   std::cout << "[ ";
   for(int i=0;i<size;i++)
   {
      std::cout << A[i] << " ";
   }


   std::cout << "]" << std::endl;
}

void randomVector(float* A, int size)
{
   for(int i=0;i<size;i++)
   {
      A[i] = rand()/(float)RAND_MAX;
   }
}

int main()
{
   Timer timer;
   const int N = 512;
   float P[12] = {1,0,0,0,0,1,0,0,0,0,1,0};
   float Q[N*3];
   float q[N*2];
    
   //printVector(P,12);

   randomVector(Q,N*3);
   //printVector(Q,N*3);

   float* dP;
   cudaMalloc(&dP,sizeof(float)*12);
   float3* dQ;
   cudaMalloc(&dQ,sizeof(float3)*N);
   float2* dq;
   cudaMalloc(&dq,sizeof(float2)*N);

   timer.start();
   
   cudaMemcpy(dP,P,sizeof(float)*12,cudaMemcpyHostToDevice);  
   cudaMemcpy(dQ,Q,sizeof(float3)*N,cudaMemcpyHostToDevice);

   std::cout << "Elapsed time(s): " << timer.getElapsed() << std::endl;

   ProjectPoints<<<1,N>>>(dP,dQ,dq);

   std::cout << "Elapsed time(s): " << timer.getElapsed() << std::endl;

   cudaMemcpy(q,dq,sizeof(float2)*N,cudaMemcpyDeviceToHost);

   std::cout << "Elapsed time(s): " << timer.getElapsed() << std::endl;

   //printVector(q,N*2);

   getch();
}


