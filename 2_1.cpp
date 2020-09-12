#include <bits/stdc++.h>
#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char** argv)
{
  MPI_Init (&argc, &argv);
  int numprocs;
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
  int V;
  int rank;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  if(rank == 0)
  {
    int i = 0, j = 0, k = 0, n = 0;
    float **mat = NULL;
    float d = 0.0;

    cin >> n;

    mat = new float*[2*n];
    for (i = 0; i < 2*n; ++i)
    {
        mat[i] = new float[2*n]();
    }

    for(i = 0; i < n; ++i)
    {
        for(j = 0; j < n; ++j)
        {
            cin >> mat[i][j];
        }
    }

    int low_element = mat[0][0];
    int high_element = mat[n-1][n-1];

    int t1=5;




    for(i = 0; i < n; ++i)
    {
        for(j = 0; j < 2*n; ++j)
        {
            if(j == (i+n))
            {
                mat[i][j] = 1;
            }
        }
    }

    for(i = n; i > 1; --i)
    {
        if(mat[i-1][1] < mat[i][1])
        {
            for(j = 0; j < 2*n; ++j)
            {
                d = mat[i][j];
                mat[i][j] = mat[i-1][j];
                mat[i-1][j] = d;
            }
        }
    }

    for(i = 0; i < n; ++i)
    {
        for(j = 0; j < 2*n; ++j)
        {
            if(j != i)
            {
                d = mat[j][i] / mat[i][i];
                for(k = 0; k < n*2; ++k)
                {
                    mat[j][k] -= mat[i][k]*d;
                }
            }
        }
    }
    for(i = 0; i < n; ++i)
    {
        d = mat[i][i];
        for(j = 0; j < 2*n; ++j)
        {
            mat[i][j] = mat[i][j]/d;
        }
    }


    for(i=0; i < n; ++i)
    {
        for(j = n; j < 2*n; ++j)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
    for (i = 0; i < n; ++i)
    {
        delete[] mat[i];
    }
    delete[] mat;
  }
  MPI_Finalize();

  return 0;
}
