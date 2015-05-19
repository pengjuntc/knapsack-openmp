// This program is wriiten based on knap.c whic is provided by the instructor.
// Author: Jun Peng. 04/08/2015
// Implement parallel version for knapsack problem using OpenMP


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

void init_things(int n, int max, int weight[n], int profit[n]) 
{
	int i = 0;
	
	for (i = 0; i < n; i++)	{
		weight[i] = (random() % max) + 1;
		profit[i] = (random() % max) + 1;
	}
}

void print_things(int n, int weight[n], int profit[n]) 
{
	int i = 0;
	printf("Weights\n");	
	for (i = 0; i < n; i++)	{
	  printf("%2d ", weight[i]);
	}
	printf("\n");

	printf("Profits\n");	
	for (i = 0; i < n; i++)	{
	  printf("%2d ", profit[i]);
	}
	printf("\n");
}

void print_table(int n, int c, int total[n][c], int use[n][c])
{
  int i, j;

  printf("Total Profit\n");
  for (j = 0; j < c; j++) {
	for (i = 0; i < n; i++) {
	  printf("%2d ",total[i][j]);
	}
	printf("\n");
  }

  printf("Flags\n");
  for (j = 0; j < c; j++) {
	for (i = 0; i < n; i++) {
	  printf("%2d ",use[i][j]);
	}
	printf("\n");
  }
}


void solver(int n, int c, int weight[n], int profit[n], 
		   int total[n][c], int use[n][c], int nthreads)
{
  int i, j;
/* Max profit using thing 0, if it fits */
  for (j = 0; j < c; j++) {
	if (weight[0] > j) {
	  total[0][j] = 0;
	  use[0][j] = 0;
	} else {
	  total[0][j] = profit[0];
	  use[0][j] = 1;
	}
  }

  for (i = 1; i < n; i++) {
/*
  Here we want to compute a row in parallel.
  Recurrence Relation:
  (index i -- ith item; index j -- capacity j)
  1. total[i][j] = total[i-1][j] if weight[i] > j or total[i-1][j] >= total[i-1][j-weight[i]] + profit[i]
  2. total[i][j] = total[i-1][j-weight[i]] + profit[i]    otherwise

  So we can parallelly compute row i. Notice that when we compute ith row, we have already computed all rows before row i. When we calculate total[i][j], we don't need any other entry in ith row. So there is no race condition.
*/

# pragma omp parallel for num_threads(nthreads) 
	for (j = 0; j < c; j++) {
	  if ( (j<weight[i]) || 
		   (total[i-1][j] >= total[i-1][j-weight[i]] + profit[i])) {
		total[i][j] = total[i-1][j];
		use[i][j] = 0;
	  } else {
		total[i][j] = total[i-1][j-weight[i]] + profit[i];
		use[i][j] = 1;
	  }
	}
  }
}

int main(int argc, char *argv[]) {
        int i, n = 0, c = 0, m = 50, nthreads = 6; /* the default number of threads is 6 */
	int *weights, *profits;
	unsigned long long usec;
  	struct timeval tstart, tend;
	int (*table)[c], (*flags)[c];

	if (argc > 2 && argc < 6) {
		n = atoi(argv[1]);		/* Number of things */
		c = atoi(argv[2]);		/* Capacity of knapsack */
		if (argc == 4)
			nthreads = atoi(argv[3]);
		if (argc == 5) {
                        nthreads = atoi(argv[3]);
                        m = atoi(argv[4]);
                }
	}
	if ((n <= 0) || (c <= 0) || (m <= 0)) {
	  	fprintf(stdout, "usage %s n c - where n and c >= 0\n", argv[0]);
		exit(1);
	}
	
	weights = malloc(sizeof(int [n]));
	profits = malloc(sizeof(int [n]));
	init_things(n, m, weights, profits);
	//print_things(n, weights, profits);
	table = malloc(sizeof (int [n][c]));
	flags = malloc(sizeof (int [n][c]));
	gettimeofday(&tstart, NULL);
	solver(n, c, weights, profits, table, flags, nthreads);
	gettimeofday(&tend, NULL);

	if (tend.tv_usec > tstart.tv_usec) {
		usec = (tend.tv_sec - tstart.tv_sec) * 1000000
			+ tend.tv_usec - tstart.tv_usec;
	} else {
		usec = (tend.tv_sec - (tstart.tv_sec + 1)) * 1000000
			+ (1000000 + tend.tv_usec - tstart.tv_usec);
	}

	fprintf(stdout,
		"Solver finished in %f seconds.\n", (double)usec/1000000.0);
	//print_table(n,c,table,flags);
}
