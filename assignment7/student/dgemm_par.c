#include "dgemm.h"
#include <immintrin.h>
#include <inttypes.h>


void dgemm(float *a, float *b, float *c, int n)
{
    // TODO: insert your solution here
    int vec_length = 8;
    int vec_end = n - (n % vec_length);
    float sum;
    __m256 A, B, sum_v, mul_v;

    for (int i = 0; i < n; i++) // row index
    {
        for (int j = 0; j < n; j++) //column index
        {
            sum = 0;
            sum_v = _mm256_setzero_ps();
            for(int k = 0; k < vec_end; k+=vec_length)
            {
                A = _mm256_loadu_ps(a + i * n + k);
                B = _mm256_loadu_ps(b + j * n + k);
                mul_v = _mm256_mul_ps(A, B);
                sum_v = _mm256_add_ps(sum_v, mul_v);
                // _mm256_storeu_ps(c[i], mul_v);
            }
            for( int l = 0; l < vec_length; l++)
            {
                sum += sum_v[l];
            }
            c[ i * n + j ] = sum;
        }
            
    }

}
