#include<stdio.h>
#include<mpi.h>
#define MAX 100

#define zero 0
#define one 1

int graph[MAX][MAX] = {0};

void dfs(int visited[MAX],int start,int curr,int cycle_len,int *cnt,int V){
    visited[curr] = one;
    if(cycle_len == zero){
        visited[curr] = zero;
        if(graph[curr][start]){
            *cnt = *cnt + one;
            return;
        }
        else
            return;
    }
    for(int i = zero;i < V;++i){
        if(!visited[i] && graph[curr][i]){
            dfs(visited,start,i,cycle_len-one,cnt,V);
        }
    }
    visited[curr] = zero;
}

int func(int t1, int t2, int V, int n,int rank, int numprocs)
{

    MPI_Status status;


        int visited[MAX] = {0};
        int totalCycles = zero;

    if(rank == zero)
    {
      int nodesPerProcess = (V-(n-one))/(numprocs-one);
        int leftOverNodes = (V-(n-one)) % (numprocs - one);

        int low_element = zero,high_element = nodesPerProcess - one;

        if(leftOverNodes != zero){
            high_element = leftOverNodes - one;

            int cnt = zero;
            for(int node = low_element; node <= high_element; ++node){
                dfs(visited,node,node,n-one,&cnt,V);
                visited[node] = one;
            }
            totalCycles += cnt;

            low_element = leftOverNodes;
            high_element += nodesPerProcess;
        }

        for(int dest = one;dest < numprocs;dest++){
            MPI_Send(&low_element, one, MPI_INT, dest, t1, MPI_COMM_WORLD);
            MPI_Send(&high_element, one, MPI_INT, dest, t1 + one, MPI_COMM_WORLD);
            MPI_Send(&V, one, MPI_INT, dest, t1 + 2, MPI_COMM_WORLD);
            MPI_Send(&n, one, MPI_INT, dest, t1 + 3, MPI_COMM_WORLD);
            low_element = high_element + one;
                        high_element +=  nodesPerProcess;
                    }
                }

                MPI_Bcast(&graph, MAX * MAX, MPI_INT, zero, MPI_COMM_WORLD);
                MPI_Bcast(&visited,MAX,MPI_INT,zero,MPI_COMM_WORLD);

                if(rank > zero){

                    int low_element,high_element,V,n;
                    MPI_Recv(&low_element,one, MPI_INT, zero, t1, MPI_COMM_WORLD, &status);
                    MPI_Recv(&high_element,one, MPI_INT, zero, t1 +one, MPI_COMM_WORLD, &status);
                    MPI_Recv(&V,one, MPI_INT, zero, t1 + 2, MPI_COMM_WORLD, &status);
                    MPI_Recv(&n,one, MPI_INT, zero, t1 + 3, MPI_COMM_WORLD, &status);
                    int localWalks = zero;

                    int cnt = zero;
                    for(int node = low_element;node <= high_element;node++){
                        dfs(visited,node,node,n-1,&cnt,V);
                        visited[node] =one;
                    }

                    MPI_Send(&cnt,one, MPI_INT, zero, t2, MPI_COMM_WORLD);
                }

                if(rank == zero){

                    int y;
                    for(int src = one;src < numprocs;++src){
                        MPI_Recv(&y, one, MPI_INT, src, t2, MPI_COMM_WORLD, &status);
                        totalCycles += y;
                    }
                    return totalCycles/2;
                }

            }

    int main(int argc, char** argv){
      int numprocs,rank;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);

        int V;

    if(rank == 0){
        int e;
            scanf("%d %d",&V,&e);

            int s,d;
            for(int i = 0;i < e;i++){
                scanf("%d %d",&s,&d);
                s--;
                d--;
                graph[s][d] = 1;
                graph[d][s] = 1;
            }

    }
    int teen = func(5,10,V,3,rank,numprocs);
    int chaar = func(20,30,V,4,rank,numprocs);

    if(rank == 0)
      printf("%d %d\n",teen,chaar);


    MPI_Finalize();
    return 0;
}
