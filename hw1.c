#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double square(double input_1,double input_2)//算出兩數平方的差距

{
    return (input_1 - input_2)*(input_1 - input_2);
}

int main()
{
    int node_count;
    int repeated_count;
    double node_value[100];
    double coordinate[100][2];//[][0]存x值,[][1]存y值
    double weighted;
    double neighbor_count[100];
    double outcomes[100];
    int link[100][100];//判斷兩點是不是neighbor
    int node_position;
    int current_position;
    scanf("%d %d",&node_count,&repeated_count);
    for(int count=0;count<node_count;count++)//把每個node的資料讀入
    {
        scanf("%d",&node_position);
        scanf("%lf %lf %lf",&coordinate[node_position][0],&coordinate[node_position][1],&node_value[node_position]);
    }
    for(current_position=0;current_position<node_count;current_position++)//算每個點的neighbor數
    {
        for(node_position=0;node_position<node_count;node_position++)
        {
            if(current_position == node_position)
            {
                continue;
            }
            double square_sum = 0;
            square_sum = square_sum + square(coordinate[current_position][0],coordinate[node_position][0]) + square(coordinate[current_position][1],coordinate[node_position][1]);//算出距離平方
            if(square_sum<1)//如果距離平方<1,代表彼此是鄰居
            {
                link[current_position][node_position] = 1;
                neighbor_count[current_position]++;
            }
        }
    }
    printf("%d %d\n",node_count,repeated_count);
    for(int current=0;current<node_count;current++)//先印本來存進去的值
    {
        printf("%.2lf ",node_value[current]);
    }
    for(int round = 1;round<repeated_count;round++)
    {
        for(current_position=0;current_position<node_count;current_position++)//每收斂一次
        {
            double temp_outcome = 0;
            double self_weighted = 1;
            for(node_position=0;node_position<node_count;node_position++)
            {
                if(link[current_position][node_position]==0)//不是鄰居 OR 算到自己
                {
                    continue;
                }
                if(neighbor_count[current_position]>neighbor_count[node_position])//是鄰居的話才會進這個if,以鄰居數多的那個點來決定權重
                {
                    weighted =1/(2*(neighbor_count[current_position]));
                }
                else
                {
                    weighted =1/(2*(neighbor_count[node_position]));
                }
                self_weighted = self_weighted - weighted;//自己對自己的權重
                temp_outcome = temp_outcome + node_value[node_position] * weighted;
            }
            temp_outcome = temp_outcome + node_value[current_position] * self_weighted;
            outcomes[current_position] = temp_outcome;
        }
        for(int current=0;current<node_count;current++)//把本來的值更改成新的值
        {
            node_value[current] = outcomes[current];
        }
        for(int current=0;current<node_count;current++)
        {
            printf("%.2lf",node_value[current]);
            if(round!=(repeated_count-1) || current!=(node_count-1))//做最後一次收斂的時，最後一個值後面沒有空格
            {
                printf(" ");
            }
        }
    }
    return 0;
}
