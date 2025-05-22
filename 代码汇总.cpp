#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX_ITEMS 100000           // 最大物品数量
#define MAX_CAPACITY 1000000       // 最大背包容量

// 回溯法全局变量（注意：仅适用于小规模问题）
int max_value_back = 0;
int current_weight_back = 0;
int current_value_back = 0;
int best_selected_back[MAX_ITEMS] = {0};

// 回溯法函数
void backtrack(int *weights, int *values, int n, int capacity, int index, int *selected) {
    if (index == n) {
        if (current_value_back > max_value_back) {
            max_value_back = current_value_back;
            memcpy(best_selected_back, selected, n * sizeof(int));
        }
        return;
    }
    // 不选当前物品
    backtrack(weights, values, n, capacity, index + 1, selected);
    
    // 选当前物品
    if (current_weight_back + weights[index] <= capacity) {
        selected[index] = 1;
        current_weight_back += weights[index];
        current_value_back += values[index];
        backtrack(weights, values, n, capacity, index + 1, selected);
        selected[index] = 0;
        current_weight_back -= weights[index];
        current_value_back -= values[index];
    }
}

int max_value_backtracking(int *weights, int *values, int n, int capacity, int *selected) {
    memset(selected, 0, n * sizeof(int));
    max_value_back = current_weight_back = current_value_back = 0;
    backtrack(weights, values, n, capacity, 0, selected);
    memcpy(selected, best_selected_back, n * sizeof(int));
    return max_value_back;
}

// 动态规划法函数
int max_value_dp(int *weights, int *values, int n, int capacity, int *selected) {
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));
    
    // 初始化DP表
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= capacity; j++)
            dp[i][j] = 0;
    
    // 填充DP表
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= capacity; j++) {
            if (j >= weights[i-1])
                dp[i][j] = MAX(dp[i-1][j], dp[i-1][j - weights[i-1]] + values[i-1]);
            else
                dp[i][j] = dp[i-1][j];
        }
    }
    
    // 回溯选择的物品
    int c = capacity;
    memset(selected, 0, n * sizeof(int));
    for (int i = n; i > 0 && c > 0; i--) {
        if (dp[i][c] != dp[i-1][c]) {
            selected[i-1] = 1;
            c -= weights[i-1];
        }
    }
    
    int max_val = dp[n][capacity];
    // 释放内存
    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    return max_val;
}

// 暴力枚举法函数（仅适用于n<20的情况）
int max_value_brute(int *weights, int *values, int n, int capacity, int *selected) {
    int max_val = 0;
    int best[MAX_ITEMS] = {0};
    
    for (int mask = 0; mask < (1 << n); mask++) {
        int w = 0, v = 0, comb[MAX_ITEMS] = {0};
        for (int j = 0; j < n; j++) {
            if (mask & (1 << j)) {
                w += weights[j];
                v += values[j];
                comb[j] = 1;
            }
        }
        if (w <= capacity && v > max_val) {
            max_val = v;
            memcpy(best, comb, n * sizeof(int));
        }
    }
    memcpy(selected, best, n * sizeof(int));
    return max_val;
}

// 贪心算法结构体和比较函数
typedef struct {
    int weight;
    int value;
    double ratio;
    int index;
} Item;

int compare_items(const void *a, const void *b) {
    Item *i1 = (Item*)a, *i2 = (Item*)b;
    return (i2->ratio > i1->ratio) - (i2->ratio < i1->ratio);
}

int max_value_greedy(int *weights, int *values, int n, int capacity, int *selected) {
    Item items[MAX_ITEMS];
    for (int i = 0; i < n; i++) {
        items[i] = (Item){weights[i], values[i], (double)values[i]/weights[i], i};
    }
    qsort(items, n, sizeof(Item), compare_items);
    
    int total_w = 0, total_v = 0;
    memset(selected, 0, n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        if (total_w + items[i].weight <= capacity) {
            total_w += items[i].weight;
            total_v += items[i].value;
            selected[items[i].index] = 1;
        }
    }
    return total_v;
}

// 通用函数：生成随机物品
void generate_items(int *weights, int *values, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        weights[i] = rand() % 50 + 1;     // 重量1-50
        values[i] = rand() % 200 + 50;    // 价值50-250
    }
}

// 通用函数：打印物品列表
void print_items(int *weights, int *values, int n) {
    printf("\n物品列表（编号-重量-价值）：\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\n", i+1, weights[i], values[i]);
}

// 通用函数：打印结果
void print_result(int *selected, int *weights, int *values, int n, int capacity, int total_v, const char *method) {
    int total_w = 0;
    printf("\n---------------- %s 结果 ----------------\n", method);
    printf("选择的物品（编号-重量-价值）：\n");
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf("%d\t%d\t%d\n", i+1, weights[i], values[i]);
            total_w += weights[i];
        }
    }
    printf("总重量：%d/%d | 总价值：%d\n", total_w, capacity, total_v);
}

int main() {
    int n, capacity, choice;
    int weights[MAX_ITEMS], values[MAX_ITEMS], selected[MAX_ITEMS] = {0};
    
    // 输入参数
    printf("请输入物品数量（建议回溯法/暴力法≤20，动态规划≤1000，贪心任意）：");
    scanf("%d", &n);
    printf("请输入背包容量：");
    scanf("%d", &capacity);
    
    // 生成随机物品
    generate_items(weights, values, n);
    print_items(weights, values, n);
    
    // 算法选择菜单
    printf("\n请选择算法（输入对应数字）：\n");
    printf("1. 回溯法（递归搜索）\n");
    printf("2. 动态规划法（0-1背包标准解法）\n");
    printf("3. 暴力枚举法（仅适用于小规模）\n");
    printf("4. 贪心算法（近似解，非最优）\n");
    scanf("%d", &choice);
    
    clock_t start = clock();
    int max_val = 0;
    
    switch(choice) {
        case 1:
            if (n > 20) {
                printf("警告：回溯法适用于n≤20，当前n=%d可能导致超时！\n", n);
            }
            max_val = max_value_backtracking(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "回溯法");
            break;
        case 2:
            max_val = max_value_dp(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "动态规划法");
            break;
        case 3:
            if (n > 20) {
                printf("错误：暴力枚举法仅适用于n≤20，当前n=%d超出范围！\n", n);
                return 1;
            }
            max_val = max_value_brute(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "暴力枚举法");
            break;
        case 4:
            max_val = max_value_greedy(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "贪心算法");
            break;
        default:
            printf("无效选择！\n");
            return 1;
    }
    
    // 计算执行时间
    double time_ms = ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000;
    printf("\n执行时间：%.2f 毫秒\n", time_ms);
    
    return 0;
}
