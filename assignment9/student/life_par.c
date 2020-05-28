#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>
#include"helper.h"
#include"life.h"
#include"gui.h"
// #include"test.c"

void rearrange(int chunk, int tmp_old[chunk][chunk], int rank,int height, int grid_par[height][height]){
    int row = rank/4;
    int col = rank%4;
    int range = chunk-2;
    int boundry_r = (row!=3)?chunk:chunk-2;
    int boundry_c = (col!=3)?chunk:chunk-2;
    for(int i=0; i<boundry_r; i++){
        for(int j=0; j<boundry_c; j++){
            tmp_old[i][j] = grid_par[row * range + i][col * range + j];
            // if(tmp_old[i][j]!=)
        }
    }
}
void switch_ptr(int** a, int** b){
    int* tmp = *a;
    *a = *b;
    *b = tmp;
}
void evolve_par(int chunk, int rank, int tmp_old[chunk][chunk], int tmp_new[chunk][chunk]){

    MPI_Request r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16;
    MPI_Status status;
    int size = 16;
    int top[chunk], down[chunk], left[chunk], right[chunk], corner[4];
    int top_new[chunk], down_new[chunk], left_new[chunk], right_new[chunk], corner_new[4];

    int boundry_r = (rank<12)?chunk:chunk-2;
    int boundry_c = ((rank%4)!=3)?chunk:chunk-2;
    // printf("calculate sum:%d",rank);
    for(int i = 1; i < boundry_r-1; i++){
        for(int j = 1; j < boundry_c-1; j++){
            int sum = tmp_old[i-1][j-1]  + tmp_old[i-1][j] + tmp_old[i-1][j+1] + \
                tmp_old[i][j-1] + tmp_old[i][j+1] + \
                tmp_old[i+1][j-1] + tmp_old[i+1][j] + tmp_old[i+1][j+1];

            if(tmp_old[i][j] == 0){  //dead
                if(sum == 3)// reproduction
                    tmp_new[i][j] = 1;
                else
                    tmp_new[i][j] = 0;
            }
            else{ //alive
                if(sum == 2 || sum == 3)  //stay alive
                    tmp_new[i][j] = 1;
                else     // dies due to under or overpopulation
                    tmp_new[i][j] = 0;
            }
        }
    }
    //COPY FOUR BOUNDRIES
    // printf("copy four boundries:%d",rank);
    for(int i=0;i<boundry_r;i++){
        for(int j=0;j<boundry_c;j++){
            if(i==1)                top[j] = tmp_new[i][j];
            else if(i==boundry_r-2) down[j] = tmp_new[i][j];
            if(j==1)                left[i] = tmp_new[i][j];
            else if(j==boundry_c-2) right[i] = tmp_new[i][j];
        }
    }
    //COPY CORNERS
    corner[0] = tmp_new[1][1];
    corner[1] = tmp_new[1][boundry_c-2];
    corner[2] = tmp_new[boundry_r-2][1];
    corner[3] = tmp_new[boundry_r-2][boundry_c-2];

    //SEND DATA
    //top
    MPI_Irecv(&top_new,chunk,MPI_INT,rank<4?rank+12:rank-4 ,2,MPI_COMM_WORLD,&r1);
    MPI_Isend(&top,    chunk,MPI_INT,rank<4?rank+12:rank-4 ,1,MPI_COMM_WORLD,&r2);
    //down
    MPI_Irecv(&down_new,chunk,MPI_INT,rank<12?rank+4:rank-12 ,1,MPI_COMM_WORLD,&r3);
    MPI_Isend(&down,    chunk,MPI_INT,rank<12?rank+4:rank-12 ,2,MPI_COMM_WORLD,&r4);
    //left
    MPI_Irecv(&left_new,chunk,MPI_INT,rank%4?rank-1:rank+3 ,4,MPI_COMM_WORLD,&r5);
    MPI_Isend(&left,    chunk,MPI_INT,rank%4?rank-1:rank+3 ,3,MPI_COMM_WORLD,&r6);
    //right
    MPI_Irecv(&right_new,chunk,MPI_INT,(rank+1)%4?rank+1:rank-3 ,3,MPI_COMM_WORLD,&r7);
    MPI_Isend(&right,    chunk,MPI_INT,(rank+1)%4?rank+1:rank-3 ,4,MPI_COMM_WORLD,&r8);

    //Corner
    int lt,rt,ld,rd;
    if(rank<=3&&rank>0) lt= rank+11;            //1,2,3
    else if((rank%4)==0)lt = (rank+size-1)%size;//0,4,8,12
    else                lt = rank-5;            //5,6,7,9,10,11,13-15

    if(rank<3)          rt = rank+13;           //0,1,2
    else if(rank%4==3)  rt = (rank+size-7)%size;//3,7,11,15
    else                rt = rank-3;            //4,5,6,8,9,10,12-14

    if(!(rank%4))       ld = (rank+7)%size;     //0,4,8,12
    else if(rank>12)    ld = rank-13;           //13-15
    else                ld = rank+3;            //123,567,9,10,11

    if(rank>11&&rank<15)rd = rank-11;           //12-14
    else if(rank%4==3)  rd = (rank+1)%size;     //3,7,11,15
    else                rd = rank+5;            //012,456,8910

    MPI_Irecv(&corner_new[0], 1, MPI_INT, lt ,5,MPI_COMM_WORLD,&r9);
    MPI_Irecv(&corner_new[1], 1, MPI_INT, rt ,5,MPI_COMM_WORLD,&r10);
    MPI_Irecv(&corner_new[2], 1, MPI_INT, ld ,5,MPI_COMM_WORLD,&r11);
    MPI_Irecv(&corner_new[3], 1, MPI_INT, rd ,5,MPI_COMM_WORLD,&r12);
    MPI_Isend(&corner[0],     1, MPI_INT, lt ,5,MPI_COMM_WORLD,&r13);
    MPI_Isend(&corner[1],     1, MPI_INT, rt ,5,MPI_COMM_WORLD,&r14);
    MPI_Isend(&corner[2],     1, MPI_INT, ld ,5,MPI_COMM_WORLD,&r15);
    MPI_Isend(&corner[3],     1, MPI_INT, rd ,5,MPI_COMM_WORLD,&r16);
    //
    MPI_Wait(&r1, &status);
    MPI_Wait(&r2, &status);
    MPI_Wait(&r3, &status);
    MPI_Wait(&r4, &status);
    MPI_Wait(&r5, &status);
    MPI_Wait(&r6, &status);
    MPI_Wait(&r7, &status);
    MPI_Wait(&r8, &status);

    MPI_Wait(&r9, &status);
    MPI_Wait(&r10, &status);
    MPI_Wait(&r11, &status);
    MPI_Wait(&r12, &status);
    MPI_Wait(&r13, &status);
    MPI_Wait(&r14, &status);
    MPI_Wait(&r15, &status);
    MPI_Wait(&r16, &status);

    //Start combine
    for(int i=0;i<boundry_r;i++){
        for(int j=0;j<boundry_c;j++){
            if(i==0)                tmp_new[i][j] = top_new[j];
            else if(i==boundry_r-1) tmp_new[i][j] = down_new[j];
            if(j==0)                tmp_new[i][j] = left_new[i];
            else if(j==boundry_c-1) tmp_new[i][j] = right_new[i];
        }
    }

    tmp_new[0][0] = corner_new[0];
    tmp_new[0][boundry_c-1] = corner_new[1];
    tmp_new[boundry_r-1][0] = corner_new[2];
    tmp_new[boundry_r-1][boundry_c-1] =corner_new[3];

}

void simulate(int height, int width, int grid[height][width], int num_iterations)
{
    int rank,size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status s;

    if( size == 1){
		
		copy_edges(height, width, grid );
		for (int i = 0; i < num_iterations; i++)
		{
		  evolve( height, width, grid );
		  if (global_show_gui) gui_draw(height, width, grid[0]);
		}

    }else
    {
	    int range = height/4;
	    int chunk = range+2;    //chunk = 252


	    int (*tmp_old)[chunk] = NULL;
	    int (*tmp_new)[chunk] = NULL;

	    tmp_old = malloc(sizeof(int[chunk][chunk]));
	    tmp_new = malloc(sizeof(int[chunk][chunk]));

	    if(!rank){
	        copy_edges(height,width, grid);

	        for(int i=1;i<size;i++){
	            MPI_Send(grid,height*height,MPI_INT,i,0,MPI_COMM_WORLD);
	        }
	        rearrange(chunk, tmp_old, rank, height, grid);

	        for(int i=0;i<num_iterations;i++){
	            evolve_par(chunk, rank, tmp_old, tmp_new);
	            // evolve(height, width, grid);
	            switch_ptr((int**)tmp_old, (int**)tmp_new);
	            // test_chunk(height,chunk,rank,grid,tmp_old);
	        }
	        // test_chunk(height,chunk,rank,grid,tmp_old);

	        //Gather DATA process 0
	        for(int i=0;i<chunk;i++){
	            for(int j=0;j<chunk;j++)
	                grid[i][j] = tmp_old[i][j];
	        }

	        //Gather DATA otherp process
	        for(int k=1;k<size;k++){
	            MPI_Recv(tmp_new,chunk*chunk,MPI_INT,k,0,MPI_COMM_WORLD,&s);
	            // printf("receive successful %d\n",k);
	            // printf("test%d%d\n",tmp_new[0][0],tmp_new[chunk-1][chunk-1]);

	            int row = k/4;
	            int col = k%4;
	            int end_row = (k<12)?chunk:chunk-2;
	            int end_col = (k%4!=3)?chunk:chunk-2;
	            // printf("rank:%d,row:%d,col:%d,end_row:%d,end_col:%d\n",k,row,col,end_row,end_col);
	            for(int i=0;i<end_row;i++){
	                for(int j=0;j<end_col;j++){
	                    grid[row*range+i][col*range+j] = tmp_new[i][j];
	                }
	            }
	            //top
	            if(k<4){
	                for(int j=1;j<end_col-1;j++){
	                    grid[0][col*range+j] = tmp_new[0][j];
	                }
	            }
	            //down
	            else if(k>11){
	                for(int j=1;j<end_col-1;j++){
	                    grid[999][col*range+j] = tmp_new[end_row-1][j];
	                }
	            }
	            //left
	            if(k%4==0){
	                for(int i=1;i<end_row-1;i++){
	                    grid[row*range+i][0] = tmp_new[i][0];
	                }
	            }
	            //right
	            else if(k%4==3){
	                for(int i=1;i<end_row-1;i++){
	                    grid[row*range+i][999] = tmp_new[i][end_col-1];
	                }
	            }
	            //copy corner
	            if(k==3)grid[0][999] = tmp_new[0][end_col-1];
	            else if(k==12)grid[999][0] = tmp_new[end_row-1][0];
	            else if(k==15)grid[999][999] = tmp_new[end_row-1][end_col-1];

	        }

	        // test_grid(height,grid,grid_par);

	    }
	    else{
	        int (*grid_par)[height] = NULL;
	        grid_par= malloc( sizeof( int[height][height] ));
	        MPI_Recv(grid_par,height*height,MPI_INT,0,0,MPI_COMM_WORLD,&s);
	        rearrange(chunk, tmp_old, rank, height, grid_par);

	        for(int i=0;i<num_iterations;i++){
	            evolve_par(chunk, rank, tmp_old, tmp_new);
	            // evolve(height,height,grid_par);
	            switch_ptr((int**)tmp_old, (int**)tmp_new);
	            // test_chunk(height,chunk,rank,grid_par,tmp_old);
	        }
	        // test_chunk(height,chunk,rank,grid_par,tmp_old);

	        // //Gather Data
	        MPI_Send(tmp_old,chunk*chunk,MPI_INT,0,0,MPI_COMM_WORLD);
	        free(grid_par);
	    }

	    free(tmp_old);
	    free(tmp_new);
	}
}
