#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int max_value_dynamic_programming(int *weights, int *values, int n, int capacity, int *selected_items) {
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));
    }

    // ��ʼ��dp����
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= capacity; j++) {
            dp[i][j] = 0;
        }
    }

    // ��̬�滮���
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

    // �����ҵ�ѡ�����Ʒ
    int current_capacity = capacity;
    for (int i = n; i > 0 && current_capacity > 0; i--) {
        if (dp[i][current_capacity] != dp[i - 1][current_capacity]) {
            selected_items[i - 1] = 1;
            current_capacity -= weights[i - 1];
        }
    }

    // �ͷ��ڴ�
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("������ɵ���Ʒ�б�\n");
    printf("���\t����\t��ֵ\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int total_weight, int capacity) {
    printf("\n����ѡ�����Ʒ��\n");
    printf("���\t����\t��ֵ\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
        }
    }
    printf("������: %d/%d\n", total_weight, capacity);
    printf("�ܼ�ֵ: %d\n", total_value);
}

int main() {
    srand(time(NULL));

    int n = 10; // ������Ʒ�����Ա�鿴���
    int capacity = 50; // ������������
    int weights[n];
    int values[n];
    int selected_items[n] = {0};

    // ���������Ʒ�������ͼ�ֵ
    for (int i = 0; i < n; i++) {
        weights[i] = rand() % 100 + 1; // ������1��100֮��
        values[i] = rand() % 900 + 100; // ��ֵ��100��1000֮��
    }

    clock_t start, end;
    double cpu_time_used;

    // ��ӡ������ɵ���Ʒ
    print_items(weights, values, n);

    start = clock(); // ��ʼ��ʱ
    int max_value = max_value_dynamic_programming(weights, values, n, capacity, selected_items);
    end = clock(); // ������ʱ

    // ����������
    int total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            total_weight += weights[i];
        }
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000; // ת��Ϊ����
    printf("\n����ִ��ʱ��: %.2f ms\n", cpu_time_used);

    // ��ӡ���
    print_results(selected_items, weights, values, n, max_value, total_weight, capacity);

    return 0;
}
