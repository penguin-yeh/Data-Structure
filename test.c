#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int adjacency_matrix[100][100];//[i][j]=1:i has child j

int count = 0;
int path_count = 0;
int visited[100];
int path[100];
int switches[100];
// int switch_start;

int power(int num,int a)
{
    if(a==0)
    {
        return 1;
    }
    int temp = num;
    for(int i=1;i<a;i++)
    {
        num = temp*num;
    }
    return num;
}

int main()
{
    printf("%d",power(2,0));
    return 0;
}