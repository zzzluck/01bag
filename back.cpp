#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int max_value = 0;
int current_weight = 0;
int current_value = 0;
int best_selected_items[1000000]; 

void backtrack(int *weights, int *values, int n, int capacity, int index, int *selected_items) {
    if (index == n) {
        if (current_value > max_value) {
            max_value = current_value;
            for (int i = 0; i < n; i++) {
                best_selected_items[i] = selected_items[i];
            }
        }
        return;
    }
    // 不选择当前物品
    backtrack(weights, values, n, capacity, index + 1, selected_items);

    // 选择当前物品
    if (current_weight + weights[index] <= capacity) {
        selected_items[index] = 1;
        current_weight += weights[index];
        current_value += values[index];
        backtrack(weights, values, n, capacity, index + 1, selected_items);
        selected_items[index] = 0;
        current_weight -= weights[index];
        current_value -= values[index];
    }
}

int max_value_backtracking(int *weights, int *values, int n, int capacity, int *selected_items) {
    max_value = 0;
    current_weight = 0;
    current_value = 0;
    backtrack(weights, values, n, capacity, 0, selected_items);
    for (int i = 0; i < n; i++) {
        selected_items[i] = best_selected_items[i];
    }
    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("随机生成的物品信息：\n");
    printf("编号\t重量\t价值\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int capacity) {
    int total_weight = 0;
    printf("\n最终选择的物品信息：\n");
    printf("编号\t重量\t价值\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
            total_weight += weights[i];
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

    // 显示随机生成的物品
    print_items(weights, values, n);

    clock_t start, end;
    double cpu_time_used;

    start = clock(); // 开始计时
    int max_value = max_value_backtracking(weights, values, n, capacity, selected_items);
    end = clock(); // 结束计时

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000; // 转换为毫秒
    printf("\n程序总执行时间: %.2f ms\n", cpu_time_used);

    // 显示最终结果
    print_results(selected_items, weights, values, n, max_value, capacity);

    return 0;
}
