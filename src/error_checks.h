/**
 * @file
 *
 * This header provides two helper macros for error checking.
 * File modified from CUDA samples/common/inc/helper_cuda.h
 */

#ifndef CUDA_ERROR_UTIL_H_
#define CUDA_ERROR_UTIL_H_


#include <stdio.h>
#include <stdlib.h>


#define CUDA_CHECK(errarg)   __checkErrorFunc((errarg), #errarg, __FILE__, __LINE__)
#define CHECK_ERROR_MSG(errstr) __checkErrMsgFunc(errstr, __FILE__, __LINE__)


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
		fprintf(stderr, "Error: %s at %s(%i): %s\n",
				errstr, file, line, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

#endif
