#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>
#include"helper.h"
#include"life.h"
#include"gui.h"

void test_grid(int height,int grid[height][height],int grid_par[height][height]){
    printf("test grid\n");

    for(int i=0;i<height;i++){
        for(int j=0;j<height;j++){
            if(grid_par[i][j]!=grid[i][j]){
                printf("(%d,%d):%d",i,j,grid[i][j]);
                // printf("(%d,%d) ",i,j);
            }
        }
    }
    printf("\n");
}
void test_chunk(int height,int chunk, int rank, int grid[height][height],int tmp_old[chunk][chunk]){
    printf("test chunk:%d\n",rank);
    //top
    int row = rank/4;
    int col = rank%4;
    int range = chunk-2;
    int boundry_r = (row!=3)?chunk:chunk-2;
    int boundry_c = (col!=3)?chunk:chunk-2;
    if(rank<4){         //top
        for(int i=1;i<boundry_c-1;i++){
            if(tmp_old[0][i]!=grid[0][col * range + i])
                printf("(%d,%d)=%d  ",0,i,tmp_old[0][i]);
        }
    }
    else if(rank>11){   //down
        printf("\n");
        for(int i=1;i<boundry_c-1;i++){
            if(tmp_old[boundry_r-1][i]!=grid[row*range+boundry_r-1][col * range + i])
                printf("(%d,%d)=%d ",boundry_r-1,i,tmp_old[boundry_r-1][i]);
        }
        printf("\n");
    }
    if(rank%4==3){      //right
        for(int i=1;i<boundry_r-1;i++){
            if(tmp_old[i][boundry_c-1]!=grid[row * range + i][col*range+boundry_c-1])
                printf("(%d,%d)=%d  ",i,boundry_c-1,tmp_old[i][boundry_c-1]);
        }
    }
    else if(rank%4==0){ //left
        printf("\n");
        for(int i=0;i<boundry_r-1;i++){
            if(tmp_old[i][0]!=grid[row*range+i][0])
                printf("(%d,%d)=%d ",i,0,tmp_old[i][0]);
        }
    }
    //inside
    for(int i=1;i<boundry_r-1;i++){
        for(int j=1;j<boundry_c-1;j++){
            if(tmp_old[i][j]!=grid[row*range+i][col*range+j]){
                printf("(%d,%d):%d ",i,j,tmp_old[i][j]);
            }
        }
    }
    printf("\n");

    //corner check
    printf("corner check wrong:");
    if(rank==0 && grid[0][0]!=tmp_old[0][0])printf("1 ");
    else if(rank==3 && grid[0][height-1]!=tmp_old[0][boundry_c-1])printf("2 ");
    else if(rank==12 && grid[height-1][0]!=tmp_old[boundry_r-1][0])printf("3 ");
    else if(rank==15 && grid[height-1][height-1]!=tmp_old[boundry_r-1][boundry_c-1])printf("4 ");
    printf(" all check finish\n");

    // if(rank==15)printf("corner test in grid: 15 %d",tmp_old[boundry_r-1][boundry_c-1]);


}
