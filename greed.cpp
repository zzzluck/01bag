#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int weight;
    int value;
    double ratio;
    int index;
} Item;

int compare(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    return (item2->ratio > item1->ratio) - (item2->ratio < item1->ratio);
}

int max_value_greedy(int *weights, int *values, int n, int capacity, int *selected_items) {
    Item items[n];
    for (int i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = (double)values[i] / weights[i];
        items[i].index = i;
    }

    // 按单位价值从高到低排序
    qsort(items, n, sizeof(Item), compare);

    int max_value = 0;
    int current_capacity = capacity;

    for (int i = 0; i < n; i++) {
        if (current_capacity >= items[i].weight) {
            max_value += items[i].value;
            current_capacity -= items[i].weight;
            selected_items[items[i].index] = 1;
        }
    }

    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("随机生成的物品列表：\n");
    printf("编号\t重量\t价值\t单位价值\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%.2f\n", i + 1, weights[i], values[i], (double)values[i] / weights[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int total_weight, int capacity) {
    printf("\n最终选择的物品：\n");
    printf("编号\t重量\t价值\t单位价值\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\t%.2f\n", i + 1, weights[i], values[i], (double)values[i] / weights[i]);
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
    int max_value = max_value_greedy(weights, values, n, capacity, selected_items);
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
