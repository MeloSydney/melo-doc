

// TODO - grid block thread


// TODO - 修饰符
__global__
	类似main

__device__


// TODO - malloc

cudaMalloc()
cudaMallocManaged()
cudaFree()


// TODO - 内存类型

Global
	__device__ int GlobalVar;
	grid

Texture

Constant
	[__device__] __constant__ int ConstantVar;
	grid

Shared
	[__device__] __shared__ int SharedVar;
	block

Local
	int LocalArray[10];
	thread

Register
	int LocalVar;
	thread