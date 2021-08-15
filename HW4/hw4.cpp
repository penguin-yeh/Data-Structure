#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <functional>
using namespace std;

void init(bool **bits,int m,int r,int **a,int **b,int p)
{
    *bits =(bool*)malloc(sizeof(bool)*m);
    *a = (int *)malloc(sizeof(int)*r);
    *b = (int *)malloc(sizeof(int)*r);
    srand(1);
    for(int i=0;i<r;i++)
    {
        *(*(a)+i) = rand()%(p-1)+1;
    }
    srand(2);
    for(int i=0;i<r;i++)
    {
        *(*(b)+i) = rand()%(p-1)+1;
    }
}

int myhash(char *str,int count,int m,int r,int p,int *a,int *b)
{
    size_t key;
    std::hash<string> hasher;
    key = hasher(str);
    return (a[count]*key + b[count]) % p % m;
}

void insert(bool *bits,int m,int r,int p,char *str,int *a,int *b)
{
    for(int i=0;i<r;i++)
    {
        bits[myhash(str,i,m,r,p,a,b)] = 1;
    }
}

bool query(bool *bits,int m,int p,int r,char *str,int *a,int *b)
{
    for(int i=0;i<r;i++)
    {
        if(bits[myhash(str,i,m,r,p,a,b)]==0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    bool *bits;
    int *a;
    int *b;
    int m,r,p;
    int word_cnt;
    int test_cnt;
    char str[1000];
    scanf("%d",&m);
    scanf("%d",&r);
    scanf("%d",&word_cnt);
    scanf("%d",&test_cnt);
    scanf("%d",&p);
    init(&bits,m,r,&a,&b,p);
    for(int i=0;i<word_cnt;i++)
    {
        scanf("%s",str);
        insert(bits,m,r,p,str,a,b);
    }
    for(int i=0;i<test_cnt;i++)
    {
        scanf("%s",str);
        if(query(bits,m,p,r,str,a,b)==1)
        {
            printf("%s: true\n",str);
        }
        else
        {
            printf("%s: false\n",str);
        }
    }
    return 0;
}
