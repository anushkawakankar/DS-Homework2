#include<stdio.h>
#include<mpi.h>
#define MAX 100
#define one 1
#define zero 0

int graph[MAX][MAX] = {0};

void dfs(int graph[MAX][MAX],int vis[MAX],int start,int curnode,int cycle_len,int *cnt,int V){
    vis[curnode] = one;

    if(cycle_len != zero){
      continue;
    }
    else
    {
      vis[curnode] = zero;
      if(!graph[curnode][start]) return;
      else{
          *cnt = *cnt + one;
          return;
      }
    }

    int i = zero;
    while(i<V)
    {
      if(vis[i]) continue;
      else if(!vis[i] && graph[curnode][i])
      {
        dfs(graph,vis,start,i,cycle_len-1,cnt,V);
      }
      i++;
    }

    vis[curnode] = zero;
}

int TeenWaala(int tag1, int tag2 ,int n, int V)
{
    int numprocs,rank;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
    MPI_Status status;
    int vis[MAX] = {0};
    int totalCycles = 0;
    if(rank == 0)
    {
      int nodesPerProcess = (V-(n-1))/(numprocs-1);
        int leftOverNodes = (V-(n-1)) % (numprocs - 1);

        int lower_bound = 0,upper_bound = nodesPerProcess - 1;

        if(leftOverNodes != 0){
            upper_bound = leftOverNodes - 1;

            int cnt = 0;
            for(int node = lower_bound; node <= upper_bound; ++node){
                dfs(graph,vis,node,node,n-1,&cnt,V);
                vis[node] = 1;
            }
            totalCycles += cnt;

            lower_bound = leftOverNodes;
            upper_bound += nodesPerProcess;
        }

        for(int dest = 1;dest < numprocs;dest++){
            MPI_Send(&lower_bound, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
            MPI_Send(&upper_bound, 1, MPI_INT, dest, tag1 + 1, MPI_COMM_WORLD);
            MPI_Send(&V, 1, MPI_INT, dest, tag1 + 2, MPI_COMM_WORLD);
            MPI_Send(&n, 1, MPI_INT, dest, tag1 + 3, MPI_COMM_WORLD);
            lower_bound = upper_bound + 1;
            upper_bound +=  nodesPerProcess;
        }
    }

    MPI_Bcast(&graph, MAX * MAX, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vis,MAX,MPI_INT,0,MPI_COMM_WORLD);

    if(rank > 0){

        int lower_bound,upper_bound,V,n;
        MPI_Recv(&lower_bound, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
        MPI_Recv(&upper_bound, 1, MPI_INT, 0, tag1 + 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&V, 1, MPI_INT, 0, tag1 + 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&n, 1, MPI_INT, 0, tag1 + 3, MPI_COMM_WORLD, &status);
        int localWalks = 0;

        int cnt = 0;
        for(int node = lower_bound;node <= upper_bound;node++){
            dfs(graph,vis,node,node,n-1,&cnt,V);
            vis[node] = 1;
        }

        MPI_Send(&cnt, 1, MPI_INT, 0, tag2, MPI_COMM_WORLD);
    }
    if(rank == 0){

        int x;
        for(int src = 1;src < numprocs;++src){
            MPI_Recv(&x, 1, MPI_INT, src, tag2, MPI_COMM_WORLD, &status);
            totalCycles += x;
        }
        return(totalCycles/2);
    }
    return 0;

}




int main(int argc, char** argv){

    MPI_Init (&argc, &argv);




    if(rank == 0){
    	int n,k;
	    cin>>&n;
      cin>>&k;

	    int s,d;
	    for(int i = 0;i < k;i++){
	        scanf("%d %d",&s,&d);
          s--;
          d--;
	        graph[s][d] = 1;
	        graph[d][s] = 1;
	    }
      int teen = TeenWaala(5,10,3,n);
      int chaar = TeenWaala(20,30,4,n);
      cout<<teen<<" "<<chaar<<endl;
    }



    MPI_Finalize();
    return 0;
}
