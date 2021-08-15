#include <stdio.h>
#include <stdlib.h>

struct node;  //先宣告在前面，不然struct list會找不到struct node
struct list{
    struct node* data;
    struct list* next;
};
struct node{
    int id;
    struct list* children;
    struct node* parent;
};
typedef struct node* node_ptr;
typedef struct list* list_ptr;

list_ptr input_id(list_ptr original,int id)
{
    list_ptr temp;
    temp = (list_ptr)malloc(sizeof(struct list));
    temp->data = (node_ptr)malloc(sizeof(struct node));
    temp->data->id = id;
    if(original == NULL)
    {
        original = temp;
        return original;
    }
    list_ptr prev;
    prev = original;
    while(prev->next)
    {
        prev = prev->next;
    }
    prev->next = temp;
    temp->next = NULL;
    return original;
}

list_ptr link_parent(list_ptr original,list_ptr current,list_ptr parent)
{
    if(parent == NULL)
    {
        current->data->parent = NULL;
        return original;
    }
    current->data->parent = parent->data;
    return original;
}

list_ptr link_parent_id(list_ptr original,int current_id,int parent_id)
{
    list_ptr current;
    list_ptr parent;
    current = original;
    parent = original;
    for(int id=0;id<current_id;id++)
    {
        current = current->next;
    }
    if(parent_id==-1)
    {
        current->data->parent = NULL;
        return original;
    }
    for(int id=0;id<parent_id;id++)
    {
        parent = parent->next;
    }
    current->data->parent = parent->data;
    return original;
}

list_ptr link_child(list_ptr original,int current_id,int child_id)
{
    list_ptr temp;
    temp = (list_ptr)malloc(sizeof(struct list));
    temp->data = (node_ptr)malloc(sizeof(struct node));
    temp->data->id = child_id;
    list_ptr current;
    current = original;
    for(int id=0;id<current_id;id++)
    {
        current = current->next;
    }
    if(current->data->children == NULL)
    {
        current->data->children = temp;
        temp->next = NULL;
        return original;
    }    
    else
    {
        list_ptr prev;
        prev = current->data->children;
        while(prev->next)
        {
            prev = prev->next;
        }
        prev->next = temp;
        temp->next = NULL;
    }
    return original;
}
int main()
{
    list_ptr original = NULL;
    list_ptr goal = NULL;
    list_ptr tail;
    int repeated_cnt = 1;
    int node_cnt;
    int next_tail[1000];
    int tail_cnt = 0;
    int child_table[1000];
    int child_cnt=0;
    int isolated_node[100];
    int isolation=0;
    scanf("%d",&node_cnt);
    for(int id=0;id<node_cnt;id++)//initial int array
    {
        next_tail[id]=0;
        child_table[id]=0;
        isolated_node[id]=0;
    }
    for(int position=0;position<node_cnt;position++)//先把original,goal中每個node的id設好
    {
        original = input_id(original,position);
    }
    for(int position=0;position<node_cnt;position++)
    {
        goal = input_id(goal,position);
    }

    for(int id=0;id<node_cnt;id++)//把original中每個node的parent串起來
    {
        int parent_id;
        scanf("%d",&parent_id);
        original = link_parent_id(original,id,parent_id);
    }
    for(int id=0;id<node_cnt;id++)//把goal中每個node的parent串起來(這裡的parent_id可能為-1)
    {
        int parent_id;
        scanf("%d",&parent_id);
        goal = link_parent_id(goal,id,parent_id);
    }
    list_ptr temp;
    temp = goal;
    for(int goal_id=0;goal_id<node_cnt;goal_id++)//先由goal把original每個node的children連起來
    {    
        if(temp->data->parent == NULL)
        {
            temp = temp->next;
            continue;
        }
        original = link_child(original,temp->data->parent->id,temp->data->id);
        goal = link_child(goal,temp->data->parent->id,temp->data->id);//之後算round時會用到
        child_table[temp->data->parent->id]++;//把每個node的child個數記起來，之後用來算round
        temp = temp -> next;
    }
    temp = original;
    tail = goal;
    while(tail)//找到goal的結尾
    {
        if(tail->data->parent == NULL && tail->data->children != NULL)
        {
            break;
        }
        tail = tail->next;
    }
    temp = goal;
    int lost_cnt = 0;//isolated node_cnt;
    while(temp)//看哪些node到最後會是isolated
    {
        if(temp->data->parent == NULL && temp->data->children == NULL)
        {
            isolated_node[lost_cnt] = temp->data->id;//把isolated的node_id記起來
            lost_cnt++;
        }
        temp = temp->next;
    }
    int fisrt_round=0;
    int next_round[100];//下次要跑的node_id
    int next_round_cnt = 0;//下次要跑幾次
    while(child_cnt!=(node_cnt-1))//算總共要做幾次
    {
        if(fisrt_round == 0)
        {
            child_cnt = child_table[tail->data->id] + lost_cnt;
            fisrt_round=1;
            while(tail->data->children)
            {
                next_round[next_round_cnt] = tail->data->children->data->id;
                next_round_cnt++;
                tail->data->children = tail->data->children->next;
            }
            repeated_cnt++;
        }
        else
        {
            list_ptr temp;
            int current_next_round_cnt;
            int current_next_round[100];
            temp = original;
            current_next_round_cnt = next_round_cnt;
            next_round_cnt=0;
            for(int num=0;num<current_next_round_cnt;num++)
            {
                current_next_round[num] = next_round[num];
            }
            for(int id=0;id<current_next_round_cnt;id++)//有幾個tail就跑幾次
            {
                child_cnt = child_cnt + child_table[current_next_round[id]];//這個tail有幾個child
                if(id==0)
                {
                    for(int position=0;position<current_next_round[id];position++)
                    {
                        temp = temp->next;
                    }
                }
                else if(id!=0)
                {
                    for(int position=0;position<current_next_round[id]-current_next_round[id-1];position++)
                    {
                        temp = temp->next;
                    }
                }
                while(temp->data->children)
                {
                    next_round[next_round_cnt] = temp->data->children->data->id;
                    next_round_cnt++;
                    temp->data->children = temp->data->children->next;
                }
            }
            repeated_cnt++;
        }
    }
    printf("%d\n",repeated_cnt);
    temp = original;
    for(int id=0;id<node_cnt;id++)
    {
        if(temp->data->parent == NULL)
        {
            printf("-1 ");
            temp = temp->next;
            continue;
        }
        printf("%d ",temp->data->parent->id);
        temp = temp->next;
    }
    printf("\n");
    temp = goal;
    for(int id=0;id<node_cnt;id++)//goal的child清空
    {
        temp->data->children = NULL;
        temp = temp->next;
    }
    temp = original;
    for(int id=0;id<node_cnt;id++)//original的child清空
    {
        temp->data->children = NULL;
        temp = temp->next;
    }
    temp = goal;
    for(int goal_id=0;goal_id<node_cnt;goal_id++)//全部child重新接一次
    {    
        if(temp->data->parent == NULL)
        {
            temp = temp->next;
            continue;
        }
        original = link_child(original,temp->data->parent->id,temp->data->id);
        goal = link_child(goal,temp->data->parent->id,temp->data->id);
        temp = temp -> next;
    }
    tail = goal;
    while(tail)//找tail
    {
        if(tail->data->parent == NULL && tail->data->children != NULL)//只要符合這個條件，就代表找到tail了
        {
            break;
        }
        tail = tail->next;
    }
    int fisrt = 0;
    for(int count=1;count<repeated_cnt;count++)
    {
        list_ptr current;
        list_ptr child;
        list_ptr temp;
        list_ptr lost;
        current = original;
        child = original;
        temp = original;
        lost = original;
        isolation = 0;
        if(fisrt == 0)
        {
            for(int id=0;id < tail->data->id;id++)//先移到tail的位置
            {
                current = current->next;
            }
            for(int id=0;id<lost_cnt;id++)//如果有個isolated node的parent是tail,則把這個isolated node的parent改成NULL     
            {
                lost = original;
                for(int lost_id=0;lost_id<isolated_node[id];lost_id++)
                {
                    lost = lost->next;
                }
                if(lost->data->parent->id == current->data->id)
                {
                    lost->data->parent = NULL;
                }
            }
            while(current->data->children)//更改parent
            {
                child = original;
                for(int id=0;id<current->data->children->data->id;id++)
                {
                    child = child->next;
                }
                original = link_parent(original,child,current);
                next_tail[tail_cnt] = child->data->id;
                tail_cnt++;
                current->data->children = current->data->children->next;
            }
            fisrt = 1;
            for(int id=0;id<node_cnt;id++)
            {
                if(temp->data->parent==NULL)
                {
                    printf("-1 ");
                }
                else
                {
                    printf("%d ",temp->data->parent->id);
                }
                temp = temp->next;
            }
            printf("\n");
            continue;
        }
        else if(fisrt!= 0)
        {
            int current_tail_cnt;
            int current_next_tail[100];
            isolation = 0;
            current_tail_cnt = tail_cnt;
            tail_cnt = 0;
            for(int num=0;num<current_tail_cnt;num++)//copy next_tail到新的陣列
            {
                current_next_tail[num] = next_tail[num];
            }
            for(int tail_id=0;tail_id<current_tail_cnt;tail_id++)//有幾個tail,做幾次
            {
                if(tail_id == 0)
                {
                    for(int next_id=0;next_id<current_next_tail[tail_id];next_id++)
                    {
                        current = current->next;
                    }
                }
                else
                {
                    for(int next_id=0;next_id<current_next_tail[tail_id]-current_next_tail[tail_id-1];next_id++)
                    {
                        current = current->next;
                    }
                }
                for(int id=0;id<lost_cnt;id++)
                {
                    lost = original;
                    for(int lost_id=0;lost_id<isolated_node[id];lost_id++)
                    {
                        lost = lost->next;
                    }
                    if(lost->data->parent->id == current->data->id)
                    {
                        lost->data->parent = NULL;
                    }
                }
                while(current->data->children)
                {
                    child = original;
                    for(int id=0;id<current->data->children->data->id;id++)
                    {
                        child = child->next;
                    }
                    original = link_parent(original,child,current);
                    next_tail[tail_cnt] = child->data->id;
                    tail_cnt++;
                    current->data->children = current->data->children->next;
                }
            }
            temp = original;
            for(int id=0;id<node_cnt;id++)
            {
                if(temp->data->parent==NULL)
                {
                    printf("-1 ");
                }
                else
                {
                    printf("%d ",temp->data->parent->id);
                }
                temp = temp->next;
            }
        }
        printf("\n");
    }
    return 0;
}
