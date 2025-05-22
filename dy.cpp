#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int max_value_dynamic_programming(int *weights, int *values, int n, int capacity, int *selected_items) {
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));
    }

    // 初始化dp数组
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = 0;
        }
    }

    // 动态规划填表
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= capacity; j++) {
            if (j >= weights[i - 1]) {
                dp[i][j] = MAX(dp[i - 1][j], dp[i - 1][j - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    int max_value = dp[n][capacity];

    // 回溯找到选择的物品
    int current_capacity = capacity;
    for (int i = n; i > 0 && current_capacity > 0; i--) {
        if (dp[i][current_capacity] != dp[i - 1][current_capacity]) {
            selected_items[i - 1] = 1;
            current_capacity -= weights[i - 1];
        }
    }

    // 释放内存
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("随机生成的物品列表：\n");
    printf("编号\t重量\t价值\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int total_weight, int capacity) {
    printf("\n最终选择的物品：\n");
    printf("编号\t重量\t价值\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
        }
    }
    printf("总重量: %d/%d\n", total_weight, capacity);
    printf("总价值: %d\n", total_value);
}

int main() {
    srand(time(NULL));

    int n = 10; // 减少物品数量以便查看结果
    int capacity = 50; // 调整背包容量
    int weights[n];
    int values[n];
    int selected_items[n] = {0};

    // 随机生成物品的重量和价值
    for (int i = 0; i < n; i++) {
        weights[i] = rand() % 100 + 1; // 重量在1到100之间
        values[i] = rand() % 900 + 100; // 价值在100到1000之间
    }

    clock_t start, end;
    double cpu_time_used;

    // 打印随机生成的物品
    print_items(weights, values, n);

    start = clock(); // 开始计时
    int max_value = max_value_dynamic_programming(weights, values, n, capacity, selected_items);
    end = clock(); // 结束计时

    // 计算总重量
    int total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            total_weight += weights[i];
        }
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000; // 转换为毫秒
    printf("\n程序执行时间: %.2f ms\n", cpu_time_used);

    // 打印结果
    print_results(selected_items, weights, values, n, max_value, total_weight, capacity);

    return 0;
}
