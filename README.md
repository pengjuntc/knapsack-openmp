The folder contains four files. 
(1) knap.c, which is the sequential code for knapsack problem, provided by the professor.
(2) knap_para.c, which is parallel version in OpenMP for knapsack problem.
(3) makefile, which compiles knap.c and knap_para.c, generating executable knap and knap_para respectively.
(4) README.txt, which is this file, explaining the parallel code and describing how to test the code.


**********explain the parallel code*********
Basically, total[i][j] records the maximum value for i items (from 1 to i) with maximum capacity j.
Recurrence Relation:
  (index i -- ith item; index j -- capacity j)
  1. total[i][j] = total[i-1][j] if weight[i] > j or total[i-1][j] >= total[i-1][j-weight[i]] + profit[i]
  2. total[i][j] = total[i-1][j-weight[i]] + profit[i]    otherwise

  So we can parallelly compute row i. Notice that when we compute ith row, we have already computed all rows before row i. When we calculate total[i][j], we don't need any other entry in ith row. So there is no race condition.

  I have checked the final result on small capacity and number of items, which is correct. 


**********general test instruction**********
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




****************test result*****************
The program is tested on freightliner, which its CPU has 8 processors. 
1. strong scaling
   command: ./knap 30000 30000
            ./knap_para 30000 30000
   
   The default number of threads in knap_para is 6.
   I have run each command 5 times. 
   The time for knap:     24.107918  24.247040  24.098447  24.104660  24.136015
   The time for knap_para: 7.081746   8.263385   7.415478   6.712311   6.846641

   So the average time for knap and knap_para are 24.14s and 7.26s respectively.
   The speedup is 24.14/7.26 = 3.33.
   The efficiency is 24.14/(7.26*6) = 55.4%.

2. weak scaling
   command: ./knap_para 20000 20000 2
   command: ./knap_para 30000 40000 6
   
   I have run each command 5 times. 
   The time for 2 processors: 8.203174  8.162956  8.304605  8.161813  8.162743
   The time for 6 processors: 8.539332  8.821148  8.724051  8.539023  8.486855

   So the average time for 2 and 6 processors are 8.20s and 8.62s respectively. 
   The isoefficiency is 8.20/8.62 = 95%.
