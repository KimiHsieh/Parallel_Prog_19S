
void compute(double **a, double **b, double **c, double **d, int N, int num_threads) {

  /*TODO:
    Apply loop optimisations to the code below. Think about when certain optimisations are applicable and why.
    For example, when should you apply loop fusion? Should you do this for large problem sizes or small or both?
    Alternatively, does it make sense to break apart a heavy loop with a lot of computations? Why?
*/
  #pragma omp parallel for num_threads(num_threads)
  for (int i = 0; i < N; i++) {
    for (int j = 0 ; j < N-1; j++) {
            a[i][j] =  2 * c[i][j];
      d[i][j] = 2 * c[i][j+1] * b[i][j+1] + c[i][j] ;
    }
        a[i][N-1] = 2 * c[i][N-1];
        d[i][N-1] = 2 * c[i][N-1] * b[i][N-1];
  }


}
