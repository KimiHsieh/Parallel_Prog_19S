#include <stdio.h> 
#include <stdlib.h> 
  
int main() 
{ 
    int r = 3, c = 4, i, j, count; 
  
    int **arr = (int **)malloc(r * sizeof(int *)); 
    for (i=0; i<r; i++) 
         arr[i] = (int *)malloc(c * sizeof(int)); 
  
    // Note that arr[i][j] is same as *(*(arr+i)+j) 
    count = 0; 
    for (i = 0; i <  r; i++) 
      for (j = 0; j < c; j++) 
         arr[i][j] = ++count;  // OR *(*(arr+i)+j) = ++count 
  
    // for (i = 0; i <  r; i++) 
    //   for (j = 0; j < c; j++) 
        //  printf("%d ", arr[i][j]); 
    
    // printf("arr = %p\n", arr);
    // printf("*arr = %p\n", *arr);
    // printf("arr[0] = %p\n", arr[0]);
    // printf("&arr[0] = %p\n", &arr[0]);
    // printf("&arr[0][0] = %p\n", &arr[0][0]);
    // printf("arr+1 = %p\n", (arr + 1));
    // printf("arr[1] = %p\n", &arr[1]);


    int A[2][2];
    A[0][0] = 1;
    A[0][1] = 2;
    A[1][0] = 3;
    A[1][1] = 4;

    int (*image)[4][3]; // A pointer to an 2D array of integers
    image = malloc( 4 * 4 * sizeof(int[3]));

    printf("image = %p\n", image); 
    printf("*image = %p\n", *image); 
    printf("**image = %p\n", **image); 
    printf("***image = %d\n", ***image); 

    // unsigned char(*img)[100][3];
    // img = malloc(100* 100 * sizeof(char[3]));
    // printf("image[0][3] %c\n",image[0][3]);

    // printf("image[0][0] = %p\n", image[0][0]);

        /* Code for further processing and free the  
    dynamically allocated memory */

        return 0;
} 