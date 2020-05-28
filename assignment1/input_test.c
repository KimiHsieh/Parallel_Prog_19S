#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>
#include "mandelbrot_set.h"

// Arguments pass to pthre
struct pthread_args
{
    int id;
    int x_resolution, y_resolution, max_iter;
    double view_x0, view_x1, view_y0, view_y1, x_stepsize, y_stepsize;
    int palette_shift;
    int *A;
    // unsigned char(*img);
};

// function to be executed by the thread
void *kernel(void *args)
{
    struct pthread_args *arg = (struct pthread_args *)args;

    double y;
    double x;

    complex double Z;
    complex double C;

    int k;
    // printf("Thread ID = %d, y_resolution = %d, x_resolution = %d\n", arg->id, arg->y_resolution, arg->x_resolution);

    // *(arg->A) = *(arg->A)+ 5;   
    // return (void *)out;
    for (int i = 0; i < arg->y_resolution; i++)
    {
        for (int j = 0; j < arg->x_resolution; j++)
        {
            y = arg->view_y1 - i * arg->y_stepsize;
            x = arg->view_x0 + j * arg->x_stepsize;

            Z = 0 + 0 * I;
            C = x + y * I;

            k = 0;

            do
            {
                Z = Z * Z + C;
                k++;
            } while (cabs(Z) < 2 && k < arg->max_iter);

            if (k == arg->max_iter)
            {
                // memcpy(img[i][j], "\0\0\0", 3);
            }
            else
            {
                int index = (k + arg->palette_shift) % (sizeof(colors) / sizeof(colors[0]));
                // memcpy(img[i][j], colors[index], 3);
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int num_threads;
    int x_resolution, y_resolution, max_iter;
    // int *A = (int *)malloc(4 * sizeof(int));
    // double view_x0, view_x1, view_y0, view_y1, x_stepsize, y_stepsize;
    // int palette_shift, num_threads;
    // unsigned char(*img)[x_resolution][3];

    //0. initialization
    num_threads = 4;
    x_resolution = 1000;
    y_resolution = 1000;
    // A[0] = 100;
    // A[1] = 200;
    // A[2] = 300;
    // A[3] = 400;

    //1. Allocate the threads
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    struct pthread_args *args = (struct pthread_args *)malloc(num_threads * sizeof(struct pthread_args));
    // int *id = (int *)malloc(num_threads * sizeof(int));

    //create threads, start executing kernel in parallel
    for (int i = 0; i < num_threads; ++i)
    {
        //set the input in args
        // args[i].A = &A[i];
        args[i].id = i;
        args[i].x_resolution = x_resolution;
        args[i].y_resolution = (y_resolution / num_threads) * i;
        // args[i].max_iter = max_iter;
        // args[i].view_x0 = view_x0;
        // args[i].view_x1 = view_x1;
        // args[i].view_y0 = view_y0;
        // args[i].view_y1 = view_y1;
        // args[i].x_stepsize = x_stepsize;
        // args[i].y_stepsize = y_stepsize;
        // args[i].palette_shift = palette_shift;
        // args[i].img = img;
        pthread_create(&threads[i], NULL, kernel, args + i);
    }

    //wait for all the threads to finish executing kernel
    for (int i = 0; i < num_threads; ++i)
    {
        // int *out;
        // pthread_join(threads[i], (void *)&out);
        pthread_join(threads[i], NULL);
        // printf("A[%d] = %d, out = %d!\n", i, A[i], *out);
        // printf("A[%d] = %d\n", i, A[i]);
        // free(out);
    }
}
