
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int adjacency_matrix[100][100];//[i][j]=1:i has child j

int count = 0;
int path_count = 0;
int visited[100];
int path[100];
int switches[100];

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
        distance[switches[i]] = map[start_node][switches[i]];
    }
    parent[start_node] = -1;
    visited[start_node] = 1;
    for(int i=0;i<node_count-1;i++)//(n-1)node left:do (n-1)times
    {
        position = start_node;
        int min = 99999;
        for(int j=0;j<node_count;j++)//find the node that has the shortest distance with start_node
        {
            if(!visited[switches[j]] && distance[switches[j]]<min)
            {
                min = distance[switches[j]];
                position = switches[j];
            }
        }
        if(i==0)//first time run
        {
            parent[position] = start_node;
        }
        visited[position] = 1;
        for(int j=0;j<node_count;j++)//update
        {
            if(!visited[switches[j]] && distance[switches[j]]>distance[position]+map[position][switches[j]])
            {
                distance[switches[j]] = distance[position]+map[position][switches[j]];
                parent[switches[j]] = position;
            }
            else if(!visited[switches[j]] && distance[switches[j]]==distance[position]+map[position][switches[j]])
            {
                int temp1 = position;
                int temp2 = switches[j];
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
                temp2 = switches[j];
                for(int k=0;k<temp1_cnt;k++)
                {
                    temp_1[temp1_cnt-1-k] = parent[temp1];
                    temp1 = parent[temp1];
                }
                temp_1[temp1_cnt] = position;
                temp_1[temp1_cnt+1] = switches[j];
                for(int k=0;k<temp2_cnt;k++)
                {
                    temp_2[temp2_cnt-1-k] = parent[temp2];
                    temp2 = parent[temp2];
                }
                temp_2[temp2_cnt] = switches[j];
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
                            parent[switches[j]] = position;
                            break;
                        }
                    }
                }
                else if(temp2_cnt > temp1_cnt+1)
                {
                    parent[switches[j]] = position;
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
    int max_id = -10;
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
    for(int i=0;i<switch_count;i++)
    {
        scanf("%d",&switch_id);
        if(switch_id>max_id)
        {
            max_id = switch_id;
        }
        switches[i] = switch_id;
        scanf("%d",&switch_capacity[switches[i]]);
    }
    scanf("%d",&link_count);
    for(int i=0;i<=max_id;i++)//initialize
    {
        for(int j=0;j<=max_id;j++)
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
    scanf("%d",&rule_cnt);
    original_rule_cnt = rule_cnt;
    for(int i=1;i<=rule_cnt;i++)
    {
        scanf("%s",rules[i].src);
        scanf("%s",rules[i].dst);
        scanf("%s",rules[i].act);
    }
    for(int i=1;i<=rule_cnt;i++)
    {
        int temp_upper = 0;
        int temp_lower = 0;
        int temp_left = 0 ;
        int temp_right = 0;
        int num;
        if(strcmp(rules[i].src,"*")==0)
        {
            infos[i].upper_bound = 0;
            infos[i].lower_bound = power(2,bits)-1;
        }
        else if(strcmp(rules[i].src,"*")!=0)
        {
            for(int j=0;j<bits;j++)
            {
                if(rules[i].src[j]=='*')
                {
                    temp_lower = temp_upper + power(2,bits-j)-1;
                    break;
                }
                else if(rules[i].src[j]!='*')
                {
                    num = rules[i].src[j]-48;
                    temp_upper = num*power(2,bits-1-j) + temp_upper;
                    temp_lower = temp_upper;
                }
            }
            infos[i].upper_bound = temp_upper;
            infos[i].lower_bound = temp_lower;
        }
        if(strcmp(rules[i].dst,"*")==0)
        {
            infos[i].left_bound = 0;
            infos[i].right_bound = power(2,bits)-1;
        }
        else if(strcmp(rules[i].dst,"*")!=0)
        {
            for(int j=0;j<bits;j++)
            {
                if(rules[i].dst[j]=='*')
                {
                    temp_right = temp_left + power(2,bits-j)-1;
                    break;
                }
                else if(rules[i].dst[j]!='*')
                {
                    num = rules[i].dst[j]-48;
                    temp_left = num*power(2,bits-1-j) + temp_left;
                    temp_right = temp_left;
                }
            }
            infos[i].left_bound = temp_left;
            infos[i].right_bound = temp_right;
        }
        infos[i].priority = i;
    }
    for(int i=1;i<=rule_cnt;i++)
    {
        original_rule[i] = infos[i];
    }
    for(int i=0;i<power(2,bits);i++)//initialize tables
    {
        for(int j=0;j<power(2,bits);j++)
        {
            tables[i][j].priority = 99999;
        }
    }
    for(int i=0;i<power(2,bits);i++)//build table
    {
        for(int j=0;j<power(2,bits);j++)
        {
            for(int k=1;k<=rule_cnt;k++)
            {
                if(i<=infos[k].right_bound && i>=infos[k].left_bound && j>=infos[k].upper_bound && j<=infos[k].lower_bound )
                {
                    if(tables[i][j].priority>k)
                    {
                        tables[i][j].my_rule = k;
                        tables[i][j].priority = k;
                    }
                }
            }
        }
    }
    // printf("capacity\n");
    // for(int i=0;i<switch_count;i++)
    // {
    //     printf("capacity[%d]=%d\n",switch_capacity[switches[path[i]]],switch_capacity[path[i]]);
    // }
    printf("%d\n",path_length);
    // printf("path\n") ;
    // {
    //     for(int i=0;i<path_length;i++)
    //     {
    //         printf("%d ",path[i]);
    //     }
    // }
    printf("\n");
    printf("capacity:\n");
    for(int i=0;i<path_length;i++)
    {
        printf("%d ",switch_capacity[path[i]]);
    }
    printf("\n");
    int end = 0;
    int q_cnt = 1;
    for(int a=0;a<path_length;a++)
    {
         if(switch_capacity[switches[path[a]]]<rule_cnt && end==0)
        {   
            int total = power(2,bits);
            int temp_x_cnt = 0;
            int first = 1;
            float internal_rule_cnt = 0;
            float overlapping_rule_cnt = 0;
            float utility = -1;
            int area = 0;
            float temp_internal = 0;
            float temp_overlapping = 0;
            float temp_utility ;
            int temp_area;
            int true_q[100];//save internal rule ID in sequence,starts at 0
            int temp_q[100];
            int q_inside[100];//save relations between rules and q
            int true_q_inside[100];
            int q_inside_cnt = 0;
            int true_q_inside_cnt = 0;
            for(int k=0;k<total;k=power(2,temp_x_cnt)-1)//decide q's range:left,right bound
            {
                int q_left = 0;
                int q_right = k;
                int x_dis = (q_right - q_left)+1;
                int x_repeated_times = total/x_dis ;
                int temp_y_cnt = 0;
                for(int m=0;m<total;m=power(2,temp_y_cnt)-1)//q's  upper,lower bound
                {
                    int q_upper = 0;
                    int q_lower = m;
                    if(q_left==0 && q_right==power(2,bits)-1 && q_upper==0 && q_lower==power(2,bits)-1)
                        break;
                    int y_dis = (q_lower - q_upper)+1;
                    int y_repeated_times = total/y_dis ;
                    for(int n=0;n<x_repeated_times;n++)//current left,right
                    {
                        q_left = 0;//reset
                        q_right = k;
                        q_left = q_left + n*x_dis;
                        q_right = q_right + n*x_dis;
                        for(int p=0;p<y_repeated_times;p++)//current upper,lower
                        {
                            q_upper = 0;//reset upper,lower
                            q_lower = m;
                            q_upper = q_upper + p*y_dis;
                            q_lower = q_lower + p*y_dis;
                            for(int u=1;u<=rule_cnt;u++)
                            {
                                q_inside[u] = 0;
                            }
                            for(int u=q_left;u<=q_right;u++)
                            {
                                for(int v=q_upper;v<=q_lower;v++)
                                {
                                    q_inside[tables[u][v].my_rule] = 1;
                                }
                            }
                            for(int s=0;s<100;s++)
                            {
                                temp_q[s] = 0;
                            }
                            temp_internal = 0;
                            temp_overlapping = 0;
                            for(int r=1;r<=rule_cnt;r++)
                            {
                                if(q_upper<=infos[r].upper_bound && q_lower>=infos[r].lower_bound && q_left<=infos[r].left_bound && q_right>=infos[r].right_bound)
                                {
                                    temp_q[(int)temp_internal++] = r;
                                    q_inside[r] = 2;//r is inside q and internal
                                }
                            }
                            temp_area = (q_right-q_left+1)*(q_lower-q_upper+1);
                            for(int l=1;l<=rule_cnt;l++)
                            {
                                if(q_inside[l] == 1)
                                {
                                    temp_overlapping++;
                                }
                            }
                            q_inside_cnt = (int)temp_internal+(int)temp_overlapping;
                            temp_utility = (temp_internal-1)/(temp_overlapping+1);
                            if(temp_utility>utility && temp_utility>0 && q_inside_cnt<=switch_capacity[path[a]])
                            {
                                for(int s=0;s<temp_internal;s++)
                                {
                                    true_q[s] = temp_q[s];
                                }
                                for(int s=1;s<=rule_cnt;s++)
                                {
                                    true_q_inside[s] = q_inside[s];
                                }
                                area = temp_area;
                                utility = temp_utility;
                                internal_rule_cnt = temp_internal;
                                overlapping_rule_cnt = temp_overlapping;
                                true_q_inside_cnt = q_inside_cnt;
                                q[q_cnt].upper_bound = q_upper;
                                q[q_cnt].lower_bound = q_lower;
                                q[q_cnt].left_bound = q_left;
                                q[q_cnt].right_bound = q_right;
                            }
                            else if(temp_utility == utility && temp_utility>0 && q_inside_cnt<=switch_capacity[path[a]])
                            {
                                if(temp_internal>internal_rule_cnt)
                                {
                                    for(int s=1;s<=rule_cnt;s++)
                                    {
                                        true_q_inside[s] = q_inside[s];
                                    }
                                    area = temp_area;
                                    utility = temp_utility;
                                    internal_rule_cnt = temp_internal;
                                    overlapping_rule_cnt = temp_overlapping;
                                    true_q_inside_cnt = q_inside_cnt;
                                    q[q_cnt].upper_bound = q_upper;
                                    q[q_cnt].lower_bound = q_lower;
                                    q[q_cnt].left_bound = q_left;
                                    q[q_cnt].right_bound = q_right;
                                }
                                else if(temp_area<area && temp_internal==internal_rule_cnt)
                                {
                                    for(int s=1;s<=rule_cnt;s++)
                                    {
                                        true_q_inside[s] = q_inside[s];
                                    }
                                    area = temp_area;
                                    utility = temp_utility;
                                    internal_rule_cnt = temp_internal;
                                    overlapping_rule_cnt = temp_overlapping;
                                    true_q_inside_cnt = q_inside_cnt;
                                    q[q_cnt].upper_bound = q_upper;
                                    q[q_cnt].lower_bound = q_lower;
                                    q[q_cnt].left_bound = q_left;
                                    q[q_cnt].right_bound = q_right;
                                }
                                else if(temp_area==area && temp_internal==internal_rule_cnt)
                                {
                                    for(int s=0;s<temp_internal;s++)
                                    {
                                        if(temp_q[s]<true_q[s])
                                        {
                                            for(int s=1;s<=rule_cnt;s++)
                                            {
                                                true_q_inside[s] = q_inside[s];
                                            }
                                            area = temp_area;
                                            utility = temp_utility;
                                            internal_rule_cnt = temp_internal;
                                            overlapping_rule_cnt = temp_overlapping;
                                            true_q_inside_cnt = q_inside_cnt;
                                            q[q_cnt].upper_bound = q_upper;
                                            q[q_cnt].lower_bound = q_lower;
                                            q[q_cnt].left_bound = q_left;
                                            q[q_cnt].right_bound = q_right;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(temp_y_cnt+1 > bits)
                        break;
                    temp_y_cnt++;
                }
                if(temp_x_cnt+1 > bits)
                    break;
                temp_x_cnt++;
            }
            rules_info in_q[100][100];//both start from 1,be cut!
            int temp_q_inside_cnt = 1;
            for(int j=1;j<=rule_cnt;j++)//find the range of rules in q
            {
                if(true_q_inside[j] == 1)//means j rule is overlapping
                {
                    strcpy(inq_rule[temp_q_inside_cnt].act,rules[j].act);
                    if(infos[j].upper_bound<=q[a+1].upper_bound)//choose bigger
                    {
                        in_q[q_cnt][temp_q_inside_cnt].upper_bound = q[a+1].upper_bound;
                    }
                    else
                    {
                        in_q[q_cnt][temp_q_inside_cnt++].upper_bound = infos[j].upper_bound;
                    }
                    if(infos[j].lower_bound<=q[a+1].lower_bound)//choose smaller
                    {
                        in_q[q_cnt][temp_q_inside_cnt].lower_bound = infos[j].lower_bound;
                    }
                    else
                    {
                        in_q[q_cnt][temp_q_inside_cnt].lower_bound = q[a+1].lower_bound;
                    }
                    if(infos[j].left_bound<=q[a+1].left_bound)//choose bigger
                    {
                        in_q[q_cnt][temp_q_inside_cnt].left_bound = q[a+1].left_bound;
                    }
                    else
                    {
                        in_q[q_cnt][temp_q_inside_cnt].left_bound = infos[j].left_bound;
                    }
                    if(infos[j].right_bound<=q[a+1].right_bound)//choose smaller
                    {
                        in_q[q_cnt][temp_q_inside_cnt].right_bound = infos[j].right_bound;
                    }
                    else
                    {
                        in_q[q_cnt][temp_q_inside_cnt].right_bound = q[a+1].right_bound;
                    }
                    temp_q_inside_cnt++;
                }
                else if(true_q_inside[j]==2)//means j rule is internal
                {
                    strcpy(inq_rule[temp_q_inside_cnt].act,rules[j].act);
                    in_q[q_cnt][temp_q_inside_cnt].upper_bound = infos[j].upper_bound;
                    in_q[q_cnt][temp_q_inside_cnt].lower_bound = infos[j].lower_bound;
                    in_q[q_cnt][temp_q_inside_cnt].left_bound = infos[j].left_bound;
                    in_q[q_cnt][temp_q_inside_cnt].right_bound = infos[j].right_bound;
                    temp_q_inside_cnt++;
                }
            }
            char temp_up[10];
            char temp_low[10];
            char temp_left[10];
            char temp_right[10];
            rules_bit temp_bits[q_inside_cnt+1];//starts from 1
            rules_bit qq;
            rule goal_q;
            for(int j=0;j<10;j++)
            {
                temp_up[j] = '\0';
                temp_low[j] = '\0';
                temp_left[j] = '\0';
                temp_right[j] = '\0';
            }
            int up = q[q_cnt].upper_bound;
            int low = q[q_cnt].lower_bound;
            int left = q[q_cnt].left_bound;
            int right = q[q_cnt].right_bound;
            for(int k=0;k<bits;k++)
            {
                if(up%2 == 0)
                {
                    qq.up[bits-1-k] = '0';
                }
                else if(up%2 == 1)
                {
                    qq.up[bits-1-k] = '1';
                }
                if(low%2 == 0)
                {
                    qq.low[bits-1-k] = '0';
                }
                else if(low%2 == 1)
                {
                    qq.low[bits-1-k] = '1';
                }
                if(left%2 == 0)
                {
                    qq.left[bits-1-k] = '0';
                }
                else if(left%2 == 1)
                {
                    qq.left[bits-1-k] = '1';
                }
                if(right%2 == 0)
                {
                    qq.right[bits-1-k] = '0';
                }
                else if(right%2 == 1)
                {
                    qq.right[bits-1-k] = '1';
                }
                up = up/2;
                low = low/2;
                left = left/2;
                right = right/2;
            }
            qq.up[bits] = '\0';
            qq.low[bits] = '\0';
            qq.left[bits] = '\0';
            qq.right[bits] = '\0';
            int flag_src = 1;
            int flag_dst = 1;
            for(int k=0;k<bits;k++)
            {
                if(qq.up[k]=='0' && qq.low[k] == '0' && flag_src==1)
                {
                    goal_q.src[k] = '0';
                }
                if(qq.up[k]=='1' && qq.low[k] == '1' && flag_src==1)
                {
                    goal_q.src[k] = '1';
                }
                if(qq.up[k] != qq.low[k] && flag_src==1)
                {
                    flag_src = 0;
                    goal_q.src[k] = '*';
                    goal_q.src[k+1] = '\0';
                }
                if(qq.left[k]=='0' && qq.right[k] == '0' && flag_dst==1)
                {
                    goal_q.dst[k] = '0';
                }
                if(qq.left[k]=='1' && qq.right[k] == '1' && flag_dst==1)
                {
                    goal_q.dst[k] = '1';
                }
                if(qq.left[k] != qq.right[k] && flag_dst==1)
                {
                    flag_dst = 0;
                    goal_q.dst[k] = '*';
                    goal_q.dst[k+1] = '\0';
                }
            }
            for(int j=1;j<=true_q_inside_cnt;j++)//range -> string
            {
                int temp_up = in_q[q_cnt][j].upper_bound;
                int temp_low = in_q[q_cnt][j].lower_bound;
                int temp_left = in_q[q_cnt][j].left_bound;
                int temp_right = in_q[q_cnt][j].right_bound;
                for(int k=0;k<bits;k++)
                {
                    if(temp_up%2 == 0)
                    {
                        temp_bits[j].up[bits-1-k] = '0';
                    }
                    else if(temp_up%2 == 1)
                    {
                        temp_bits[j].up[bits-1-k] = '1';
                    }
                    if(temp_low%2 == 0)
                    {
                        temp_bits[j].low[bits-1-k] = '0';
                    }
                    else if(temp_low%2 == 1)
                    {
                        temp_bits[j].low[bits-1-k] = '1';
                    }
                    if(temp_left%2 == 0)
                    {
                        temp_bits[j].left[bits-1-k] = '0';
                    }
                    else if(temp_left%2 == 1)
                    {
                        temp_bits[j].left[bits-1-k] = '1';
                    }
                    if(temp_right%2 == 0)
                    {
                        temp_bits[j].right[bits-1-k] = '0';
                    }
                    else if(temp_right%2 == 1)
                    {
                        temp_bits[j].right[bits-1-k] = '1';
                    }
                    temp_up = temp_up/2;
                    temp_low = temp_low/2;
                    temp_left = temp_left/2;
                    temp_right = temp_right/2;
                }
                temp_bits[j].up[bits] = '\0';
                temp_bits[j].low[bits] = '\0';
                temp_bits[j].left[bits] = '\0';
                temp_bits[j].right[bits] = '\0';
            }
            for(int j=1;j<=true_q_inside_cnt;j++)
            {
                for(int k=0;k<bits;k++)
                {
                    inq_rule[j].src[k] = '\0';
                    inq_rule[j].dst[k] = '\0';
                }
            }
            for(int j=1;j<=true_q_inside_cnt;j++)//string -> prefix
            {
                int flag_src = 1;
                int flag_dst = 1;
                for(int k=0;k<bits;k++)
                {
                    if(temp_bits[j].up[k]=='0' && temp_bits[j].low[k] == '0' && flag_src==1)
                    {
                        inq_rule[j].src[k] = '0';
                    }
                    if(temp_bits[j].up[k]=='1' && temp_bits[j].low[k] == '1' && flag_src==1)
                    {
                        inq_rule[j].src[k] = '1';
                    }
                    if(temp_bits[j].up[k] != temp_bits[j].low[k] && flag_src==1)
                    {
                        flag_src = 0;
                        inq_rule[j].src[k] = '*';
                        inq_rule[j].src[k+1] = '\0';
                    }
                    if(temp_bits[j].left[k]=='0' && temp_bits[j].right[k] == '0' && flag_dst==1)
                    {
                        inq_rule[j].dst[k] = '0';
                    }
                    if(temp_bits[j].left[k]=='1' && temp_bits[j].right[k] == '1' && flag_dst==1)
                    {
                        inq_rule[j].dst[k] = '1';
                    }
                    if(temp_bits[j].left[k] != temp_bits[j].right[k] && flag_dst==1)
                    {
                        flag_dst = 0;
                        inq_rule[j].dst[k] = '*';
                        inq_rule[j].dst[k+1] = '\0';
                    }
                }
            }
            printf("%d %d\n",path[a],true_q_inside_cnt);
            for(int j=1;j<=true_q_inside_cnt;j++)
            {
                printf("%s %s %s\n",inq_rule[j].src,inq_rule[j].dst,inq_rule[j].act);
            }
            rule temp_rules[100];//rewrite rules
            for(int j=1;j<=rule_cnt;j++)
            {
                temp_rules[j] = rules[j];
            }
            strcpy(goal_q.act ,"Fwd");
            int current = 1;
            strcpy(rules[current].src,goal_q.src);
            strcpy(rules[current].dst,goal_q.dst);
            strcpy(rules[current].act,goal_q.act);
            current++;
            for(int j=1;j<=rule_cnt;j++)
            {
                if(true_q_inside[j] != 2)//rule j is not internal
                {
                    strcpy(rules[current].src,temp_rules[j].src);
                    strcpy(rules[current].dst,temp_rules[j].dst);
                    strcpy(rules[current].act,temp_rules[j].act);
                    current++;
                }
            }
            int count = 0;
            for(int i=1;i<=rule_cnt;i++)
            {
                if(q[a+1].upper_bound<=infos[i].upper_bound && q[a+1].lower_bound>=infos[i].lower_bound && q[a+1].left_bound<=infos[i].left_bound && q[a+1].right_bound>=infos[i].right_bound)
                {
                    if(true_q_inside[i] == 2);//i rule is internal
                    count++;
                }
            }
            rule_cnt = rule_cnt-count+1;
            for(int i=1;i<=rule_cnt;i++)
            {
                int temp_upper = 0;
                int temp_lower = 0;
                int temp_left = 0 ;
                int temp_right = 0;
                int num;
                if(strcmp(rules[i].src,"*")==0)
                {
                    infos[i].upper_bound = 0;
                    infos[i].lower_bound = power(2,bits)-1;
                }
                else if(strcmp(rules[i].src,"*")!=0)
                {
                    for(int j=0;j<bits;j++)
                    {
                        if(rules[i].src[j]=='*')
                        {
                            temp_lower = temp_upper + power(2,bits-j)-1;
                            break;
                        }
                        else if(rules[i].src[j]!='*')
                        {
                            num = rules[i].src[j]-48;
                            temp_upper = num*power(2,bits-1-j) + temp_upper;
                            temp_lower = temp_upper;
                        }
                    }
                    infos[i].upper_bound = temp_upper;
                    infos[i].lower_bound = temp_lower;
                }
                if(strcmp(rules[i].dst,"*")==0)
                {
                    infos[i].left_bound = 0;
                    infos[i].right_bound = power(2,bits)-1;
                }
                else if(strcmp(rules[i].dst,"*")!=0)
                {
                    for(int j=0;j<bits;j++)
                    {
                        if(rules[i].dst[j]=='*')
                        {
                            temp_right = temp_left + power(2,bits-j)-1;
                            break;
                        }
                        else if(rules[i].dst[j]!='*')
                        {
                            num = rules[i].dst[j]-48;
                            temp_left = num*power(2,bits-1-j) + temp_left;
                            temp_right = temp_left;
                        }
                    }
                    infos[i].left_bound = temp_left;
                    infos[i].right_bound = temp_right;
                }
                infos[i].priority = i;
            }
            for(int i=0;i<power(2,bits);i++)//initialize tables
            {
                for(int j=0;j<power(2,bits);j++)
                {
                    tables[i][j].priority = 99999;
                }
            }
            for(int i=0;i<power(2,bits);i++)//build table
            {
                for(int j=0;j<power(2,bits);j++)
                {
                    for(int k=1;k<=rule_cnt;k++)
                    {
                        if(i<=infos[k].right_bound && i>=infos[k].left_bound && j>=infos[k].upper_bound && j<=infos[k].lower_bound )
                        {
                            if(tables[i][j].priority>k)
                            {
                                tables[i][j].my_rule = k;
                                tables[i][j].priority = k;
                            }
                        }
                    }
                }
            }
            table temp_tables[100][100];
            for(int i=0;i<power(2,bits);i++)
            {
                for(int j=0;j<power(2,bits);j++)
                {
                    temp_tables[i][j] = tables[i][j];
                }
            }
            int fwd[50];
            int fwd_cnt=0;
            int covered[50];
            int covered_cnt = 0;
            for(int i=0;i<=rule_cnt;i++)
            {
                fwd[i] = 0;
                covered[i] = 1;
            }
            for(int i=1;i<=rule_cnt;i++)//刪掉被Fwd全部覆蓋住的rule
            {
                if(strcmp(rules[i].act,"Fwd")==0)
                {
                    fwd[fwd_cnt++]=i;
                }
            }
            for(int i=0;i<power(2,bits);i++)
            {
                for(int j=0;j<power(2,bits);j++)
                {
                    for(int k=0;k<fwd_cnt;k++)
                    {
                        if(temp_tables[i][j].my_rule == fwd[k])
                        {
                            temp_tables[i][j].my_rule = 1000;
                        }
                    }
                }
            }
            for(int i=fwd_cnt+1;i<=rule_cnt;i++)
            {
                int flag = 1;
                for(int j=infos[i].left_bound;j<=infos[i].right_bound;j++)
                {
                    for(int k=infos[i].upper_bound;k<=infos[i].lower_bound;k++)
                    {
                        for(int m=0;m<fwd_cnt;m++)
                        {
                            if(temp_tables[j][k].my_rule!=1000)
                            {
                                flag = 0;
                            }
                        }
                    }
                }
                if(flag == 1)//covered
                {
                    covered[covered_cnt++] = i;
                }
            }
            rules_info temp_infos[100];
            int temp_covered_cnt = 1;
            int temp_rule_cnt = 1;
            for(int i=1;i<=rule_cnt;i++)
            {
                temp_rules[i] = rules[i];
                temp_infos[i] = infos[i];
            }
            for(int i=1;i<=rule_cnt;i++)
            {
                if(i!=covered[temp_covered_cnt-1])
                {
                    rules[i] = temp_rules[temp_rule_cnt++];
                }
                else if(i == covered[temp_covered_cnt-1])
                {
                    temp_rule_cnt++;
                    temp_covered_cnt++;
                }
            }
            rule_cnt = rule_cnt-covered_cnt;
            q_cnt++;
        }
        else if(switch_capacity[path[a]]>=rule_cnt && end==0)
        {
            end = 1;
            printf("%d %d\n",path[a],rule_cnt);
            for(int j=1;j<=rule_cnt;j++)
            {
                printf("%s %s %s\n",rules[j].src,rules[j].dst,rules[j].act);
            }
            rule_cnt = 0;
        }
        else if(end == 1)
        {
            printf("%d %d\n",path[a],0);
        }
    }
    return 0;
}
