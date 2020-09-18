#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define DBJ_API static
// #define __SSE__

/**********************************
 Pseudo-random number generator
 */

DBJ_API uint64_t mat_rng[2] = { 11ULL, 1181783497276652981ULL };

DBJ_API inline uint64_t xorshift128plus(uint64_t s[2])
{
	uint64_t x, y;
	x = s[0], y = s[1];
	s[0] = y;
	x ^= x << 23;
	s[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
	y += s[1];
	return y;
}

DBJ_API double mat_drand(void)
{
	return (xorshift128plus(mat_rng) >> 11) * (1.0 / 9007199254740992.0);
}

/*
 Helper routines for matrix manipulation

 */

#ifdef  __clang__
/*
actually this works in any compiler supporting the VLA
but notice we actually do use VLA just to declare the structure we want
no usual VLA stack use 
*/
DBJ_API float** mat_init(int n_rows, int n_cols)
{
	// single ste no reqwiring matrix creation
	// we could have used malloc but calloc zeros the mem allocated
	// and contrary to popular belief it might be faster than malloc
	return  (float**)calloc(1, sizeof(float[n_rows][n_cols]) );
}
#else // ! __clang__
/*
 Now next is perhaps a very intweresting matrix allocation to study
 (see: https://en.wikipedia.org/wiki/Row-_and_column-major_order )
 but also slow: two step allocation + rewiring algorithm
*/
DBJ_API float** mat_init(int n_rows, int n_cols)
{
	float** m;
	int i;
	/* allocate array of pointers to rows first */
	m = (float**)malloc(n_rows * sizeof(float*));
	/* allocate height * width, 1D array of required data type */
	m[0] = (float*)calloc(n_rows * n_cols, sizeof(float));
	/* now re-wire the first array pointers to simulate the 2D layout */
	for (i = 1; i < n_rows; ++i) {
		/* re-wire to point to the beginning of the row */
		m[i] = m[i - 1] + n_cols;
	}
	return m;
}
#endif // ! __ clang_

DBJ_API void mat_destroy(float** m)
{
	free(m[0]); free(m);
}

/*
allocate the matrix and populate it with random float's
*/
DBJ_API float** mat_gen_random(int n_rows, int n_cols)
{
	float** m = NULL ;
	int i, j;
	m = mat_init(n_rows, n_cols);
	for (i = 0; i < n_rows; ++i)
		for (j = 0; j < n_cols; ++j)
			m[i][j] = (float)mat_drand() ; // added cast
	return m;
}

DBJ_API float** mat_transpose(int n_rows, int n_cols, float* const* a)
{
	int i, j;
	float** m;
	m = mat_init(n_cols, n_rows);
	for (i = 0; i < n_rows; ++i)
		for (j = 0; j < n_cols; ++j)
			m[j][i] = a[i][j];
	return m;
}

DBJ_API float sdot_1(int n, const float* x, const float* y)
{
	int i;
	float s = 0.0f;
	for (i = 0; i < n; ++i) s += x[i] * y[i];
	return s;
}

DBJ_API float sdot_8(int n, const float* x, const float* y)
{
	int i, n8 = n >> 3 << 3;
	float s, t[8];
	t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = 0.0f;
	for (i = 0; i < n8; i += 8) {
		t[0] += x[i + 0] * y[i + 0];
		t[1] += x[i + 1] * y[i + 1];
		t[2] += x[i + 2] * y[i + 2];
		t[3] += x[i + 3] * y[i + 3];
		t[4] += x[i + 4] * y[i + 4];
		t[5] += x[i + 5] * y[i + 5];
		t[6] += x[i + 6] * y[i + 6];
		t[7] += x[i + 7] * y[i + 7];
	}
	for (s = 0.0f; i < n; ++i) s += x[i] * y[i];
	s += t[0] + t[1] + t[2] + t[3] + t[4] + t[5] + t[6] + t[7];
	return s;
}

#ifdef __SSE__
#include <intrin.h>

DBJ_API float sdot_sse(int n, const float* x, const float* y)
{
	int i, n8 = n >> 3 << 3;
	__m128 vs1, vs2;
	float s, t[4];
	vs1 = _mm_setzero_ps();
	vs2 = _mm_setzero_ps();
	for (i = 0; i < n8; i += 8) {
		__m128 vx1, vx2, vy1, vy2;
		vx1 = _mm_loadu_ps(&x[i]);
		vx2 = _mm_loadu_ps(&x[i + 4]);
		vy1 = _mm_loadu_ps(&y[i]);
		vy2 = _mm_loadu_ps(&y[i + 4]);
		vs1 = _mm_add_ps(vs1, _mm_mul_ps(vx1, vy1));
		vs2 = _mm_add_ps(vs2, _mm_mul_ps(vx2, vy2));
	}
	for (s = 0.0f; i < n; ++i) s += x[i] * y[i];
	_mm_storeu_ps(t, vs1);
	s += t[0] + t[1] + t[2] + t[3];
	_mm_storeu_ps(t, vs2);
	s += t[0] + t[1] + t[2] + t[3];
	return s;
}
#endif

/**************************************************
 
 Various Matrix multiplication algorithms

 NOTE: check and choose for your RT Env
       there are differences but are *very* dependant on 
	   compiler, OS and hardware

 */

DBJ_API float** mat_mul0(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, k;
	float** m;
	m = mat_init(n_a_rows, n_b_cols);
	for (i = 0; i < n_a_rows; ++i) {
		for (j = 0; j < n_b_cols; ++j) {
			float t = 0.0;
			for (k = 0; k < n_a_cols; ++k)
				t += a[i][k] * b[k][j];
			m[i][j] = t;
		}
	}
	return m;
}

DBJ_API float** mat_mul1(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, k, n_b_rows = n_a_cols;
	float** m, ** bT;
	m = mat_init(n_a_rows, n_b_cols);
	bT = mat_transpose(n_b_rows, n_b_cols, b);
	for (i = 0; i < n_a_rows; ++i) {
		const float* ai = a[i];
		float* mi = m[i];
		for (j = 0; j < n_b_cols; ++j) {
			float t = 0.0f, * bTj = bT[j];
			for (k = 0; k < n_a_cols; ++k)
				t += ai[k] * bTj[k];
			mi[j] = t;
		}
	}
	mat_destroy(bT);
	return m;
}

#ifdef __SSE__
DBJ_API float** mat_mul2(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, n_b_rows = n_a_cols;
	float** m, ** bT;
	m = mat_init(n_a_rows, n_b_cols);
	bT = mat_transpose(n_b_rows, n_b_cols, b);
	for (i = 0; i < n_a_rows; ++i)
		for (j = 0; j < n_b_cols; ++j)
			m[i][j] = sdot_sse(n_a_cols, a[i], bT[j]);
	mat_destroy(bT);
	return m;
}
DBJ_API float** mat_mul7(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, ii, jj, x = 16, n_b_rows = n_a_cols;
	float** m, ** bT;
	m = mat_init(n_a_rows, n_b_cols);
	bT = mat_transpose(n_b_rows, n_b_cols, b);
	for (i = 0; i < n_a_rows; i += x) {
		for (j = 0; j < n_b_cols; j += x) {
			int je = n_b_cols < j + x ? n_b_cols : j + x;
			int ie = n_a_rows < i + x ? n_a_rows : i + x;
			for (ii = i; ii < ie; ++ii)
				for (jj = j; jj < je; ++jj)
					m[ii][jj] += sdot_sse(n_a_cols, a[ii], bT[jj]);
		}
	}
	mat_destroy(bT);
	return m;
}
#endif

DBJ_API float** mat_mul3(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, n_b_rows = n_a_cols;
	float** m, ** bT;
	m = mat_init(n_a_rows, n_b_cols);
	bT = mat_transpose(n_b_rows, n_b_cols, b);
	for (i = 0; i < n_a_rows; ++i)
		for (j = 0; j < n_b_cols; ++j)
			m[i][j] = sdot_8(n_a_cols, a[i], bT[j]);
	mat_destroy(bT);
	return m;
}

DBJ_API float** mat_mul4(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, n_b_rows = n_a_cols;
	float** m, ** bT;
	m = mat_init(n_a_rows, n_b_cols);
	bT = mat_transpose(n_b_rows, n_b_cols, b);
	for (i = 0; i < n_a_rows; ++i)
		for (j = 0; j < n_b_cols; ++j)
			m[i][j] = sdot_1(n_a_cols, a[i], bT[j]);
	mat_destroy(bT);
	return m;
}

#ifdef HAVE_CBLAS
#include <cblas.h>

DBJ_API float** mat_mul5(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	int i, j, n_b_rows = n_a_cols;
	float** m, ** bT;
	m = mat_init(n_a_rows, n_b_cols);
	bT = mat_transpose(n_b_rows, n_b_cols, b);
	for (i = 0; i < n_a_rows; ++i)
		for (j = 0; j < n_b_cols; ++j)
			m[i][j] = cblas_sdot(n_a_cols, a[i], 1, bT[j], 1);
	mat_destroy(bT);
	return m;
}

DBJ_API** mat_mul6(int n_a_rows, int n_a_cols, float* const* a, int n_b_cols, float* const* b)
{
	float** m, n_b_rows = n_a_cols;
	m = mat_init(n_a_rows, n_b_cols);
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n_a_rows, n_b_cols, n_a_cols, 1.0f, a[0], n_a_rows, b[0], n_b_rows, 0.0f, m[0], n_a_rows);
	return m;
}
#endif

/**********************************************************************************/

#include "../getopt_win32/getopt.h"
#include <time.h>

enum { default_multiplication_algorithm = 4, max_matrix_side = 0xFF + 0xFF } ;

/*
These are square matrices, matrix_side_length is the length of the side of that square
matrix data type is pointer to pointer
matrix element type is float
*/
DBJ_API int test_matmul(unsigned matrix_side_length, unsigned algorithm_id, const char * algo_name  )
{
	int algo = algorithm_id;
	
	float** matrix_A = NULL, ** matrix_B = NULL , ** matrix_M = NULL ;

	// quietly adjusting to max side .. not exactly good but ok in this context
	matrix_side_length = matrix_side_length % (size_t)max_matrix_side;

	matrix_A = mat_gen_random(matrix_side_length, matrix_side_length);
	matrix_B = mat_gen_random(matrix_side_length, matrix_side_length);

	clock_t start_time_ = clock();

	if (algo == 0) {
		matrix_M = mat_mul0(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
	}
	else if (algo == 1) {
		matrix_M = mat_mul1(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
#ifdef __SSE__
	}
	else if (algo == 2) {
		matrix_M = mat_mul2(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
	}
	else if (algo == 7) {
		matrix_M = mat_mul7(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
#endif
	}
	else if (algo == 3) {
		matrix_M = mat_mul3(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
	}
	else if (algo == 4) {
		matrix_M = mat_mul4(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
#ifdef HAVE_CBLAS
	}
	else if (algo == 5) {
		matrix_M = mat_mul5(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
	}
	else if (algo == 6) {
		matrix_M = mat_mul6(matrix_side_length, matrix_side_length, matrix_A, matrix_side_length, matrix_B);
#endif
	}
	else {
		fprintf(stderr, "SKIPPING: unknown algorithm %d\rows_cols", algo);
		goto matmul_exit;
	}

	fprintf(stderr, "\nAlgorithm %-36s: ", algo_name );
	fprintf(stderr, "CPU time: %2.3g sec", (double)(clock() - start_time_) / CLOCKS_PER_SEC);
	// fprintf(stderr, " Central cell: %g", matrix_M[rows_cols / 2][rows_cols / 2]);

	matmul_exit:
	if (matrix_M) mat_destroy(matrix_M);
	if (matrix_A) mat_destroy(matrix_A);
	if (matrix_B) mat_destroy(matrix_B);
	return EXIT_SUCCESS ;
}

static const char* algo_name_ [] = {
"0: naive - no optimization" ,
"1: transposing the second matrix" ,
"2: explicitly vectorized sdot() with SSE" ,
"3: implicitly vectorized sdot()" ,
"4: no vectorization hints",
"5: with sdot() from an external CBLAS library" ,
"6: with sgemm() from an external CBLAS library",
"7: explicitly SSE sdot() plus loop tiling" ,
"Unknown"
};

void test_various_matmuls()
{
	const unsigned mx_size = 0xFF;
	const unsigned outer_loop_ = 0xF;
	fprintf(stderr, "\n------------------------------------------------");
	fprintf(stderr, "\nMatrix size: %4d", mx_size);


	for (unsigned k = 0; k < outer_loop_; ++k)
	{
		fprintf(stderr, "\n------------------------------------------------");
		test_matmul(mx_size, 0, algo_name_[0]);
		test_matmul(mx_size, 1, algo_name_[1]);
#ifdef __SSE__
		test_matmul(mx_size, 2, algo_name_[2]);
		test_matmul(mx_size, 7, algo_name_[7]);
#endif
		test_matmul(mx_size, 3, algo_name_[3]);
		test_matmul(mx_size, 4, algo_name_[4]);
#ifdef HAVE_CBLAS
		test_matmul(mx_size, 5, algo_name_[5]);
		test_matmul(mx_size, 6, algo_name_[6]);
#endif 
	}

}


