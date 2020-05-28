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
	int start, end;
	int id;
	int x_resolution, y_resolution, max_iter, remain;
	double view_x0, view_x1, view_y0, view_y1, x_stepsize, y_stepsize;
	int palette_shift;
	void *img;
};

// function to be executed by the thread
void *kernel(void *args)
{
	struct pthread_args *arg = (struct pthread_args *)args;
	unsigned char(*img)[arg->x_resolution][3] = (unsigned char(*)[arg->x_resolution][3])arg->img;

	// printf("id = %d\n", arg->id);
	int start = arg->start;
	int end = arg->end;
	int max_iter = arg->max_iter;
	int palette_shift = arg->palette_shift;
	int x_resolution = arg->x_resolution;
	int y_resolution = arg->y_resolution;
	double view_x0 = arg->view_x0;
	double view_x1 = arg->view_x1;
	double view_y0 = arg->view_y0;
	double view_y1 = arg->view_y1;
	double x_stepsize = arg-> x_stepsize;
	double y_stepsize = arg-> y_stepsize;

	double x;
	double y;
	complex double Z;
	complex double C;

	int k;
	int z;
	// printf("id = %d\n", arg->id);
	// printf("Thread: x_resolution = %d\n", arg->x_resolution);
	for (int i = start; i < end; i++)
	{
		// printf("i = %d, id = %d\n", i, arg->id);
		for (int j = 0; j < x_resolution; j++)
		{
			
			y = view_y1 - i * y_stepsize;
			x = view_x0 + j * x_stepsize;

			Z = 0 + 0 * I;
			C = x + y * I;

			k = 0;

			do
			{
				Z = Z * Z + C;
				k++;
			} while (cabs(Z) < 2 && k < max_iter);
			// printf("id = %d, j = %d\n",arg->id, j);
			if (k == max_iter)
			{
				// printf("id = %d, j = %d\n",arg->id, j);	
				memcpy(img[i][j], "\0\0\0", 3);
			}
			else
			{
				// printf("id = %d, j = %d\n ,else", arg->id, j);
				int index = (k + palette_shift) % (sizeof(colors) / sizeof(colors[0]));
				// printf("j = %d\n",j);
				memcpy(img[i][j], colors[index], 3);
			}
		}
	}
	return NULL;
}


void mandelbrot_draw(int x_resolution, int y_resolution, int max_iter,
	                double view_x0, double view_x1, double view_y0, double view_y1,
	                double x_stepsize, double y_stepsize,
	                int palette_shift, unsigned char (*img)[x_resolution][3],
						 int num_threads) {
	// TODO:
	// implement your solution in this file.

	//1. Allocate the threads
	pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
	struct pthread_args *args = (struct pthread_args *)malloc(num_threads * sizeof(struct pthread_args));
	int i, taskSize = y_resolution / num_threads;

	//create threads, start executing kernel in parallel
	for ( i = 0; i < num_threads; ++i)
	{
		//set the input in args
		// printf("i = %d\n",i);
		args[i].start = i * taskSize;
		//If  (i < (num_threads - 1)), args[i].end = (i + 1) * taskSize,
		//Else, args[i].end = ( i + 1) * taskSize + (y_resolution % num_threads)
		args[i].end = (i < (num_threads - 1)) ? (i + 1) * taskSize : (i + 1) * taskSize + (y_resolution % num_threads);
		args[i].x_resolution = x_resolution;
		args[i].max_iter = max_iter;
		args[i].view_x0 = view_x0;
		args[i].view_x1 = view_x1;
		args[i].view_y0 = view_y0;
		args[i].view_y1 = view_y1;
		args[i].x_stepsize = x_stepsize;
		args[i].y_stepsize = y_stepsize;
		args[i].palette_shift = palette_shift;
		args[i].img = img;
		pthread_create(&threads[i], NULL, kernel, args+i);
	}

	//wait for all the threads to finish executing kernel
	for ( i = 0; i < num_threads; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(args);
}
