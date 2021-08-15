#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void dijkstra(int map[100][100],int start_node,int end_node,int node_count,int *path_length,int *path)
{
    int distance[100];//dis(start,i)
    int visited[100];
    int parent[100];
    int parent_count = 0;
    int reverse_path[100];
    int position;
    int temp;
    for(int i=0;i<100;i++)
    {
        visited[i] = 0;
    }
    for(int i=0;i<node_count;i++)
    {
        distance[i] = map[start_node][i];
    }
    parent[start_node] = -1;
    visited[start_node] = 1;
    for(int i=0;i<node_count;i++)//(n-1)node left:do (n-1)times
    {
        position = start_node;
        int min = 99999;
        for(int j=0;j<node_count;j++)//find the node that has the shortest distance with start_node
        {
            if(!visited[j] && distance[j]<min)
            {
                min = distance[j];
                position = j;
            }
        }
        if(i==0)//first time run
        {
            parent[position] = start_node;
        }
        visited[position] = 1;
        for(int j=0;j<node_count;j++)//update
        {
            if(!visited[j] && distance[j]>distance[position]+map[position][j])
            {
                distance[j] = distance[position]+map[position][j];
                parent[j] = position;
            }
            else if(!visited[j] && distance[j]==distance[position]+map[position][j])
            {
                int temp1 = position;
                int temp2 = j;
                int temp1_cnt = 0;//temp1's parent count
                int temp2_cnt = 0;//temp2's paretn count
                int temp_1[100];
                int temp_2[100];
                for(int k=0;k<100;k++)
                {
                    temp_1[k] = -2;
                    temp_2[k] = -2;
                }
                while(parent[temp1]!=-1)
                {
                    temp1_cnt++;
                    temp1 = parent[temp1];
                }
                while(parent[temp2]!=-1)
                {
                    temp2_cnt++;
                    temp2 = parent[temp2];
                }
                temp1 = position;
                temp2 = j;
                for(int k=0;k<temp1_cnt;k++)
                {
                    temp_1[temp1_cnt-1-k] = parent[temp1];
                    temp1 = parent[temp1];
                }
                temp_1[temp1_cnt] = position;
                temp_1[temp1_cnt+1] = j;
                for(int k=0;k<temp2_cnt;k++)
                {
                    temp_2[temp2_cnt-1-k] = parent[temp2];
                    temp2 = parent[temp2];
                }
                temp_2[temp2_cnt] = j;
                // printf("temp_1\n");
                // for(int m=0;m<=temp1_cnt;m++)
                // {
                //     printf("%d ",temp_1[m]);
                // }
                // printf("\ntemp_2\n");
                // for(int m=0;m<=temp2_cnt;m++)
                // {
                //     printf("%d ",temp_2[m]);
                // }
                // printf("\n");
                if(temp2_cnt == temp1_cnt+1)//same hop
                {
                    for(int k=0;k<=temp1_cnt;k++)
                    {
                        if(temp_1[k] < temp_2[k])
                        {
                            parent[j] = position;
                            break;
                        }
                    }
                }
                else if(temp2_cnt > temp1_cnt+1)
                {
                    parent[j] = position;
                }
                // printf("winner:%d\n",parent[switches[j]]);
            }
        }
    }
    /*for(int i=0;i<node_count;i++)
    {
        printf("%d ",distance[i]);
    }*/
    temp = end_node;
    while(parent[temp]!=-1)
    {
        reverse_path[parent_count++] = parent[temp];
        temp = parent[temp];
    }
    for(int i=0;i<parent_count;i++)
    {
        path[parent_count-1-i] = reverse_path[i];
    }
    path[parent_count++] = end_node;
    *path_length = parent_count;
}

int main()
{
    int bits;
    int switch_capacity[100];
    int switch_count;
    int switch_id;
    int map[100][100];
    int start_node;
    int end_node;
    int x,y;
    int weighted;
    int link_count;
    int path[100];
    int path_length;
    int rule_cnt;
    int current_switch_id;
    int original_rule_cnt;
    typedef struct rule{
        char src[100];
        char dst[100];
        char act[100];
    }rule;
    typedef struct table{
        int my_rule;
        int priority;
    }table;
    typedef struct rules_info{
        int upper_bound;
        int lower_bound;
        int left_bound;
        int right_bound;
        int priority;
    }rules_info;
    typedef struct rulse_bit{
        char up[10];
        char low[10];
        char left[10];
        char right[10];
    }rules_bit;
    rule rules[100];
    table tables[100][100];
    rules_info infos[100];
    rules_info original_rule[100];
    rules_info q[100];
    rule inq_rule[10];//start from 1
    scanf("%d",&bits);
    scanf("%d %d",&start_node,&end_node);
    scanf("%d",&switch_count);
    // scanf("%d",&switch_start);
    // scanf("%d",&switch_capacity[switch_start]);
    for(int i=0;i<switch_count;i++)
    {
        scanf("%d",&switch_id);
        scanf("%d",&switch_capacity[switch_id]);
    }
    scanf("%d",&link_count);  
    for(int i=0;i<switch_count;i++)//initialize
    {
        for(int j=0;j<switch_count;j++)
        {
            map[i][j] = 99999;
        }
        map[i][i] = 0;//dis(i,i) = 0
    }
    for(int i=0;i<link_count;i++)
    {
        scanf("%d %d %d",&x,&y,&weighted);
        if(weighted<map[x][y])
        {
            map[x][y] = weighted;
        }
        if(weighted<map[y][x])
        {
            map[y][x] = weighted;
        }
    }
    dijkstra(map,start_node,end_node,switch_count,&path_length,path);
    for(int i=0;i<path_length;i++)
    {
        printf("%d ",path[i]);
    }
    return 0;
}
