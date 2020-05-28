#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "search.h"

void search_text(char* text, int num_lines, int line_length,
                 char* search_string, int* occurences) {
  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  int* sendcounts = (int*)malloc(num_procs * sizeof(int));
  int* displs = (int*)malloc(num_procs * sizeof(int));
  int num_lines_local = (int)(num_lines / num_procs);
  // The master distributes the chunks
  if (rank == 0) {
    for (int i = 0; i < num_procs; ++i) {
      sendcounts[i] = num_lines_local * line_length;
      displs[i] = num_lines_local * i * line_length;
    }
    int residual = num_lines - num_lines_local * num_procs;
    sendcounts[num_procs - 1] += residual;
  } else if (rank == (num_procs - 1)) {
    int residual = num_lines - num_lines_local * num_procs;
    num_lines_local += residual;
  }

  //
  int recvcount = num_lines_local * line_length;
  char* text_rcv = (char*)malloc(sizeof(char) * recvcount);

  // scatter the parts of text
  MPI_Scatterv(text, sendcounts, displs, MPI_CHAR, text_rcv, recvcount,
               MPI_CHAR, 0, MPI_COMM_WORLD);

  // now each processor solve its part of text
  int running_count = 0;
  for (int i = 0; i < num_lines_local; i++) {
    running_count +=
        count_occurences(text_rcv + i * line_length, search_string);
  }

  MPI_Reduce(&running_count, occurences, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
}
