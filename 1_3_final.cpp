#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<bits/stdc++.h>
using namespace std;


int highest_val_in_block(int rank,int  p,int n)
{
  return ((((rank)+1) * (n) / (p) / 2) - 1);
}


int min(int a, int b)
{
  if(a>b) return a;
  return b;
}


int size_of_block(int rank,int p,int n)
{
  return  ((((rank)+1) * (n) / (p) / 2) - (((rank)) * (n) / (p) / 2));
}


int primes_before_n(int n)
{
  int rank, p;
  int sqrtn = sqrt(n);

  char* primes = (char*)calloc(sqrtn + 1, 1);
  for (int prime_mult = 2;prime_mult <= sqrtn;prime_mult += 2)
  {
          primes[prime_mult] = 1;
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  int low  = 3 + ((rank) * (n-1) / (p) / 2)  * 2;
  int high = 3 + highest_val_in_block(rank, p, n - 1) * 2;
  int size = size_of_block(rank, p, n - 1);


  int proc0_size = (n-1)/p;


  for(int prime = 3; prime <= sqrtn; prime += 2)
  {
          if(primes[prime] == 1) continue;

          for(int prime_mult = prime << 1; prime_mult<=sqrtn; prime_mult+=prime)
          {
                  primes[prime_mult] = 1;
          }
  }

  char* marked = (char*)calloc(size * sizeof(char),1);
  int num_per_block = 1024*1024;
  int block_low = low;
  int block_high = min(high, low+num_per_block*2);

    int block_ind1=0, prime, first, first_val_ind;

    while(block_ind1<size)
    {
      prime = 3;
      while (prime<=sqrtn)
      {

              if(primes[prime]!=1)
              {

                if(prime > sqrt(block_low))
                        first = prime*prime;

                else
                {
                        if((block_low % prime))
                                first = prime - (block_low%prime)+ block_low;
                        else
                                first = block_low;

                }

                if ((first+prime) % 2)
                        first += prime;

                first_val_ind = (first - 3) / 2 - ((rank) * (n-1) / (p) / 2);
                int prime_double = prime << 1;
                int prime_step = prime_double / 2;

                for(int i = first; i<= high; i += prime_double)
                {
                  marked[first_val_ind] = 1;
                  first_val_ind += prime_step;
                }

              }
              prime++;

            }

            block_low += num_per_block * 2;
            block_high = min(high, block_high + num_per_block*2);
            block_ind1 += num_per_block;
          }

          int count = 0, global_count;
          int i = 0;
           while (i<size)
           {
             if (!marked[i])
                 count++;
            i++;
           }



           MPI_Reduce(&count, &global_count, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);

           if(rank == 0){
             global_count += 1;
             return global_count;
           }

           return 0;
   }


   int main(int argc, char** argv){
           MPI_Init(&argc, &argv);
           MPI_Barrier(MPI_COMM_WORLD);
           int rank,  numprocs;
           MPI_Comm_rank (MPI_COMM_WORLD, &rank);
           MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
           int p = atoi(argv[1]);
             int ans1 = primes_before_n(p);
             int ans2 = primes_before_n(p-1);
             if(rank == 0)
             cout<<ans1-ans2<<endl;

           MPI_Finalize();

           return 0;
   }
