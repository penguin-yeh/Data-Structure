#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double square(double input_1,double input_2)//��X��ƥ��誺�t�Z

{
    return (input_1 - input_2)*(input_1 - input_2);
}

int main()
{
    int node_count;
    int repeated_count;
    double node_value[100];
    double coordinate[100][2];//[][0]�sx��,[][1]�sy��
    double weighted;
    double neighbor_count[100];
    double outcomes[100];
    int link[100][100];//�P�_���I�O���Oneighbor
    int node_position;
    int current_position;
    scanf("%d %d",&node_count,&repeated_count);
    for(int count=0;count<node_count;count++)//��C��node�����Ū�J
    {
        scanf("%d",&node_position);
        scanf("%lf %lf %lf",&coordinate[node_position][0],&coordinate[node_position][1],&node_value[node_position]);
    }
    for(current_position=0;current_position<node_count;current_position++)//��C���I��neighbor��
    {
        for(node_position=0;node_position<node_count;node_position++)
        {
            if(current_position == node_position)
            {
                continue;
            }
            double square_sum = 0;
            square_sum = square_sum + square(coordinate[current_position][0],coordinate[node_position][0]) + square(coordinate[current_position][1],coordinate[node_position][1]);//��X�Z������
            if(square_sum<1)//�p�G�Z������<1,�N�����O�F�~
            {
                link[current_position][node_position] = 1;
                neighbor_count[current_position]++;
            }
        }
    }
    printf("%d %d\n",node_count,repeated_count);
    for(int current=0;current<node_count;current++)//���L���Ӧs�i�h����
    {
        printf("%.2lf ",node_value[current]);
    }
    for(int round = 1;round<repeated_count;round++)
    {
        for(current_position=0;current_position<node_count;current_position++)//�C���Ĥ@��
        {
            double temp_outcome = 0;
            double self_weighted = 1;
            for(node_position=0;node_position<node_count;node_position++)
            {
                if(link[current_position][node_position]==0)//���O�F�~ OR ���ۤv
                {
                    continue;
                }
                if(neighbor_count[current_position]>neighbor_count[node_position])//�O�F�~���ܤ~�|�i�o��if,�H�F�~�Ʀh�������I�ӨM�w�v��
                {
                    weighted =1/(2*(neighbor_count[current_position]));
                }
                else
                {
                    weighted =1/(2*(neighbor_count[node_position]));
                }
                self_weighted = self_weighted - weighted;//�ۤv��ۤv���v��
                temp_outcome = temp_outcome + node_value[node_position] * weighted;
            }
            temp_outcome = temp_outcome + node_value[current_position] * self_weighted;
            outcomes[current_position] = temp_outcome;
        }
        for(int current=0;current<node_count;current++)//�⥻�Ӫ��ȧ�令�s����
        {
            node_value[current] = outcomes[current];
        }
        for(int current=0;current<node_count;current++)
        {
            printf("%.2lf",node_value[current]);
            if(round!=(repeated_count-1) || current!=(node_count-1))//���̫�@�����Ī��ɡA�̫�@�ӭȫ᭱�S���Ů�
            {
                printf(" ");
            }
        }
    }
    return 0;
}
