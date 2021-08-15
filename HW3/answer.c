#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define MAX_N 106
#define MAX_R 101
#define MAX_BITS 10
#define INF 0x3f3f3f3f
#define min(a,b) (a < b ? a : b)
#define Debug 0
#define Node_status_greater_than(a, b) (!Node_status_smaller_than(a, b))
#define Vec_greater_than(a, b) (!Vec_smaller_than(a, b))
typedef struct Vector Vector;
typedef struct Node_status Node_status;
typedef struct Q Q;
typedef struct Edge Edge;
typedef struct Node_edge_list Node_edge_list;
typedef struct priority_queue priority_queue;
struct Vector // 陣列 + size
{
    int arr[MAX_N];
    int size;
};
struct Q // 選取的範圍
{
    int x, y;
    int x_len, y_len;
    double utility;
    int internal, overlap;
    Vector rule;
    Vector internal_rule;
};

struct Edge  // 邊，相鄰的點與權重
{
    int to, w;
};

struct Node_edge_list // 紀錄每個點的邊
{
    Edge edge[MAX_N];
    int edge_size;
};

struct Node_status // priority queue裡的element
{
    int dis;
    int hop_cnt;
    int idx;
    Vector path;
};
struct priority_queue // dijkstra時用的容器
{
    int size;
    Node_status *element;
};

int bits; // 輸入的有幾個bits
int src, des; // path的起點與終點
int cap[MAX_N]; // 每個點的容量
int dis[MAX_N]; // 起點到每個點的距離
int par[MAX_N]; // 紀錄上一個點
int hop[MAX_N]; // 紀錄每個點，從起點到每個點走過幾個點
int table[(1 << MAX_BITS) + 5][(1 << MAX_BITS) + 5]; // rule的整數二維陣列
int rule_sz; // 全部的rule數量
char rule[MAX_R][3][MAX_N]; // 所有的rule
Node_edge_list adj[MAX_N]; // 紀錄所有點的相鄰的點

char *dec_to_bin(int num, int len);
char *intersection(char s[], char h[]);
int bin_to_dec(char tmp_s[], int status);
int my_pow2(int num);
int my_log2(int num);
int relatvie_locate(Q a, int rule);
int Vec_smaller_than(Vector a, Vector b);
int Vec_eq(Vector a, Vector b);
int Q_smaller_than(Q a, Q b);
int Node_status_smaller_than(Node_status a, Node_status b);

void print_Q(Q a);
void Q_copy(Q *a, Q b);
void print_table();
void rule_table_print();
void update_table(Q a);
void swap(int *a, int *b);
void Edge_push_back(Node_edge_list *a, Edge b);
void Vec_reverse(Vector *a);
void Vec_push_back(Vector *a, int num);
void Vec_copy(Vector *a, Vector b);
void Vec_print(Vector a);
void Vec_init(Vector *a);
void pq_push(priority_queue *priq, Node_status a);
void pq_pop(priority_queue *priq);
void Node_status_copy(Node_status *a, Node_status b);
void Node_status_swap(Node_status *a, Node_status *b);


Vector make_path(int src);
Node_status make_Node_status(int distance, int hop_cnt, int idx, Vector path);
Node_status make_INF_Node_status();
Node_status pq_top(priority_queue priq);

priority_queue pq;

void initialize() // 初始化
{
    for(int i = 0;i < MAX_N;i++)
    {
        adj[i].edge_size = 0;
    }
    pq.size = 0;
    pq.element = malloc(sizeof(Node_status) * MAX_N * 5);
    for(int i = 0;i < MAX_N * 5;i++)
    {
        Node_status_copy(&(pq.element[i]), make_INF_Node_status()); // 把pq裡的東西全部初始化成距離無限大
    }

}
int main()
{
    if(Debug) freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    initialize();
    scanf("%d", &bits);
    scanf("%d%d", &src, &des);
    int n;
    scanf("%d", &n);
    for(int i = 0;i < n;i++)
    {
        int idx, c;
        scanf("%d%d", &idx, &c);
        cap[i] = c;
    }

    int m;
    scanf("%d", &m);
    for(int i = 0;i < m;i++)
    {
        int a, b, w;
        scanf("%d%d%d", &a, &b, &w);
        Edge to_b, to_a;
        to_b.to = b;
        to_a.to = a;
        to_b.w = w;
        to_a.w = w;
        Edge_push_back(&adj[a], to_b);
        Edge_push_back(&adj[b], to_a);
    }

    int r;
    scanf("%d", &r);
    rule_sz = r;
    for(int i = 0;i < r;i++)
    {
        for(int j = 0;j < 3;j++) scanf("%s", rule[i][j]);
        int src_begin = bin_to_dec(rule[i][0], 0);
        int src_end = bin_to_dec(rule[i][0], 1);
        int des_begin = bin_to_dec(rule[i][1], 0);
        int des_end = bin_to_dec(rule[i][1], 1);
                
        for(int x = src_begin;x <= src_end;x++)
        {
            for(int y = des_begin;y <= des_end;y++)
            {
                if(table[x][y]) continue;
                table[x][y] = i + 1;
            }
        }
    }


    /*dijkstra*/

    /*初始化*/
    memset(dis, INF, sizeof(dis));
    memset(hop, INF, sizeof(hop));

    dis[src] = 0;
    hop[src] = 1;
    par[src] = -1; // 將起點的母節點設為-1
    Vector init; 
    Vec_copy(&init, make_path(src));
    pq_push(&pq, make_Node_status(dis[src], hop[src], src, init));

    /*開始選點*/
    while(pq.size > 0)
    {
        Node_status top;
        Node_status_copy(&top, pq_top(pq)); // 每次都選priority queue裡優先度最大的，存在top裡
        pq_pop(&pq); // pop掉

        int from = top.idx; // 選中from這個點

        /*檢查這個點之前是否被選過了*/
        Vector from_path;
        Vec_copy(&from_path, make_path(from));        
        if(dis[from] < top.dis) continue;
        if(dis[from] == top.dis && hop[from] < top.hop_cnt) continue;
        if(dis[from] == top.dis && hop[from] == top.hop_cnt && Vec_smaller_than(from_path, top.path)) continue;

        for(int i = 0;i < adj[from].edge_size;i++) // 更新所有與from相鄰的點
        {
            int to = adj[from].edge[i].to;
            int w = adj[from].edge[i].w;
            int need_relax = 0;
            if(dis[to] > dis[from] + w) need_relax = 1;
            else if(dis[to] == dis[from] + w && hop[to] > hop[from] + 1) need_relax = 1;
            else if(dis[to] == dis[from] + w && hop[to] == hop[from] + 1)
            {
                Vector path1;
                Vector path2;
                Vec_copy(&path1, make_path(to));
                Vec_copy(&path2, make_path(from));
                if(Vec_smaller_than(path2, path1)) need_relax = 1;
            }

            if(need_relax)
            {
                dis[to] = dis[from] + w;
                hop[to] = hop[from] + 1;
                par[to] = from;
                Vector path;

                Vec_copy(&path, make_path(to));
                pq_push(&pq, make_Node_status(dis[to], hop[to], to, path));
            }
        }
    }

    Vector ans_switch;
    Vec_copy(&ans_switch, make_path(des));
    // Vec_print(ans_switch);
    printf("%d\n", ans_switch.size);
    for(int idx = 0;idx < ans_switch.size;idx++)
    {
        int cur_node = ans_switch.arr[idx]; // 現在這個點的編號
        Q cur_Q;

        /*初始化*/
        cur_Q.utility = -INF;
        Vec_init(&cur_Q.rule);
        Vec_init(&cur_Q.internal_rule);
        int pow2_bits = my_pow2(bits);

        for(int i = 1;i <= pow2_bits;i *= 2) // i為x的長度範圍，i = 1, 2, 4, 8...
        {
            for(int j = 1;j <= pow2_bits;j *= 2) // j為y的長度範圍，j = 1, 2, 4, 8...
            {
                for(int x = 0;x < pow2_bits;x += i) 
                {
                    for(int y = 0;y < pow2_bits;y += j)
                    {
                        // (x, y)為目前所選的Q的左上角的點
                        // 目前所選的Q叫a
                        int cnt_overlap = 0;
                        int cnt_internal = 0;

                        Q a;
                        Vec_init(&a.rule); Vec_init(&a.internal_rule);
                        a.x = x, a.x_len = i, a.y = y, a.y_len = j;

                        for(int k = 0;k < rule_sz;k++)
                        {
                            // 計算每個點的rule的狀態，觀察是否為internal或overlaping或不相交
                            char *r_src = rule[k][0];
                            char *r_des = rule[k][1];

                            int r_src_begin = bin_to_dec(r_src, 0);
                            int r_src_end = bin_to_dec(r_src, 1);
                            int r_des_begin = bin_to_dec(r_des, 0);
                            int r_des_end = bin_to_dec(r_des, 1);

                            int in = 0, out = 0; // in = 1代表rule出現在a的範圍裡 // out = 1代表出現在a的範圍外
                            
                            /*將這個rule的範圍全部掃過*/
                            for(int r_x = r_src_begin;r_x <= r_src_end;r_x++)
                            {
                                for(int r_y = r_des_begin;r_y <= r_des_end;r_y++)
                                {
                                    if((x <= r_x && r_x < x + i) && (y <= r_y && r_y < y + j)) 
                                    {
                                        if(table[r_x][r_y] != k + 1) continue;
                                        // 如果出現在a的範圍裡
                                        in = 1;
                                    }
                                    else
                                    {
                                        // 如果出現在a的範圍外
                                        out = 1;
                                    }
                                }
                            }

                            if(!in) continue;

                            Vec_push_back(&(a.rule), k);
                            
                            if(out == 1) cnt_overlap++;
                            if(out == 0)
                            {
                                Vec_push_back(&(a.internal_rule), k);
                                cnt_internal++;
                            }
                        }                      
                        
                        // 如果這個Q超過容量，不考慮
                        if(cap[cur_node] < cnt_overlap + cnt_internal) continue;
                        
                        // 如果utility是負數，不考慮
                        a.utility = (cnt_internal - 1.0) / (cnt_overlap + 1.0);
                        if(a.utility <= (1e-6)) continue;
                        
                        a.internal = cnt_internal;
                        a.overlap = cnt_overlap;
                        
                        // 比較這個a與現有的答案，若更好就將現有的答案更新成a
                        if(Q_smaller_than(cur_Q, a)) Q_copy(&cur_Q, a);
                    }
                }
            }
        }

        printf("%d %d\n", cur_node, cur_Q.rule.size);
        print_Q(cur_Q);
        update_table(cur_Q);
        // print_table();
    }

    return 0;
}

int Vec_smaller_than(Vector a, Vector b) //比較是否a < b，
{
    // Vec_print(a), Vec_print(b);
    for(int i = 0;i < min(a.size, b.size);i++)
    {
        if(a.arr[i] != b.arr[i]) 
        {
            return a.arr[i] < b.arr[i]; // 一個一個元素比較，遇到不同即回傳
        }
    }
    return (a.size < b.size); // 若前幾個數字都一樣，檢查size
}
int Vec_eq(Vector a, Vector b)
{
    if(a.size != b.size) return 0;
    for(int i = 0;i < a.size;i++)
    {
        if(a.arr[i] != b.arr[i]) return 0;
    }
    return 1;
}

char *intersection(char s[], char h[]) // s跟h的intersection 其實就是選小的那一個
{
    int s_len = strlen(s), h_len = strlen(h);
    if(s_len < h_len) return h; // 比較長度 看誰的星號在前面
    if(s_len > h_len) return s;
    if(s[s_len - 1] == '*') return h; // 若長度都為bits，檢查是否是最後一個有星號
    if(h[h_len - 1] == '*') return s;
    return s; // 做到這邊代表s, h一樣
}

int bin_to_dec(char tmp_s[], int status) // 二進位轉十進位, status = 0代表*後面填0， = 1代表後面填1
{
    char s[MAX_BITS]; 
    strcpy(s, tmp_s);
    int len = strlen(s);
    if(s[len - 1] == '*') // 將*號去掉
    {
        s[len - 1] = '\0';
        len--;
    }

    int dif = bits - len;
    for(int i = 0;i < dif;i++) // 將*號後面的數字補滿
    {
        if(status == 0) strcat(s, "0");
        if(status == 1) strcat(s, "1");
    }
    int res = 0;
    for(int i = 0;i < bits;i++) // 二進位轉成十進位
    {
        res *= 2;
        res += (s[i] - '0');
    }

    return res;
}

int my_pow2(int num) // 2的num次方
{
    return (1 << num);
}
int my_log2(int num) // log2(num)
{
    int res = 0;
    while(num != 1)
    {
        res++;
        num = num / 2;
    }
    return res;
}

char *dec_to_bin(int num, int len) // 十進位轉二進位
{
    char *res = (char *)malloc(sizeof(char) * (MAX_BITS + 5));
    int digit = (1 << (bits - 1));
    len = my_log2(len);
    for(int i = 0;i < bits - len;i++) // digit會將num的010101掃過
    {
        res[i] = ((num & (digit)) > 0) + '0'; // 檢查num的該位bit為0或1
        digit >>= 1; // 掃下一個位數
    }
    if(len > 0) // 如果有星號
    {
        res[bits - len] = '*'; // 尾端填星號
        res[bits - len + 1] = '\0';
    }
    else
    {
        res[bits] = '\0';
    }
    return res; 
}

void print_Q(Q a) // 選取a時，將所有與a相交rule的intersection print出來
{
    if(a.utility <= 0) return;
    char *a_src = dec_to_bin(a.x, a.x_len);
    char *a_des = dec_to_bin(a.y, a.y_len);
    for(int i = 0;i < a.rule.size;i++)
    {
        int cur_rule = a.rule.arr[i];
        printf("%s %s %s\n", intersection(a_src, rule[cur_rule][0]), intersection(a_des, rule[cur_rule][1]), rule[cur_rule][2]);
    }
}

void update_table(Q a) // 選取a時，更新table
{
    if(a.utility <= 0) return;
    char *a_src = dec_to_bin(a.x, a.x_len);
    char *a_des = dec_to_bin(a.y, a.y_len);

    int a_src_begin = bin_to_dec(a_src, 0);
    int a_src_end = bin_to_dec(a_src, 1);
    int a_des_begin = bin_to_dec(a_des, 0);
    int a_des_end = bin_to_dec(a_des, 1);
    
    for(int x = a_src_begin;x <= a_src_end;x++)
    {
        for(int y = a_des_begin;y <= a_des_end;y++)
        {
            table[x][y] = 1;
        }
    }
    
    int abandon_rule[MAX_R];
    for(int i = 0;i < rule_sz;i++) abandon_rule[i] = 1;

    for(int x = 0;x < my_pow2(bits);x++)
    {
        for(int y = 0;y < my_pow2(bits);y++)
        {
            if(a_src_begin <= x && x <= a_src_end && a_des_begin <= y && y <= a_des_end) continue;

            abandon_rule[table[x][y] - 1] = 0;
        }
    }


    char new_rule[MAX_R][3][MAX_R];
    for(int i = 0;i < MAX_R;i++)
    {
        for(int j = 0;j < 3;j++) new_rule[i][j][0] = '\0';
    }
    strcpy(new_rule[0][0], a_src);
    strcpy(new_rule[0][1], a_des);
    strcpy(new_rule[0][2], "Fwd");
    int new_sz = 1;

    for(int i = 0;i < rule_sz;i++)
    {
        if(abandon_rule[i]) continue;

        strcpy(new_rule[new_sz][0], rule[i][0]);
        strcpy(new_rule[new_sz][1], rule[i][1]);
        strcpy(new_rule[new_sz][2], rule[i][2]);
        new_sz++;
    }

    rule_sz = new_sz;
    

    memset(table, 0, sizeof(table));
    for(int i = 0;i < rule_sz;i++)
    {
        strcpy(rule[i][0], new_rule[i][0]);
        strcpy(rule[i][1], new_rule[i][1]);
        strcpy(rule[i][2], new_rule[i][2]);
    
        int src_begin = bin_to_dec(rule[i][0], 0);
        int src_end = bin_to_dec(rule[i][0], 1);
        int des_begin = bin_to_dec(rule[i][1], 0);
        int des_end = bin_to_dec(rule[i][1], 1);
        
        for(int x = src_begin;x <= src_end;x++)
        {
            for(int y = des_begin;y <= des_end;y++)
            {
                if(table[x][y]) continue;
                table[x][y] = i + 1;
            }
        }
    }
}


void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
void print_table()
{
    printf("sz:%d\n", rule_sz);
    for(int i = 0;i < my_pow2(bits);i++)
    {
        for(int j = 0;j < my_pow2(bits);j++)
            printf("%d ", table[i][j]);
        printf("\n");
    }
}


Vector make_path(int src) // 製造一條，從起點到該點的路徑
{
    Vector path; Vec_init(&path);
    while(src != -1)
    {
        Vec_push_back(&path, src);
        src = par[src];
    }
    Vec_reverse(&path);
    return path;
}
void Vec_push_back(Vector *a, int num) // 將num加在a的尾端
{
    a->arr[(a->size++)] = num;
}
void Vec_reverse(Vector *a) // 將a的element倒序
{
    int sz = a->size;
    for(int i = 0;i < sz / 2;i++) swap(&(a->arr[i]), &(a->arr[sz - i - 1]));
}
void Vec_copy(Vector *a, Vector b)
{
    int sz = b.size;
    a->size = sz;
    for(int i = 0;i < sz;i++)
    {
        a->arr[i] = b.arr[i];
    }
}
void Vec_print(Vector a)
{
    for(int i = 0;i < a.size;i++)
    {
        printf("%d ", a.arr[i]);
    }
    printf("\n");
}
void rule_table_print()
{
    printf("---start---\n");
    for(int i = 0;i < rule_sz;i++)
    {
        printf("%s %s %s\n", rule[i][0], rule[i][1], rule[i][2]);
    }
    printf("---end---\n");
}
void Vec_init(Vector *a) // 初始化
{
    a->size = 0;
}


int Node_status_smaller_than(Node_status a, Node_status b)
{
    if(a.dis != b.dis) return a.dis > b.dis;
    else if(a.hop_cnt != b.hop_cnt) return a.hop_cnt > b.hop_cnt;
    else return Vec_greater_than(a.path, b.path);
}
Node_status make_Node_status(int distance, int hop_cnt, int idx, Vector path)
{
    Node_status a;
    a.dis = distance;
    a.hop_cnt = hop_cnt;
    a.idx = idx;
    Vec_copy(&a.path, path);
    return a;
}
Node_status make_INF_Node_status() // 製造優先度無限低的Node在priority queue
{
    Node_status pos_INF;
    pos_INF.dis = INF;
    pos_INF.hop_cnt = INF;
    pos_INF.idx = INF;
    Vec_init(&(pos_INF.path));
    return pos_INF;
}

int Q_smaller_than(Q a, Q b) // 比較a跟b的優先度
{
    if(fabs(a.utility - b.utility) > 1e-6) 
        return a.utility < b.utility;
    if(a.internal != b.internal) 
        return a.internal < b.internal;
    if(a.x_len * a.y_len != b.x_len * b.y_len) 
        return a.x_len * a.y_len > b.x_len * b.y_len;
    
    return Vec_greater_than(a.internal_rule, b.internal_rule);
}
void Q_copy(Q *a, Q b) // a = b
{
    a->x = b.x;
    a->x_len = b.x_len;
    a->y = b.y;
    a->y_len = b.y_len;
    a->internal = b.internal;
    a->overlap = b.overlap;
    a->utility = b.utility;
    Vec_copy(&(a->rule), b.rule);
    Vec_copy(&(a->internal_rule), b.internal_rule);
}


void Edge_push_back(Node_edge_list *a, Edge b) // 將這個邊加在尾端
{
    int sz = (*a).edge_size;
    (*a).edge[sz].to = b.to;
    (*a).edge[sz].w = b.w;
    (*a).edge_size++;
}


void pq_push(priority_queue *priq, Node_status a)
{
    priq->size++;
    int sz = priq->size;
    Node_status_copy(&(priq->element[sz]), a);

    int cur = sz;
    while(cur != 1 && Node_status_greater_than(priq->element[cur], priq->element[cur / 2]))
    {
        Node_status_swap(&(priq->element[cur]), &(priq->element[cur / 2]));
        cur = cur / 2;
    }
}
void pq_pop(priority_queue *priq)
{
    // 將element[1] pop掉
    Node_status_copy(&priq->element[1], priq->element[priq->size]); // 將底下元素複製上來
    Node_status_copy(&(priq->element[priq->size]), make_INF_Node_status()); // 用距離無限大填滿
    priq->size--;
    if(priq->size == 0) return;
    
    //要開始將element[1]往下放，放到該有的位置
    int cur = 1; // 一開始在1會逐漸往下
    while(1)
    {
        Node_status left, right;
        Node_status_copy(&left, priq->element[cur * 2]); // 左節點
        Node_status_copy(&right, priq->element[cur * 2 + 1]); // 右節點
        int left_res = Node_status_smaller_than(priq->element[cur], left); // 與左節點比較的結果
        int right_res = Node_status_smaller_than(priq->element[cur], right); // 與右節點
        if(left_res == 0 && right_res == 0) break; // 如果這點已經比子節點都大了，代表完成max heap
        if(Node_status_smaller_than(left, right)) // 往比較大的節點走
        {
            // 如果右節點比較大
            // 跟右節點換
            Node_status_swap(&(priq->element[cur]), &(priq->element[cur * 2 + 1]));
            cur = cur * 2 + 1;
        }
        else
        {
            // 如果左節點比較大
            // 跟左節點換
            Node_status_swap(&(priq->element[cur]), &(priq->element[cur * 2]));
            cur = cur * 2;            
        }
    }
}
void Node_status_swap(Node_status *a, Node_status *b)
{
    Node_status tmp;
    Node_status_copy(&tmp, *a);
    Node_status_copy(a, *b);
    Node_status_copy(b, tmp);
}
void Node_status_copy(Node_status *a, Node_status b)
{
    a->dis = b.dis;
    a->hop_cnt = b.hop_cnt;
    a->idx = b.idx;
    Vec_copy(&(a->path), b.path);
}
Node_status pq_top(priority_queue priq)
{
    return priq.element[1];
}
