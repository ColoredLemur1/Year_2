// Kernel for matrix transposition.
// Input is row-major nRows x nCols; output is row-major nCols x nRows (the transpose).
__kernel void transpose( __global const float *in, __global float *out, int nRows, int nCols )
{
	int outRow = get_global_id( 0 );
	int outCol = get_global_id( 1 );
	if( outRow >= nCols || outCol >= nRows )
		return;
	out[outRow * nRows + outCol] = in[outCol * nCols + outRow];
}
