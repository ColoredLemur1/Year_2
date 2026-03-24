Complete the table below with your results, and then fill in the final two sections at the end.

Please do not edit or remove table or section headings, as the autograder script uses these to
locate the start and end of the table.

Each row of the table will be split into columns using the Python "split()" method. Therefore,
- fill in each column with values;
- only use whitespace between columns;
- do not add any non-numeric characters (which would cause the value to be read as zero);
- do not worry if your columns are not perfectly aligned.

For the parallel speed-up S, please note that:
- the time you should use is already output by the provided code;
- take as the serial execution time the time output by the code when run with a single process.
  Hence, the speed-up for 1 process in the table below must be 1.00.


No. Machines:   Total No. Processes:     Mean time (average of 3 runs) in seconds:        Parallel speed-up, S:
=============   ====================     =========================================        =====================
1                       1                             0.265135                                      1                 
1                       2                             0.201643                                      1.3149    
1                       4                             0.181933                                      1.4573                                            
1                       8                             0.140138                                      1.8920                           
2                       16                            1.190587                                      0.2227                                                     
2                       32                            1.201327                                      0.2207                                          

Please state the number of cores per machine (for Bragg 2.05, this is typically 12): Machine 1: 8 cores (Ryzen 7 9700x), Machine(s) 2 Multi-machine execution: 8 cores (i7-10700)

A brief interpretation of your results: On a single machine, the execution time decreased from 0.265 s (1 process) to 0.140 s (8 processes). 
This resulted in a speed up of approximately 1.89. While the time decreases as more processes are added, the speed up is not linear. 
This is because the parallel overhead of MPI collective routines like MPI_Scatter and MPI_Gather, which become more expensive as the number of processes increases. 
For N=8192, the amount of data being moved across memory and the network is significant, which reflect the results for 16 and 32 processes across two machines and show a speed-up value below 1.0. 
This occurs because the baseline serial time was measured on a higher-performance local machine, while the multi-machine runs were conducted on the school’s Linux cluster. 
The lower S values primarily reflect the difference in CPU clock speeds and the added network latency of moving data between two separate physical machines. 
In a distributed system, communication across a network is significantly slower than communication within a single machine.

