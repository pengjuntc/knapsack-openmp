#knapsack-openmp

Basically, total[i][j] records the maximum value for i items (from 1 to i) with maximum capacity j.
```
Recurrence Relation:
  (index i -- ith item; index j -- capacity j)
  1. total[i][j] = total[i-1][j] if weight[i] > j or total[i-1][j] >= total[i-1][j-weight[i]] + profit[i]
  2. total[i][j] = total[i-1][j-weight[i]] + profit[i]    otherwise
```

So we can parallelly compute row i. Notice that when we compute ith row, we have already computed all rows before row i. When we calculate total[i][j], we don't need any other entries in ith row. So there is no race condition.


###Test Instruction
1. run command: make
   it will generate two executables knap and knap_para
2. test sequential version: ./knap 10000 10000
   generally the command pattern is ./knap n c,
   n is the number of items, c is the maximum capcity
3. test parallel version: ./knap_para 10000 10000
                       or ./knap_para 10000 10000 6
                       or ./knap_para 10000 10000 6 50
   generally the command pattern is ./knap n c [num_threads] [max_weight_and_value] ,
   the default number of threads is 6,
   the default maximum weights and values is 50.


The program has been tested on freightliner, which its CPU has 8 processors. 
