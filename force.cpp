#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int max_value_brute_force(int *weights, int *values, int n, int capacity, int *selected_items) {
    int max_value = 0;
    int best_combination[n];

    // 穷举所有可能的组合
    for (int i = 0; i < (1 << n); i++) {
        int current_weight = 0;
        int current_value = 0;
        int combination[n] = {0};

        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) { // 如果第j个物品被选中
                current_weight += weights[j];
                current_value += values[j];
                combination[j] = 1;
            }
        }

        if (current_weight <= capacity && current_value > max_value) {
            max_value = current_value;
            for (int j = 0; j < n; j++) {
                selected_items[j] = combination[j];
            }
        }
    }

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

    int n = 10; // 示例物品数量
    int capacity = 50; // 示例背包容量
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
    int max_value = max_value_brute_force(weights, values, n, capacity, selected_items);
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
