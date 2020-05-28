#include"dgemm.h"
#include <stdio.h>
#include <stdlib.h>

void dgemm(float *a, float *b, float *c, int n)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                c[i * n + j] += a[i * n  + k] * b[j * n  + k];
            }
        }
    }

    for (int j = 0; j < n; j += 1) // row index
    {

        for (int i = 0; i < n; i += 1) //column index
        {
            printf("%f ", c[i + j * n]);
        }
        printf("\n");
    }
}
