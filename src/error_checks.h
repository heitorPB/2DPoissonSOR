/**
 * @file
 *
 * This header provides two helper macros for error checking.
 *
 * File modified from CUDA samples/common/inc/helper_cuda.h
 */

#ifndef CUDA_ERROR_UTIL_H_
#define CUDA_ERROR_UTIL_H_


#include <stdio.h>
#include <stdlib.h>


/** @brief wrap CUDA API calls with this macro to check for errors.
 *
 * Usage example:
 *
 * 	CUDA_CHECK(cudaMemcpy((void*) f, (void*) f_gpu, size, cudaMemcpyDeviceToHost));
 *
 * Example of output to stderr:
 *
 * 	CUDA error at PoissonSOR2D_CUDA.c:61 code=77 "cudaMemcpy((void*) f, (void*) f_gpu, size, cudaMemcpyDeviceToHost)" 
 */
#define CUDA_CHECK(errarg)   __checkErrorFunc((errarg), #errarg, __FILE__, __LINE__)


/** @brief wrapper for cudaGetLastError() with a custom message
 *
 * Usage example:
 *
 * 	update_CUDA<<<grid, block>>>(NULL, f_tmp, gamma, N);
 * 	cudaThreadSynchronize();
 * 	CHECK_ERROR_MSG("Kernel call");
 *
 * Example of output to stderr:
 *
 * 	PoissonSOR2D_CUDA.c(61) : cudaGetLastError() : Kernel call : (77) an illegal memory access was encountered.
 */
#define CHECK_ERROR_MSG(errstr) __checkErrMsgFunc(errstr, __FILE__, __LINE__)


/** @cond */
inline void __checkErrorFunc(cudaError_t errarg,
                             char const *const func,
                             const char* file,
                             const int line)
{
	if(errarg) {
		fprintf(stderr, "CUDA error at %s:%d code=%d \"%s\" \n",
                file, line, errarg, func);
		exit(EXIT_FAILURE);
	}
}


inline void __checkErrMsgFunc(const char* errstr,
                              const char* file,
                              const int line)
{
	cudaError_t err = cudaGetLastError();
	if(err != cudaSuccess) {
		fprintf(stderr,
		        "%s(%i) : cudaGetLastError() : %s : (%d) %s.\n",
		        file, line, errstr, (int)err, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

/** @endcond */

#endif
